
IH_TEMP := $(shell echo $(words $(MAKEFILE_LIST))-1 | bc)
LOCAL_PATH := $(dir $(word $(IH_TEMP), $(MAKEFILE_LIST)))

include $(IH_CLEAR_VARS)


