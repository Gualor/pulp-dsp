#!/usr/bin/env python3

import numpy as np


##################
# compute_result #
##################


def compute_result(result_parameter, inputs, env, fix_point):
    """
    Funciton to generate the expected result of the testcase.

    Arguments
    ---------
    result_parameter: Either OutputArgument or ReturnValue (see pulp_dsp_test.py)
    inputs: Dict mapping name to the Argument, with arg.value, arg.ctype (and arg.length)
    env: Dict mapping the variable (SweepVariable or DynamicVariable) names to their value.
    fix_point: None (if no fixpoint is used) or decimal point
    """
    isParallel = False
    if 'nPE' in inputs.keys():
        isParallel = True
        nPE = inputs['nPE'].value
    if result_parameter.ctype == 'int32_t':
        a = inputs['srcA'].value.astype(np.int32)
        b = inputs['srcB'].value.astype(np.int32)
        result = np.zeros(1, dtype=np.int32)
        if fix_point is None or fix_point == 0:
            result[0] = np.dot(a, b)
        else:
            ctype = inputs['srcA'].ctype
            # plp_dot_prod_q16_parallel
            if ctype == 'int16_t' and isParallel:
                result[0] = q_dotprod16_parallel(
                    a, b, nPE, len(a), fix_point, use_loopunroll=True)
            else:
                # group values and only regularize after grouping
                groups = 2 if ctype == 'int32_t' else 4 if ctype == 'int16_t' else 8
                for g in range(len(a) // groups):
                    tmp_val = 0
                    for i in range(groups):
                        j = g * groups + i
                        tmp_val = q_add(tmp_val, a[j] * b[j])
                    result[0] = q_add(
                        result[0], q_roundnorm(tmp_val, fix_point))
                # do the remaining elements one by one
                for i in range((len(a) // groups) * groups, len(a)):
                    result[0] = q_add(
                        result[0], q_roundnorm(a[i] * b[i], fix_point))

    elif result_parameter.ctype == 'float':
        # for float implementation, it is important to always use float32 for intermediate operations!
        a = inputs['srcA'].value.astype(np.float32)
        b = inputs['srcB'].value.astype(np.float32)
        res = np.float32(0)
        for x_a, x_b in zip(a, b):
            res += x_a * x_b
        result = np.array([res], dtype=np.float32)
    else:
        raise RuntimeError("Unrecognized result type: %s" %
                           result_parameter.ctype)

    return result


######################
# Fixpoint Functions #
######################


def q_dotprod16_parallel(a_arr, b_arr, nPE, blkSize, decipoint, use_loopunroll=False):

    blkSizePE = blkSize // nPE

    # Deploy dot product on N cores
    res = 0
    for i in range(nPE):
        res = q_add(res, q_dotprod16_core(
            a_arr, b_arr, i, blkSizePE, decipoint, use_loopunroll))

    # Compute remaining blocks
    for i in range(blkSizePE * nPE, blkSize):
        res = q_add(res, q_roundnorm(q_mul(a_arr[i], b_arr[i]), decipoint))

    return res


def q_dotprod16_core(a_arr, b_arr, core_id, blkSizePE, decipoint, use_loopunroll=False):

    blkOffset = core_id * blkSizePE
    sumPE = 0

    if use_loopunroll:
        blkCnt = blkSizePE >> 2
        for j in range(blkCnt):
            a0 = a_arr[blkOffset + j * 4: blkOffset + j * 4 + 2]
            b0 = b_arr[blkOffset + j * 4: blkOffset + j * 4 + 2]
            a1 = a_arr[blkOffset + j * 4 + 2: blkOffset + j * 4 + 4]
            b1 = b_arr[blkOffset + j * 4 + 2: blkOffset + j * 4 + 4]
            x0 = q_dotp2(a0, b0)
            x1 = q_dotp2(a1, b1)
            sumPE = q_add(sumPE, q_addroundnorm(x0, x1, decipoint))

        blkRem = blkSizePE % 4
        blkOffset += blkCnt * 4
    else:
        blkRem = blkSizePE

    for j in range(blkRem):
        sumPE = q_add(sumPE, q_roundnorm(
            q_mul(a_arr[blkOffset + j], b_arr[blkOffset + j]), decipoint))

    return sumPE


def q_sat(x):
    if x > 2**31 - 1:
        return x - 2**32
    elif x < -2**31:
        return x + 2**32
    else:
        return x


def q_add(a, b):
    return q_sat(a + b)


def q_sub(a, b):
    return q_sat(a - b)


def q_mul(a, b):
    return q_sat(a * b)


def q_roundnorm(a, p):
    rounding = 1 << (p - 1)
    return q_sat((a + rounding) >> p)


def q_addroundnorm(a, b, p):
    rounding = 1 << (p - 1)
    tmp = q_add(a, b)
    return q_sat((tmp + rounding) >> p)


def q_dotp2(a, b):
    x0 = q_mul(a[0], b[0])
    y0 = q_mul(a[1], b[1])
    return q_add(x0, y0)
