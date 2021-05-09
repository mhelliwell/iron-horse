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


IH_TARGET_PREFIX := $(LOCAL_MODULE_TYPE)_module_$(LOCAL_MODULE_NAME)
IH_CANONICAL_NAME := ih_canonical_name_$(LOCAL_MODULE_NAME)
.PHONEY: $(IH_CANONICAL_NAME)
# set IH_MODULE_TARGETS to a unique symbol
$(eval $(call gen_sym, IH_MODULE_TARGETS, $(IH_TARGET_PREFIX)))



# print message
$(eval $(call gen_sym, IH_TEMP, $(IH_TARGET_PREFIX)_message))
$(eval $(call ih_rule_template_single_action,$(IH_TEMP),,\
	@echo building $(LOCAL_MODULE_TYPE) module $(LOCAL_MODULE_NAME) ))
$(IH_MODULE_TARGETS): $(IH_TEMP)



# set up global export targets
IH_EXP_CCFLAGS := EXPORT_CCFLAGS_$(LOCAL_MODULE_NAME)
$(IH_EXP_CCFLAGS) := $(LOCAL_EXPORTED_CCFLAGS)
IH_EXP_LDFLAGS := EXPORT_LDFLAGS_$(LOCAL_MODULE_NAME)
$(IH_EXP_LDFLAGS) := $(LOCAL_EXPORTED_LDFLAGS)


$(IH_CANONICAL_NAME): $(IH_MODULE_TARGETS)

BUILD_TARGETS: $(IH_CANONICAL_NAME)

