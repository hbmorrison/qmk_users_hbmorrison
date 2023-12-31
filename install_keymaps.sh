#!/bin/bash

# Locate the base directory of the repository.

THIS_SCRIPT=$(readlink -f $0)
BASE_DIR=$(dirname $THIS_SCRIPT)
KEYMAP_NAME=$(basename $BASE_DIR)
KEYBOARDS_DIR="${BASE_DIR}/keyboards"

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

# Check that we can access the QMK keyboards directory.

QMK_KEYBOARDS_DIR="${QMK_DIR}/keyboards"

if [ ! -d "${QMK_KEYBOARDS_DIR}" ]
then
  echo "Error: QMK keyboards dir ${QMK_KEYBOARDS_DIR} not found - set user.qmk_home property"
  exit 1
fi

# Install the production keymaps.

for KEYBOARD in $(/bin/ls -1 "${KEYBOARDS_DIR}")
do
  KEYBOARDS_SUBDIR=$(echo $KEYBOARD | sed 's/_/\//g')
  TARGET_DIR="${QMK_KEYBOARDS_DIR}/${KEYBOARDS_SUBDIR}/keymaps/${KEYMAP_NAME}"
  if [ ! -d "${TARGET_DIR}" ]
  then
    echo "Creating ${TARGET_DIR}"
     mkdir -p $TARGET_DIR
  fi
  echo "Copying ${KEYMAP_NAME} keymap into ${TARGET_DIR}"
  cp -f "${KEYBOARDS_DIR}/${KEYBOARD}/rules.mk" $TARGET_DIR
  cp -f "${KEYBOARDS_DIR}/${KEYBOARD}/keymap.c" $TARGET_DIR
done
