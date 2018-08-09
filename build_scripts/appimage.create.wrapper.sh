#!/usr/bin/env bash

APPDIR=$1

if [ -z "${APPDIR}" ]; then
    echo "Missing AppDir path!"
    echo "Example:
        bash appimage.create.wrapper.sh ./AppDir"
    exit 1
fi


echo ""
echo "Creating application wrapper"


DESKTOP_FILE=$(ls ${APPDIR}/usr/share/applications/*.desktop | cut -f1)

if [[ -z ${DESKTOP_FILE} ]]; then
    echo "Unable to find a .desktop in appdir"
    echo ""
    exit 1;
fi

RUNNABLE=`cat ${DESKTOP_FILE} | grep Exec | cut -f2 -d=`

if [[ -z ${RUNNABLE} ]]; then
    echo "Unable to find a runnable in the appimage desktop file."
    echo ""
    exit 1;
fi

echo "Creating wrapper for ${RUNNABLE}"

WRAPPER_NAME="${RUNNABLE}_wrapper"
WRAPPER_PATH=${APPDIR}/usr/bin/${WRAPPER_NAME}

sed -i "s/Exec=${RUNNABLE}/Exec=${WRAPPER_NAME}/g" ${DESKTOP_FILE}

echo "Done"
echo ""