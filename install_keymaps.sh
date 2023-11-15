#!/bin/bash

# Locate the base directory of the repository.

THIS_SCRIPT=$(readlink -f $0)
BASE_DIR=$(dirname $THIS_SCRIPT)
KEYMAP_DIR="${BASE_DIR}/keymaps"

# Locate the QMK firmware directory.

which qmk > /dev/null 2>&1
if [ $? -eq 0 ]
then
  QMK_DIR=$(qmk config user.qmk_home | cut -d= -f2)
fi

# If the qmk_home property is set, assume this repo is in the users/ directory.

if [ "${QMK_DIR}" == "" -o "${QMK_DIR}" == "None" ]
then
  USERS_DIR=$(dirname $BASE_DIR)
  QMK_DIR=$(dirname $USERS_DIR)
fi

KEYBOARDS_DIR="${QMK_DIR}/keyboards"

if [ ! -d "${KEYBOARDS_DIR}" ]
then
  echo "Error: keyboards dir ${KEYBOARDS_DIR} not found - set user.qmk_home property"
  exit 1
fi

for KEYMAP in $(/bin/ls -1 $KEYMAP_DIR)
do
  for KEYBOARD in $(/bin/ls -1 "${KEYMAP_DIR}/${KEYMAP}")
  do
    KEYBOARD_SUBDIR=$(echo $KEYBOARD | sed 's/_/\//g')
      TARGET_DIR="${KEYBOARDS_DIR}/${KEYBOARD_SUBDIR}/keymaps/${KEYMAP}"
      if [ ! -d "${TARGET_DIR}" ]
      then
        echo "Creating ${TARGET_DIR}"
         mkdir -p $TARGET_DIR
      fi
      echo "Copying $KEYMAP keymap into $TARGET_DIR"
      cp -f "${KEYMAP_DIR}/${KEYMAP}/${KEYBOARD}/rules.mk" $TARGET_DIR
      cp -f "${KEYMAP_DIR}/${KEYMAP}/${KEYBOARD}/keymap.c" $TARGET_DIR
  done
done
