import subprocess
import os
import platform
import sys
import json
import shutil

testsPath = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "tests"))

if testsPath not in sys.path:
    sys.path.append(testsPath)

import tests

def runTimeTest():
    tests.timeTest()
    print("----")

def runTests():
    tests.runTests()
    print("----")

def buildDebianPackage(releaseData):
    folderPath = f"releases/{releaseData.get('projectName')}_{releaseData.get('version')}"
    os.makedirs(os.path.join(folderPath, "usr", "local", "bin"), exist_ok=True)
    os.makedirs(os.path.join(folderPath, "DEBIAN"), exist_ok=True)
    os.chmod(os.path.join(folderPath, "DEBIAN"), 0o755)

    #if platform.system() == "Windows":
    #    shutil.copyfile("dio.exe", os.path.join(folderPath, "usr", "local", "bin", "dio.exe"))
    #    os.chmod(os.path.join(folderPath, "usr", "local", "bin", "dio.exe"), 0o755)
    #else:
    shutil.copyfile("dio", os.path.join(folderPath, "usr", "local", "bin", "dio"))
    os.chmod(os.path.join(folderPath, "usr", "local", "bin", "dio"), 0o755)

    with open(os.path.join(folderPath, "DEBIAN", "control"), "w") as f:
        f.write(f"""Package: {releaseData.get('projectName')}
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

def windowsBuild(releaseData):
    name = str(releaseData.get('defaultName')).replace("$platform", f"{platform.system()}-{platform.release()}").replace("$version", releaseData["version"]).replace("$projectName", releaseData.get('projectName')).replace("$releasePlatfrom", releaseData.get('releasePlatfrom')) + ".exe"
    linkBuilder = str(releaseData.get('windowsDownloadLink')).replace("$version", releaseData.get('version')).replace("$projectName", releaseData.get('projectName'))
    #print(linkBuilder)
    shutil.copyfile("dio.exe", os.path.join("releases", name))

    #generate / update YAML logic incomming here...

def generateExecutableForRelease(releaseData):
    #Builds general executable file for specific OS that's it's executed on!
    defaultFile = "dio.exe" if platform.system() == "Windows" else "dio"
    name = str(releaseData.get('defaultName')).replace("$platform", f"{platform.system()}-{platform.release()}").replace("$version", releaseData["version"]).replace("$projectName", releaseData.get('projectName')).replace("$releasePlatfrom", releaseData.get('releasePlatfrom'))
    name = name + ".exe" if platform.system() == "Windows" else name
    shutil.copyfile(defaultFile, os.path.join("releases", name))

def makeRelease():
    releaseData = {}

    with open(os.path.join(os.path.abspath(os.path.curdir), "scripts", "data", "release.json")) as f:
        releaseData = json.loads(f.read())

    if arguments["version"] != "UNKNOWN":
        releaseData["version"] = arguments["version"]

    folderPath = "releases"
    os.makedirs(folderPath, exist_ok=True)

    generateExecutableForRelease(releaseData)

    if platform.system() == "Windows":
        windowsBuild(releaseData)
    elif platform.system() == "Linux":
        buildDebianPackage(releaseData)
    elif platform.system() == "Android":
        pass
    elif platform.system() == "FreeBSD":
        pass
    elif platform.system() == "Darwin":
        pass
    elif platform.system() == "iOS":
        pass
    else:
        print(f"Platform '{platform.system()}' is not planned to be supported, write your supported build or contact developers (via issue other means).")


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
    "debug" : {
        "data" : False,
        "collect" : "switch"
    },
    "release" : {
        "data" : False,
        "collect" : "switch"
    },
    "version" : {
        "data" : "UNKNOWN",
        "collect" : "inputData"
    },
    "tests" : {
        "data" : True,
        "collect" : "switch"
    },
    "timeTest" : {
        "data" : True,
        "collect" : "switch"
    }
}

for arg in sys.argv[1:]:
    arg = arg.split("--") if arg[1] == "-" else arg.split("-")
    argArr = arg[1].split("=")
    arg = argArr[0]

    if arguments[arg]["collect"] == "switch":
        arguments[arg] = not arguments[arg]["data"]
    elif arguments[arg]["collect"] == "inputData":
        arguments[arg] = argArr[1]

for arg in arguments:
    if isinstance(arguments[arg], dict):
        arguments[arg] = arguments[arg]["data"]

makeSTDlib()

try:
    filename = "dio"
    slash = "/"
    if platform.system() == "Windows":
        filename = "dio.exe"
        slash = "\\"
    
    #subprocess.run("gcc -g -Wall -Wextra " + getFiles() + f" -o {filename}", shell=True, check=True)

    macros = ""

    if arguments["release"]:
        macros += ' -DBUILDTYPE=\\"release\\"'

    command = f"gcc -g {getFiles()} -o ..{slash}{filename} {macros}" if platform.system() != "iOS" else f" cd .. && clang ./DIO-code/C/*.c -o .{slash}{filename} {macros} && cd ./DIO-code" 

    #maybe this p could be used?
    #subprocess.run("gcc -pg " + getFiles() + f" -o ..{slash}{filename} {macros}", shell=True, check=True)
    print(command)
    subprocess.run(command, shell=True, check=True)
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

if arguments["tests"]:
    runTests()

if arguments["timeTest"]:
    runTimeTest()