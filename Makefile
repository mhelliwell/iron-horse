##############################################################################
#                                                                            #
#    III  RRRRR    OOO   N     N      H    H   OOO   RRRRR    SSSS  EEEEE    #
#     I   R    R  O   O  NN    N      H    H  O   O  R    R  S      E        #
#     I   R    R  O   O  N N   N      H    H  O   O  R    R  S      E        #
#     I   RRRRR   O   O  N  N  N      HHHHHH  O   O  RRRRR    SSS   EEEE     #
#     I   R   R   O   O  N   N N      H    H  O   O  R   R       S  E        #
#     I   R    R  O   O  N    NN      H    H  O   O  R    R      S  E        #
#    III  R    R   OOO   N     N      H    H   OOO   R    R  SSSS   EEEEE    #
#                                                                            #
##############################################################################


# Set the path to the root of the source tree
IH_BASE_PATH := $(shell pwd)/


include build/basic_defs.mk


build: PREBUILD_TARGETS BUILD_TARGETS POSTBUILD_TARGETS
.PHONEY: build



include $(IH_BASE_PATH)source/ih.mk



PREBUILD_TARGETS:
BUILD_TARGETS:
POSTBUILD_TARGETS:
.PHONEY: PREBUILD_TARGETS BUILD_TARGETS POSTBUILD_TARGETS



create_dirs:
	@mkdir -p $(IH_BIN_RESULTS_PATH)
	@mkdir -p $(IH_LIB_RESULTS_PATH)
	@mkdir -p $(IH_OBJ_WORKING_PATH)
.PHONEY: create_dirs
PREBUILD_TARGETS: create_dirs



.PHONEY: all clean clobber rebuild rebuild_message


clean:
	@echo clean the project
	@rm -rf results/obj


clobber:
	@echo clobbering the project
	@rm -f hsc
	@rm -f *~
	@rm -rf results


rebuild_message:
	@echo rebuilding...


rebuild: clobber rebuild_message build

