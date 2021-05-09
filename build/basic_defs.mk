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


#####################################
#  Notes on Formatting:             #
#                                   #
#  a PATH always ends with a "/"    #
#  a DIR does not end with a "/"    #
#                                   #
#####################################



#####################################################
#  Some directories which are commonly referenced   #
#####################################################
IH_BUILD_PATH := $(IH_BASE_PATH)build/
IH_RESULTS_PATH := $(IH_BASE_PATH)results/


IH_BIN_RESULTS_PATH := $(IH_RESULTS_PATH)bin/
IH_LIB_RESULTS_PATH := $(IH_RESULTS_PATH)lib/
IH_OBJ_WORKING_PATH := $(IH_RESULTS_PATH)obj/


#####################################################
#      Some commonly used make include files        #
#####################################################
IH_START := $(IH_BUILD_PATH)start.mk
IH_NEXT := $(IH_BUILD_PATH)clear_local_vars.mk
IH_BUILD_EXEC := $(IH_BUILD_PATH)build_exec.mk
IH_BUILD_SHARED := $(IH_BUILD_PATH)build_shared.mk
IH_BUILD_STATIC := $(IH_BUILD_PATH)build_static.mk
IH_BUILD_CONFIG := $(IH_BUILD_PATH)build_config.mk

IH_ALL_SUBDIRS := $(IH_BUILD_PATH)include_subdirs.mk

IH_CLEAR_VARS := $(IH_BUILD_PATH)clear_local_vars.mk

IH_GENERATE_SYMBOL := $(IH_BUILD_PATH)gensym.mk


#####################################################
#            Some useful odds and ends              #
#####################################################

# IH_BASE_PATH_SIZE is the number of directories
# in the path to IH's root directory. This is
# useful, for example, when converting an absolute
# path to a path relative to the IH root directory.
IH_BASE_PATH_SIZE := $(words $(strip $(subst /, ,$(IH_BASE_PATH))))


# IH_SPACE expands to a space. This is required to
# replace spaces with another character with the subst
# function (and maybe other places as well).
TEMP_NOTHING =
IH_SPACE := $(TEMP_NOTHING) $(TEMP_NOTHING)


# Someday you might need to escape a comma so I give you IH_COMMA.
# Now, for example, you can replace all commas in a string with
# semicolons like this:
#         $(subst $(IH_COMMA),;,$(string))
IH_COMMA := ,


# Same as above for a quote
IH_QUOTE := "
# " <- this quote is here to close the quote above and make editors happy




############################
#  further initialization  #
############################

include $(IH_BUILD_PATH)functions.mk


$(eval $(call gen_sym_init))

