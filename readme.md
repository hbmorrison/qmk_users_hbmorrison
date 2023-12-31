# Flexkey Layout

## Introduction

This is a QMK layout designed for the Ferris Sweep and OLKB Planck keyboards,
based on Colemak-DH.

## Usage

This git repository should be copied or cloned into the `users/hbmorrison/`
folder in the QMK firmware, or it can be added as a submodule. The
`install_keymaps.sh` script will install exemplar keymaps into several supported
keyboards, which can then be compiled.

The keymaps comprise a `rules.mk` file and a small `keymap.c` file that is
responsible for adding layer mappings. These can be copied into other keyboard
`keymap/` subfolders and the exact configuration of the keymap can be adjusted
to suit by editing variables in the `rules.mk` file.

## Core Functionality

![Base Layer](https://i.imgur.com/o7hzWQV.png)

The base layer implements the Colemak-DH layout. The QMK Caps Word feature can
be toggled by tapping the two shift modifier keys at the same time.

## Symbol Layer

The symbol layer puts the symbols associated with the shifted number keys on the
top row in their correct finger positions,  as much as possible.

 ![Symbol Layer](https://i.imgur.com/PKImXjQ.png)

The pairs of symbols that would normally appear on the right side of a standard
UK ISO keyboard are on the right-hand side. The left-hand side gathers the
brackets, braces and parentheses. The backslash and pipe symbols appear on the
left, echoing where that key appears on a UK ISO keyboard.

## Navigation Layer

![Navigation Layer](https://i.imgur.com/wrTQpvZ.png)

The navigation layer gathers navigation related keys together. The right-hand
side of the layer has the arrow keys and related keys, along with some useful
navigation macros. The left-hand side has useful media controls.

## Number Layer

![Number Layer](https://i.imgur.com/BtJbMHw.png)

The number layer arranges the number keys in keypad format on the left-hand
side. Function keys appear on the right-hand side.

## Shortcut Layer

Finally the shortcut layer provides a number of macros that work with
applications on Windows and ChromeOS.

![Shortcut Layer](https://i.imgur.com/advFl4r.png)

The Windows app keys rely on an AutoHotkey script (included in this repo)
whereas, on ChromeOS applications are launched based on their postion on the
shelf - you can think of the six app keys as "launch shelf app 1" and so on.

By default the keyboard will start in Windows mode, but the mode can be changed
at any time with the `Win` and `CROS` keys.
