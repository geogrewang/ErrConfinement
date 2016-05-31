# ErrConfinement Project
PD_RISC: LISA processor model for PD_RISC processor
PD_RISC/lisa/: LISA files for modified PD_RISC processor with error confinement instructions
PD_RISC/app/:  application files, include sieve, hello world and jpeg
DATE16: Source files for DATE16 paper

Steps:
1. Try to use Processor Designer, create new PD_RISC project, build processor model and its toolchain, including LLVM compiler
2. Replace all lisa files with the ones under PD_RISC/lisa/
3. Rebuild processor model, no need to rebuild LLVM compiler.

There can be building issues depending on the Processor Designer version. The last working one is 2013.06

Please contact Zheng Wang (wangz@ntu.edu.sg) for any issue
