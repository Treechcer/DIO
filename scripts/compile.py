import subprocess
import os
import platform
import sys
import json
import shutil

def buildDebianPackage(releaseData):
    folderPath = f"releases/{releaseData.get('projetName')}_{releaseData.get('version')}"
    os.makedirs(os.path.join(folderPath, "usr", "local", "bin"), exist_ok=True)
    os.makedirs(os.path.join(folderPath, "DEBIAN"), exist_ok=True)
    os.chmod(os.path.join(folderPath, "DEBIAN"), 0o755)

    if platform.system() == "Windows":
        shutil.copyfile("dio.exe", os.path.join(folderPath, "usr", "local", "bin", "dio.exe"))
        os.chmod(os.path.join(folderPath, "usr", "local", "bin", "dio.exe"), 0o755)
    else:
        shutil.copyfile("dio", os.path.join(folderPath, "usr", "local", "bin", "dio"))
        os.chmod(os.path.join(folderPath, "usr", "local", "bin", "dio"), 0o755)

    with open(os.path.join(folderPath, "DEBIAN", "control"), "w") as f:
        f.write(f"""Package: {releaseData.get('projetName')}
Version: {releaseData.get('version')}
Section: base
Priority: optional
Architecture: {releaseData.get('releasePlatfrom')}
Maintainer: {releaseData.get('releaser')} <{releaseData.get('releaserEmail')}>
Description: {releaseData.get('description')}
""")
    
    os.chmod(os.path.join(folderPath, "DEBIAN", "control"), 0o644)

    subprocess.run(f"dpkg-deb --build {folderPath}", shell=True)

    shutil.rmtree(folderPath)

def makeRelease():
    releaseData = {}

    with open(os.path.join(os.path.abspath(os.path.curdir), "scripts", "data", "release.json")) as f:
        releaseData = json.loads(f.read())

    folderPath = "releases"
    if not os.path.exists(folderPath):
        os.makedirs(folderPath)

    buildDebianPackage(releaseData)


def makeSTDlib():
    std = os.path.join(os.path.join(os.path.abspath(os.path.curdir), "..", "scripts", "data"), "STD.dio")
    stdC = os.path.join(os.path.join(os.path.abspath(os.path.curdir), "C"), "STD.c")
    stdH = os.path.join(os.path.join(os.path.abspath(os.path.curdir), "Headers"), "STD.h")
    strStd = """char* getSTD(){
    return \"\\
"""

    with open(std, "r") as f:
        for i in f.readlines():
            strStd+= i.replace("\n", "\\n\\\n")

    strStd += """\\n\\n\\
\";
}"""
    try:
        os.remove(stdC)
    except:
        pass

    with open(stdC, "x") as f:
        f.write(strStd)

    try:
        os.remove(stdH)
    except:
        pass

    with open(stdH, "x") as f:
        f.write("""#ifndef STD__H
#define STD__H

char* getSTD();

#endif""")

def getFiles():
    curPath = os.path.join(os.path.abspath(os.path.curdir), "C")
    legacyPath = ""
    if os.path.exists(os.path.join(os.path.abspath(os.path.curdir), "legacy")):
        legacyPath = os.path.join(os.path.abspath(os.path.curdir), "legacy")
    files = os.listdir(curPath)
    string = ""
    for o in files: string += os.path.join(curPath, o) + " "
    if legacyPath != "":
        for o in os.listdir(legacyPath): string += os.path.join(legacyPath, o) + " "
    return string[:-1]

s = "/"
if platform.system() == "Windows":
    s = "\\"

if os.path.abspath(os.curdir).split(s)[-1] != "DIO-code":
    os.chdir(os.path.join(os.curdir,"DIO-code"))

if os.path.abspath(os.curdir).split(s)[-1] != "DIO-code":
    print("incorrect folder???")
    exit()

arguments = {
    "debug" : False,
    "release" : False
}

for arg in sys.argv[1:]:
    arg = arg.split("--") if arg[1] == "-" else arg.split("-")
    arguments[arg[1]] = not arguments[arg[1]]

makeSTDlib()

try:
    filename = "dio"
    slash = "/"
    if platform.system() == "Windows":
        filename = "dio.exe"
        slash = "\\"
    
    #subprocess.run("gcc -g -Wall -Wextra " + getFiles() + f" -o {filename}", shell=True, check=True)
    subprocess.run("gcc -g " + getFiles() + f" -o ..{slash}{filename}", shell=True, check=True)
    os.chdir("..")
    with open(os.path.abspath(os.path.join("scripts", "data", "CompileRunParams.txt")), "r") as f:
        executablePath = os.path.abspath(filename)
        #normalises slashes to whatever the OS uses, or should at least lol
        try:
            fName = f.read().split("-f ")[1].replace("/", slash).replace("\\", slash)
            fName = f"-f {fName}"
        except:
            fName = ""
        #os.chdir(os.path.abspath(os.path.dirname(fName)))
        if arguments["debug"]:
            subprocess.run(f'gdb -ex run -ex bt --args {executablePath} {fName}', shell=True)
        else:
            subprocess.run(f"{executablePath} {fName}", shell=True)
except Exception as e:
    print(e)
    print("failed :(")

if arguments["release"]:
    makeRelease()