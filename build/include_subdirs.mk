

IH_ALL_DIR_MAKEFILES := $(shell find $(LOCAL_PATH) -maxdepth 2 -mindepth 2 -name "ih.mk")

include $(IH_ALL_DIR_MAKEFILES)

