# Flexkey Layout

## Introduction

This is a QMK layout designed with small split and unibody split keyboards in
mind, based on Colemak-DH and a core set of layers for symbols, numbers and
navigation keys situated in a 3x3 grid. The layout is designed to work with as
little as 18 keys with 2 thumb keys and can be expanded from there.

## Usage

This git repository should be copied or cloned into the `users/flexkey/` folder
in the QMK firmware, or it can be added as a submodule. The `install_keymaps.sh`
script will install exemplar keymaps into several supported keyboards, which can
then be compiled.

The keymaps comprise a `rules.mk` file and a small `keymap.c` file that is
responsible for adding layer mappings. These can be copied into other keyboard
`keymap/` subfolders and the exact configuration of the keymap can be adjusted
to suit by editing variables in the `rules.mk` file.

## Core Functionality

![Base Layer](https://i.imgur.com/o7hzWQV.png)

The base layer is made up of the middle three columns of a Colemak-DH layout.
Most of the keys also operate as modifiers or layer keys if held down. In
addition, the QMK Caps Word feature can be toggled by tapping the two shift
modifier keys at the same time.

![Default Combo Keys](https://i.imgur.com/M9D6BeC.png)

There are a number of ways to access the rest of the Colemak-DH keys. The first
way is to use the middle `S` and `E` keys to access the extended layers.

![Left Extended Layer](https://i.imgur.com/a6SBT3U.png)

![Right Extended Layer](https://i.imgur.com/k1ppSYW.png)

The `S` key makes the outer columns on the right side of the keyboard available
and the `E` key makes the outer columns on the left side of the keyboard
available.

The modifier keys are organised vertically on the outside columns so that they
can be pressed along with the extended layer keys. The extended layers also
define the corresponding modifier keys in the same places, so you can be relaxed
about whether you hold down modifiers before or after the layer key.

The second way is to use combos.

![Combo Keys](https://i.imgur.com/1hmz6D1.png)

Combos make the outer keys available by pressing two adjacent keys at the same
time. For example, `K` is accessed by pressing the `H` and `,` keys together,
and `/` is accessed by pressing the `,` and `.` keys together.

The final way is to use a larger keyboard and expand the keymap to use them by
setting variables in the `rules.mk` file.

![Expanded Keys](https://i.imgur.com/wZwN2c9.png)

The following variables are available:

- **FK_TOP_OUTER_KEY = yes** enables the `Q` and `Backspace` keys
- **FK_MIDDLE_OUTER_KEY = yes** enables the `A` and `O` keys
- **FK_BOTTOM_OUTER_KEY = yes** enables the `Z` and `/` keys
- **FK_INNER_KEYS = top** enables the `B`, `J`, `G` and `M` keys
- **FK_INNER_KEYS = bottom** enables the `G`, `M`, `V` and `K` keys
- **FK_INNER_KEYS = all** enables all of the `B`, `J`, `G`, `M`, `V` and `K`
  keys

As physical keys become available the combos for those keys are disabled unless
the following variable is defined:

- **FK_COMBOS_ON = yes** enables all combos regardless of which physical keys
  are defined

Finally the number of thumb keys per side can be increased:

- **FK_TWO_THUMBKEYS = yes** enables two thumb keys per side of the keyboard,
  adding `Shift` and `Backspace` thumb keys alongside `Space` and `Enter`.

## Other Layers

### Symbol Layers

Both symbol layers put the symbols associated with the shifted number keys on
the top row in their correct finger positions,  as much as possible.

 ![Left Symbol Layer](https://i.imgur.com/PKImXjQ.png)

The left symbol layer gathers the pairs of symbols that would normally appear on
the right side of a standard UK ISO keyboard.

![Right Symbol Layer](https://i.imgur.com/sZ8cSr1.png)

The right symbol layer gathers the brackets, braces and parentheses. The
backslash and pipe symbols appear on the left, echoing where that key appears on
a UK ISO keyboard.

### Numbers and Navigation Layers

![Navigation Layer](https://i.imgur.com/wrTQpvZ.png)

The navigation layer gathers navigation related keys together. The right side of
the layer has the arrow keys and related keys, along with some useful navigation
macros. The left side has the three non-alphanumeric keys found on the left of a
standard keyboard - `Esc`, `Tab` and `Caps Lock` - as well as modified `Tab`
keys for convenience.

The `Left` and `Right` arrow keys can be held down to activate `Shift` and
`Control` modifiers.

![Number Layer](https://i.imgur.com/BtJbMHw.png)

The number layer arranges the number keys in keypad format on the left side (I'm
left-handed). Some useful keys are replicated on the right side of the layers:
`Del` and `Backspace` from the left extended layer, `,` and `.` from the base
layer, and `/` for typing dates.

### Controls and Function Layers

The controls and function layers gather together media keys and function keys
respectively.

![Controls Layer](https://i.imgur.com/l5A8Icp.png)

![Function Layer](https://i.imgur.com/1pMgyaz.png)

The function layer has keys for the left and right mouse buttons and the
controls layer has keys for scroll up and scroll down, which can be used with a
trackball operated by your thumb.

### Shortcut Layer

Finally the shortcut layer provides a number of macros that work with
applications on Windows and ChromeOS.

![Shortcut Layer](https://i.imgur.com/advFl4r.png)

The Windows app keys rely on an AutoHotkey script (included in this repo)
whereas, on ChromeOS applications are launched based on their postion on the
shelf - you can think of the six app keys as "launch shelf app 1" and so on.

An `Emoji` key will launch the emoji picker window on both operating systems.

By default the keyboard will start in Windows mode, but the mode can be changed
at any time with the `Win` and `CROS` keys.


