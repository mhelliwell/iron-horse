

define ih_rule_template_no_action
$(1): $(2)
endef


define ih_rule_template_single_action
$(1): $(2)
	@$(3)
endef


define ih_rule_template_double_action
$(1): $(2)
	@$(3)
	@$(4)
endef


define ih_rule_template_triple_action
$(1): $(2)
	@$(3)
	@$(4)
	@$(5)
endef


define rule_template
$(info rule_template $(1)-$(2)-$(3))
$(1): $(2)
	@$(3)
	@$(4)
	@$(5)
	@$(6)
	@$(7)
	@$(8)
	@$(9)
endef


#
# use this to strip the spaces off the ends of a variable
#  example: $(eval $(call strip_var LOCAL_MODULE_NAME))
#
define strip_var
$(strip $(1)) := $(strip $($(strip $(1))))
endef


################################################################################
#                                                                              #
#  This is a unique symbol generating system. There are                        #
#  three functions defined:                                                    #
#    gen_sym_init  - initialize the system (this is called in basic_defs)      #
#    gen_sym       - generates a unique symbol, takes two arguments:           #
#                     - the name of the variable to store the unique symbol in #
#                     - a string to append to the unique symbol                #
#                  - example: $(eval $(call gen_sym, MY_SYMBOL_NAME, test))    #
#                             This generates a unique symbol, appends the      #
#                             string "test" to it, and saves the string in     #
#                             the variable "MY_SYMBOL_NAME". Since these       #
#                             symbols are often used as make rule targets, the #
#                             generated symbol is also declared to be phoney.  #
#    gen_sym_no_phoney - the same as gen_sym except the symbol is not declared #
#                        to be phoney                                          #
#                                                                              #
################################################################################
define gen_sym_init
IH_GENSYM_PRIVATE := 1
IH_GENSYM := sym0
endef

define gen_sym
IH_GENSYM_PRIVATE := $(shell echo $(IH_GENSYM_PRIVATE)+1 | bc)
IH_GENSYM := sym$(IH_GENSYM_PRIVATE)
$(1) := $(IH_GENSYM)_$(strip $(2))
.PHONEY: $(IH_GENSYM)_$(strip $(2)))
endef

define gen_sym_no_phoney
IH_GENSYM_PRIVATE := $(shell echo $(IH_GENSYM_PRIVATE)+1 | bc)
IH_GENSYM := sym$(IH_GENSYM_PRIVATE)
$(1) := $(IH_GENSYM)_$(strip $(2))
endef



