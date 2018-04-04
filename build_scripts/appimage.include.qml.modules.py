#!/usr/bin/python3
import os
import sys
import json
import shutil
from subprocess import Popen, PIPE


def copytree(src, dst, symlinks=False):
    names = os.listdir(src)
    os.makedirs(dst, exist_ok=True)
    errors = []
    for name in names:
        srcname = os.path.join(src, name)
        dstname = os.path.join(dst, name)
        try:
            if symlinks and os.path.islink(srcname):
                linkto = os.readlink(srcname)
                os.symlink(linkto, dstname)
            elif os.path.isdir(srcname):
                copytree(srcname, dstname, symlinks)
            else:
                shutil.copy2(srcname, dstname)
            # XXX What about devices, sockets etc.?
        except OSError as why:
            errors.append((srcname, dstname, str(why)))
        # catch the Error from the recursive copytree so that we can
        # continue with other files
        except shutil.Error as err:
            errors.extend(err.args[0])
    try:
        shutil.copystat(src, dst)
    except OSError as why:
        # can't copy file access times on Windows
        if why.winerror is None:
            errors.extend((src, dst, str(why)))
    if errors:
        raise shutil.Error(errors)


def main(arguments):
    if len(sys.argv) < 3:
        print(
            "Usage:\n\tpython appimage.include.qmlmodules.py /sourcePath/to/your/app/qml/source/dir "
            "/sourcePath/to/the/AppDir [/paths/to/qml/modules/folders ...]\n")
        exit(1)

    qml_source_dir = sys.argv[1]

    app_dir = sys.argv[2]

    qml_module_dirs = ["/usr/lib"]
    qml_module_dirs += sys.argv[3:]

    qmlimportscanner_bin = shutil.which("qmlimportscanner")

    print("QML Modules Search paths: ", qml_module_dirs)

    qml_imports = []
    for qmlModulesDir in qml_module_dirs:
        process = Popen([qmlimportscanner_bin,
                         "-rootPath", qml_source_dir,
                         "-importPath", qmlModulesDir], stdout=PIPE)
        process.wait()

        if process.returncode == 0:
            raw_output = process.stdout.read()
            new_imports = json.loads(raw_output)
            qml_imports += new_imports
        else:
            print("ERROR: Unable to run qmlimportscanner")
            exit(1)

    qml_imports_included = []
    for qmlImport in qml_imports:
        if qmlImport["type"] == "module" and \
                "path" in qmlImport and \
                qmlImport not in qml_imports_included:
            source_path = qmlImport["path"]
            relative_path = qmlImport["relativePath"] if "relativePath" in qmlImport else ""
            target_path = app_dir + "/usr/qml/" + relative_path
            copytree(source_path, target_path)

            print("Including", qmlImport['name'], qmlImport['version'], "from", source_path)
            qml_imports_included += qmlImport


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
