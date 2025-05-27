# Include the userspace code.

INTROSPECTION_KEYMAP_C += hbmorrison.c

# Enabled features.

SEND_STRING_ENABLE = yes
CAPS_WORD_ENABLE = yes
MOUSEKEY_ENABLE = yes

# Optional features.

ifeq ($(strip $(HBM_HANDED)), yes)
  OPT_DEFS += -DHBM_HANDED
endif
