include $(IH_START)


LOCAL_MODULE_NAME := hsc
LOCAL_MODULE_TYPE := exec

LOCAL_CC_SOURCE_FILES := hsc.cc lex.cc

LOCAL_USE_CPP17 := true

LOCAL_CONFIG_MODULES := includes
LOCAL_SHARED_LIBRARIES := statemachine tokenizer

include $(IH_BUILD_EXEC)

