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


class QmlModulesImporter:
    def __init__(self):
        self.qml_source_dir = ""
        self.app_dir = ""
        self.qml_module_dirs = ["/usr/lib"]

        self.qmlimportscanner_bin = shutil.which("qmlimportscanner")

    def import_qml_modules(self):
        print("QML Modules Search paths: ", self.qml_module_dirs)
        qml_imports = self.scan_qml_imports()

        self.copy_qml_modules(qml_imports)

    def copy_qml_modules(self, qml_imports):
        qml_imports_included = []
        for qmlImport in qml_imports:
            if qmlImport["type"] == "module" and \
                    "path" in qmlImport and \
                    qmlImport not in qml_imports_included:
                source_path = qmlImport["path"]
                relative_path = qmlImport["relativePath"] if "relativePath" in qmlImport else ""
                target_path = self.app_dir + "/usr/qml/" + relative_path
                copytree(source_path, target_path)

                print("Including", qmlImport['name'], qmlImport['version'], "from", source_path)
                qml_imports_included += qmlImport

    def scan_qml_imports(self):
        qml_imports=[]
        for qmlModulesDir in self.qml_module_dirs:
            raw_output = self.run_qmlimportscanner(qmlModulesDir)

            new_imports = json.loads(raw_output)
            qml_imports += new_imports
        return qml_imports

    def run_qmlimportscanner(self, qmlModulesDir):
        raw_output = ''
        process = Popen([self.qmlimportscanner_bin,
                         "-rootPath", self.qml_source_dir,
                         "-importPath", qmlModulesDir], stdout=PIPE)
        process.wait()
        if process.returncode == 0:
            raw_output = process.stdout.read().decode('utf-8')
        else:
            print("ERROR: Unable to run qmlimportscanner")
            exit(1)
        return raw_output


def main(arguments):
    if len(sys.argv) < 3:
        print(
            "Usage:\n\tpython appimage.include.qmlmodules.py /sourcePath/to/your/app/qml/source/dir "
            "/sourcePath/to/the/AppDir [/paths/to/qml/modules/folders ...]\n")
        exit(1)

    qml_importer = QmlModulesImporter()
    qml_importer.qml_source_dir = sys.argv[1]
    qml_importer.app_dir = sys.argv[2]
    qml_importer.qml_module_dirs += sys.argv[3:]
    qml_importer.import_qml_modules()
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
