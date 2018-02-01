#!/usr/bin/env bash

#set -x

if [[ $# != 2 ]]; then
    echo "Missing search path!"
    echo "Example:
    bash appimage.include.qml.modules.sh /path/to/your/application/qml/files/base/dir /path/to/appdir"
    exit 1
fi


echo "## Looking for qml imports in $1"
modules=$(qmlimportscanner $1 | grep -oE '"name":.*"(.*)"' | cut -d" " -f 2 | sort | uniq)
modules="${modules//\"/}"
if [[ -z $modules ]]; then
    echo "No imports found in $1"
    exit 0;
else
    echo "$modules";
    echo "";
fi


qmldirs=$(find /usr/lib -iname qmldir)
qmldirs="$qmldirs $(find /opt -iname qmldir)"

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
    cp -r $dir_to_include $2/usr/qml/
done