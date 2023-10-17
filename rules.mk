SRC += hbmorrison.c

SEND_STRING_ENABLE = yes
CAPS_WORD_ENABLE = yes

ifeq ($(strip $(HBM_SIDEKEY_ENABLE)), yes)
  OPT_DEFS += -DHBM_SIDEKEY_ENABLE
endif

ifeq ($(strip $(HBM_HOMEROW_ENABLE)), yes)
  OPT_DEFS += -DHBM_HOMEROW_ENABLE
endif
