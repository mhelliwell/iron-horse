include $(IH_START)


LOCAL_MODULE_NAME := statemachine
LOCAL_MODULE_TYPE := shared

LOCAL_CC_SOURCE_FILES := state_machine.cc

LOCAL_USE_CPP17 := true


LOCAL_EXPORTED_CCFLAGS := -I$(LOCAL_PATH)
LOCAL_EXPORTED_LDFLAGS := -lstatemachine

LOCAL_CONFIG_MODULES := includes



include $(IH_BUILD_SHARED)
