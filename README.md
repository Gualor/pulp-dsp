## PULP DSP: Digital Signal Processing on Parallel Ultra Low Power Platform

# Introduction

This repository contains the work in progress of an optimized DSP library for PULP platforms.

It contains the kernel functions for different ISA extensions of RISC-V being developed for PULP platforms (RV32IM, XPULPV2, etc.).

Currently it's being developed and tested on Mr.Wolf (fabric controller (FC) with IBEX (previous zero-riscy), i.e. RV32IM, and cluster with 8 RISCY cores, i.e. RV32IM with XPULPV2 extentions.).

# List of ISA extensions on PULP

[Here](https://docs.google.com/spreadsheets/d/13hU5qtpxtcejE1NCyHPy07BwGrqVjrbf9GrBuDpx4Xc/edit?usp=sharing) is a list of PULP chips with respective instructions set extensions: [needs verification and update]

# Structure of the repository

This repository contains:

- `src` folder contains the source codes. In the folder you can find subfolders with different kinds of functions (e.g. BasicMathFunctions, StatisticsFunctions, etc.). In each subfolder you find the glue codes and a folder called `kernel` which contains the kernels for different ISA extensions. The glue codes are used for namely "gluing" the kernels, for example for checking the ISA extension of the calling platform (in the case of Mr. Wolf, it checks if it's FC or CLUSTER calling the function, in case of FC the kernel function for RV32IM is called, otherwise the kernel function for XPULPV2 extension is called.
 
- `include` folder with necessary header files. Especially the main header file `plp_math.h` has to be included in the codes which want to use this library.

[Note: in the same header file it's possible to define macros (e.g. LOOPUNROLL if you want to take into consideration the option of unrolling or not unrolling the loops).]

- `Makefile` for compiling the library. Add your glue codes and kernel functions to be compiled. Then do `make clean header all install` and the library will be compiled and installed in your pulp-sdk. To use the library add `PULP_LDFLAGS += -lplpdsp -lm` in the Makefile of your project (for example when you test the functions in the `test` folder). If you add or modify the source codes and want to rebuild the library, do `make header build install`.

- `lib` folder contains the build/ folder when building the library and the static library can be found in lib/build/wolfe/.

- `test` folder contains the testing setup used during the development of the library. For more details please read the README file in the folder.

- `doc` folder contains the configurations for building documentation for the library. Go to the folder and type `doxygen doc_config`, it creates the documentation and you can browse it by opening `html/index.html` using a browser. To add documentations use @defgroup, @ingroup, @addtogroup, etc. Please refer to plp_math.h and the source codes src/BasicMathFunctions/plp_dot_prod_i32.c and src/BasicMathFunctions/kernels/plp_dot_prod_i32s_rv32im.c as examples.

# Installation and usage

First of all, in order to install and use the library, you have to install the pulp-sdk. You can follow the instructions [here](https://github.com/pulp-platform/pulp-sdk). Configure the sdk according to the chip and the platform you want to use (for example `source configs/wolfe.sh` and `source configs/platform-board.sh`). Do not forget to source the `sourceme.sh` everytime you open a new terminal to set up the environmental variables (e.g. PULP_SDK_HOME) needed to run pulp projects.

Once you are done with the pulp-sdk setup, you can clone this repository, enter the PULP-DSP folder, and do `make clean header all install`.

After installing the library, you can call all the functions of the library you want to use. Don't forget to include the header file `plp_math.h` in your codes :)

## License and Attribution
All source code is released under Apache v2.0 license unless noted otherwise, please refer to the LICENSE file for details.

We are inspired by CMSIS-DSP and partially adapted its structure.


