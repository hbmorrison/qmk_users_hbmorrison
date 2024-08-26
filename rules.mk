# Include the userspace code.

SRC += hbmorrison.c

# This is a bit of a hack to allow combos to be defined in userspace.

INTROSPECTION_KEYMAP_C = combos.c

# Enabled features.

SEND_STRING_ENABLE = yes
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = yes
MOUSEKEY_ENABLE = yes
