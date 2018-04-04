#!/usr/bin/env bash

#set -x

if [[ $# != 2 ]]; then
    echo "Missing search path!"
    echo "Example:
    bash appimage.include.qml.modules.sh /path/to/your/application/qml/files/base/dir /path/to/appdir"
    exit 1
fi

which qmlimportscanner || { echo "ERROR: qmlimportscanner not found!" && exit 1; }

if [[ -z "$QMLIMPORTSCANNER_BIN" ]]; then
    QMLIMPORTSCANNER_BIN=`which qmlimportscanner`
fi

echo "## Looking for qml imports in $1"
modules=$($QMLIMPORTSCANNER_BIN $1 | grep -oE '"name":.*"(.*)"' | cut -d" " -f 2 | sort | uniq)
modules="${modules//\"/}"
if [[ -z $modules ]]; then
    echo "No imports found in $1"
    exit 0;
else
    echo "$modules";
    echo "";
fi

IFS=";"
for MODULE_DIR in "$QML_MODULES_FIND_DIRS;/usr/lib;/opt"; do
    qmldirs=$(find $MODULE_DIR -iname qmldir)
done
unset IFS

echo "## Matching qmldirs with imports"
dirs_to_include=""
for qmldir in $qmldirs; do
    for module in $modules; do
        if grep -q "^module $module\$" "$qmldir"; then
            dirs_to_include="$dirs_to_include `dirname ${qmldir}`"
            echo "Match $module in $qmldir"
        fi
    done
done

for dir_to_include in $dirs_to_include; do
    MODULE=$(cat $dir_to_include/qmldir | grep module | cut -f 2 -d\ )
    cp -r $dir_to_include/* $2/usr/qml/${MODULE//\./\/}/
done