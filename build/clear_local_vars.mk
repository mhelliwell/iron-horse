

#########################################
#  This file clears all local variable  #
#  used by  local.mk makefiles.  Since  #
#  they are all listed here, this is a  #
#  good place to document them.         #
#########################################


# This is the name of the module. The
# resulting artifacts will also bare
# this name.
#
#  module type           artifact name
#  -----------           -------------
#  executable            <module name>
#  shared object         lib<module name>.so
#  static library        lib<module name>.a
#  config module         nil
#
LOCAL_MODULE_NAME :=


# This is the module type.  It needs to
# one of the following:
#
#  for result type...      set modulte type...         
#  ------------------      -------------------
#     executable                 EXEC
#   shared object               SHARED
#   static library              STATIC
#   config module               CONFIG
#
LOCAL_MODULE_TYPE :=


# The source files for this module are listed in
# one of these variables. All source files must
# be written in ether C or C++, but not both.
# Only one of these variables should be set.
#
#    C files are listed in LOCAL_C_SOURCE_FILES
#  C++ files are listed in LOCAL_C_SOURCE_FILES
# 
LOCAL_C_SOURCE_FILES :=
LOCAL_CC_SOURCE_FILES :=


# These variables are used to set compiler flags.
# If this is a C language module use LOCAL_CFLAGS,
# if this is a C++ modlue use LOCAL_CCFLAGS. Only
# one of these two variables should be set.
# LOCAL_LDFLAGS sets linker flags for both C and
# C++ modules.
#
LOCAL_CFLAGS :=
LOCAL_CCFLAGS :=
LOCAL_LDFLAGS :=


# Library modules should use these variables to
# export compiler flags to other modules that
# depend on this module. The flags listed in
# these variables are included on the command
# line when modules that depend one this library
# are compiled.
#
# Example: A library named "foo" might export
#          these flags that are required for
#          another module to it.
#
#   LOCAL_EXPORTED_CFLAGS := -I$(LOCAL_PATH)/inc
#   LOCAL_EXPORTED_LDFLAGS := -lfoo
#
LOCAL_EXPORTED_CFLAGS :=
LOCAL_EXPORTED_CCFLAGS :=
LOCAL_EXPORTED_LDFLAGS :=


# Set these variables to any libraries that this
# module depends on.
#
LOCAL_STATIC_LIBRARIES :=
LOCAL_SHARED_LIBRARIES :=
LOCAL_CONFIG_MODULES :=


# Set this to 'true' to include any required
# flags to compile c++17
LOCAL_USE_CPP17 :=

