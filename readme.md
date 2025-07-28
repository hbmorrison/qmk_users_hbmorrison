# Hannah's Ferris Sweep Layout

## Introduction

This is a QMK layout designed for the Ferris Sweep based on Colemak-DH with UK
ISO symbols. The layout is designed so that shift, other modifiers and layer
keys will only affect the opposite side of the keyboard.

## Usage

Clone this repository into the `users/hbmorrison/` folder inside the QMK
firmware folder then run the `install_keymaps.sh` script to install the keymap
in the correct place under the QMK `keyboards/` folder.

Compile the firmware by running:

```
qmk compile -kb ferris/sweep -e CONVERT_TO=rp2040_ce
```

## Core Functionality

![Base Layer](assets/base.png)

The base layer is arranged in a Colemak-DH layout, with a small number of
changes.

- The top right pinky key produces `Backspace`.
- The bottom left and right pinky keys give oneshot access to the right and
  left symbol layers.
- The `Z` and `/` keys have been displaced into symbol layers.

In addition:

- The thumb keys produce, from left to right: `Left Shift`, `Space`, `Enter`,
  and `Right Shift`.
- When held, the `Space` and `Enter` thumb keys give access to the navigation
  and number layers.
- Double-tapping either of the shift keys toggles the QMK Caps Word feature.
- Holding `Left Shift` and tapping `Backspace` produces `Delete`.
- Holding the `F` key gives access to function keys in the `Function` layer.
- Holding the `U` key gives access to controls for brightness, sound and media
  in the `Controls` layer.

## Symbol Layers

The symbols associated with the shifted number keys on the top row of both
symbol layers, close to their UK ISO finger positions.

![Right Symbol Layer](assets/rsym.png)

The pairs of unshifted and shifted symbols that would normally appear clustered
on the right side of a UK ISO keyboard are on the right symbol layer.

Note that the `/` key is available on the bottom right pinky key in the right
symbol layer.

![Left Symbol Layer](assets/lsym.png)

The left symbol layer gathers the brackets, braces and parentheses. The
backslash and pipe symbols appear on the left, echoing where that key appears on
a UK ISO keyboard.

Note that the `Z` key is available on the bottom left pinky key in the left
symbol layer.

## Navigation Layer

The navigation layer arranges navigation related keys together on the right
side, including the arrow keys, the `Home`, `Page Down`, `Page Up` and `End`
keys as well as a number of shortcut keys for desktop actions.

![Navigation Layer](assets/nav.png)

The `Esc` and `Esc + Colon` keys are on the right thumb keys. Left and
right mouse buttons are available on the left side, for use with a
centralised trackball or trackpad.


## Number Layer

The number layer arranges the number keys on the left side in keypad format,
with `1` at the top left, along with `.` and `/` keys for convenience.

![Number Layer](assets/num.png)

The `Tab` and `Shift-Tab` keys are on the left thumb keys. Left and
right mouse buttons are also available on the right side, for use with a
centralised trackball or trackpad.

## Function Key Layer

Holding the `F` key makes function keys available on the right side.

![Function Key Layer](assets/function.png)

## Controls Layer

Holding the `U` key makes media controls available on the left side.

![Controls Layer](assets/controls.png)

The keys on the inner edge of the left side switch the OS-specific functions
(such as switching virtual desktop) between Windows, ChromeOS and Linux.
