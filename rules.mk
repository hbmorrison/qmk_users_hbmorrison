# SRC += hbmorrison.c

SEND_STRING_ENABLE = yes
CAPS_WORD_ENABLE = yes

# Add defines for optional features.

ifeq ($(strip $(HBM_SIDEKEY_ENABLE)), yes)
  OPT_DEFS += -DHBM_SIDEKEY_ENABLE
endif

ifeq ($(strip $(HBM_THUMBKEY_ENABLE)), yes)
  OPT_DEFS += -DHBM_THUMBKEY_ENABLE
endif

ifeq ($(strip $(HBM_HOMEROW_ENABLE)), yes)
  OPT_DEFS += -DHBM_HOMEROW_ENABLE
endif

ifeq ($(strip $(HBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE)), yes)
  OPT_DEFS += -DHBM_TD_HOLD_ORIGINAL_KEYPRESS_ENABLE
  TAP_DANCE_ENABLE = yes
endif

# Reduce the size of the compiled firmware.

LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
EXTRAKEY_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no

