# Include the userspace code.

INTROSPECTION_KEYMAP_C += hbmorrison.c

# Enabled features.

SEND_STRING_ENABLE = yes
CAPS_WORD_ENABLE = yes
MOUSEKEY_ENABLE = yes

# Parse optional features.

ifeq ($(strip $(ENABLE_ONESHOT)), yes)
  OPT_DEFS += -DENABLE_ONESHOT
endif
