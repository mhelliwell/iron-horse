




# set up working object directory
IH_LOCAL_OBJ_PATH := $(IH_OBJ_WORKING_PATH)$(LOCAL_MODULE_NAME)/shared/


# find all library and dependent modules
IH_LOCAL_ALL_LIB_DEPS := $(LOCAL_STATIC_LIBRARIES) $(LOCAL_SHARED_LIBRARIES)
IH_LOCAL_ALL_DEPS := $(IH_LOCAL_ALL_LIB_DEPS) $(LOCAL_CONFIG_MODULES)


# set IH_TARGET_PREFIX
IH_TARGET_PREFIX := $(LOCAL_MODULE_TYPE)_module_$(LOCAL_MODULE_NAME)


# set the canonical name of this module
IH_CANONICAL_NAME := ih_canonical_name_$(LOCAL_MODULE_NAME)
.PHONEY: $(IH_CANONICAL_NAME)


# set IH_MODULE_TARGETS to a unique symbol
$(eval $(call gen_sym, IH_MODULE_TARGETS, $(IH_TARGET_PREFIX)))



$(IH_CANONICAL_NAME): $(addprefix ih_canonical_name_, $(IH_LOCAL_ALL_DEPS))
$(IH_CANONICAL_NAME): $(IH_MODULE_TARGETS)



# print message
$(eval $(call gen_sym, IH_TEMP, $(IH_TARGET_PREFIX)_message))
$(eval $(call ih_rule_template_single_action,$(IH_TEMP),,\
	@echo building $(LOCAL_MODULE_TYPE) module $(LOCAL_MODULE_NAME) ))
$(IH_MODULE_TARGETS): $(IH_TEMP)



# create object directory
$(eval $(call gen_sym, IH_TEMP, $(IH_TARGET_PREFIX)_create_object_dir))
$(eval $(call ih_rule_template_single_action,$(IH_TEMP),,\
	@mkdir -p $(IH_LOCAL_OBJ_PATH) ))
$(IH_MODULE_TARGETS): $(IH_TEMP)



# set up global export targets
IH_EXP_CCFLAGS := EXPORT_CCFLAGS_$(LOCAL_MODULE_NAME)
$(IH_EXP_CCFLAGS) := $(LOCAL_EXPORTED_CCFLAGS)
IH_EXP_LDFLAGS := EXPORT_LDFLAGS_$(LOCAL_MODULE_NAME)
$(IH_EXP_LDFLAGS) := $(LOCAL_EXPORTED_LDFLAGS)



# setup local cc flags
IH_LOCAL_CCFLAGS := $(LOCAL_CCFLAGS)
ifeq ($(LOCAL_USE_CPP17), true)
IH_LOCAL_CCFLAGS += -std=c++17
endif
define build_ccflags
IH_LOCAL_CCFLAGS += $$$$(EXPORT_CCFLAGS_$(strip $(1)))
endef
$(foreach x,$(IH_LOCAL_ALL_DEPS),$(eval $(call build_ccflags, $(x))))



# setup local ld flags
IH_LOCAL_LDFLAGS := $(LOCAL_LDFLAGS)
ifneq ($(strip $(IH_LOCAL_ALL_LIB_DEPS)),)
IH_LOCAL_LDFLAGS += -L$(IH_LIB_RESULTS_PATH)
endif
define build_ldflags
IH_LOCAL_LDFLAGS += $$$$(EXPORT_LDFLAGS_$(strip $(1)))
endef
$(foreach x,$(IH_LOCAL_ALL_DEPS),$(eval $(call build_ldflags, $(x))))



# create the build objects target
$(eval $(call gen_sym, IH_OBJ_TARGET, $(IH_TARGET_PREFIX)_build_objects))


#
# set the make rules
#
# clear the IH_LOCAL_ALL_OBJS variable
IH_LOCAL_ALL_OBJS :=

# a function to set rules for turning .cc files into .o files
define make-obj-targets
$(IH_LOCAL_OBJ_PATH)$(1:.cc=.o) : $(LOCAL_PATH)$(strip $(1))
	@g++ $(IH_LOCAL_CCFLAGS) -fPIC -c $(LOCAL_PATH)$(strip $(1)) -o $(IH_LOCAL_OBJ_PATH)$(1:.cc=.o) 
IH_LOCAL_ALL_OBJS += $(IH_LOCAL_OBJ_PATH)$(1:.cc=.o)
endef

# set the rule for each source file
$(foreach x,$(LOCAL_CC_SOURCE_FILES), $(eval $(call make-obj-targets, $(x))))


$(IH_OBJ_TARGET): $(IH_LOCAL_ALL_OBJS)



# set the link rule
define make-shared-target
$(1): $(IH_OBJ_TARGET)
	@g++ $(IH_LOCAL_ALL_OBJS) $(IH_LOCAL_LDFLAGS) -shared -o $(IH_LIB_RESULTS_PATH)lib$(LOCAL_MODULE_NAME).so
endef
$(eval $(call gen_sym, IH_LINK_TARGET, $(IH_TARGET_PREFIX)_link))
$(eval $(call make-shared-target, $(IH_LINK_TARGET)))



$(IH_MODULE_TARGETS): $(IH_LINK_TARGET)



BUILD_TARGETS: $(IH_CANONICAL_NAME)

