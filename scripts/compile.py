import subprocess
import os
import platform
import sys
import json
import shutil
import tests.dio_tests as tests
import copy

#testsPath = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "tests"))
#
#if testsPath not in sys.path:
#    sys.path.append(testsPath)

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

def runValgrindTests(command):
    subprocess.run(f"valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s {command}", shell=True)

def runTimeTest():
    tests.timeTest()
    print("----")

def testCrashed():
    tests.testCrashes()
    print("----")

def runTests():
    tests.runTests()
    print("----")

def getPlatfromName():
    if platform.system() == "Windows":
        return platform.system()+ "-" + platform.release()
    else:
        return platform.system()

def buildName(releaseData):
    name = str(releaseData.get('releaseName')).replace("$platform", f"{getPlatfromName()}").replace("$version", releaseData["version"]).replace("$projectName", releaseData.get('projectName')).replace("$releasePlatfrom", releaseData.get('releasePlatfrom')) + ".exe"

    return name

def getPlatform(releaseData):
    #if releaseData.get("releasePlatfrom") != "":
    #    return releaseData.get("releasePlatfrom")

    #is this even correct?

    platformMap = {
        "amd64" : ["x86_64", "amd64", "AMD64"],
        "arm64" : ["aarch64", "arm64", "ARM64"],
        "i386": ["i386", "i686", "x86"]
    }

    curPlat = platform.machine()
    normalisedPlat = curPlat

    for k in platformMap:
        if curPlat in platformMap[k]:
            normalisedPlat = k
            break

    releaseData["releasePlatfrom"] = normalisedPlat
    
    with open(os.path.join(os.path.abspath(os.path.curdir), "scripts", "data", "release.json"), "w") as f:
        json.dump(releaseData, f, indent=4)

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

    subprocess.run(f"dpkg-deb --root-owner-group --build {folderPath}", shell=True)

    shutil.rmtree(folderPath)

def windowsBuild(releaseData):
    name = buildName(releaseData)
    shutil.copyfile("dio.exe", os.path.join("releases", name))

    #generate / update YAML logic incomming here (for https://github.com/microsoft/winget-pkgs if I ever try to be there actually)...

def generateExecutableForRelease(releaseData):
    #Builds general executable file for specific OS that's it's executed on!
    defaultFile = "dio.exe" if platform.system() == "Windows" else "dio"
    name = buildName(releaseData)
    shutil.copyfile(defaultFile, os.path.join("releases", name))

def buildWASMRelease():
    #Generate WASM files (when webrelease)
    if not os.path.exists("releases/webapp"):
        os.mkdir("releases/webapp")

    os.chdir("DIO-code")
    try:
        subprocess.run(f"emcc {getFiles()} -o {os.path.join("..", "releases", "webapp", "dio.html")} -s EXIT_RUNTIME=1", shell=True)
    except:
        print("emcc might not be installed, try to install it?")
        return
        
    os.chdir("..")
    shutil.copyfile(os.path.join("scripts", "data", "dio-fe.html"), os.path.join("releases", "webapp", "dio-fe.html"))
    shutil.copyfile(os.path.join("scripts", "data", "main.js"), os.path.join("releases", "webapp", "main.js"))
    shutil.copyfile(os.path.join("scripts", "data", "worker.js"), os.path.join("releases", "webapp", "worker.js"))
    shutil.copyfile(os.path.join("scripts", "data", "logo.png"), os.path.join("releases", "webapp", "logo.png"))

def makeRelease():
    releaseData = {}

    with open(os.path.join(os.path.abspath(os.path.curdir), "scripts", "data", "release.json")) as f:
        releaseData = json.loads(f.read())

    if arguments["version"] != "UNKNOWN":
        releaseData["version"] = arguments["version"]

    getPlatform(releaseData)

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
        "collect" : "switch",
        "inherit" : "None"
    },
    "debugArg" : {
        "data" : False,
        "collect" : "switch",
        "inherit" : "None"
    },
    "release" : {
        "data" : False,
        "collect" : "switch",
        "inherit" : "None"
    },
    "webRelease" : {
        "data" : False,
        "collect" : "switch",
        "inherit" : "release",
    },
    "version" : {
        "data" : "UNKNOWN",
        "collect" : "inputData",
        "inherit" : "None"
    },
    "tests" : {
        "data" : False,
        "collect" : "switch",
        "inherit" : "None"
    },
    "timeTest" : {
        "data" : False,
        "collect" : "switch",
        "inherit" : "tests"
    },
    "valgrind" : {
        "data" : False,
        "collect" : "switch",
        "inherit" : "None"
    },
    "crashTest" : {
        "data" : False,
        "collect" : "switch",
        "inherit" : "tests"
    }
}

unchangedArgs = copy.deepcopy(arguments)

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

#Inherit

for arg in unchangedArgs:
    if isinstance(unchangedArgs[arg], dict):
        if unchangedArgs[arg]["inherit"] != "None" and arguments[arg] == unchangedArgs[arg]["data"]:
            arguments[arg] = arguments[unchangedArgs[arg]["inherit"]]

#print(arguments["webRelease"])

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
    
    command = ""

    if platform.system() == "iOS":
        command = f"clang ./C/*.c -o .{slash}{filename} {macros}"
    elif platform.system() == "OpenBSD":
        command = f"egcc -g {getFiles()} -o ..{slash}{filename} {macros}"
    else:
        command = f"gcc -g {getFiles()} -o ..{slash}{filename} {macros}"
    
    #maybe this p could be used?
    #subprocess.run("gcc -pg " + getFiles() + f" -o ..{slash}{filename} {macros}", shell=True, check=True)
    #print(command)
    subprocess.run(command, shell=True, check=True)
    os.chdir("..")

    if os.path.exists(os.path.join("DIO-code", filename)):
        subprocess.run(f"mv {os.path.abspath(os.path.join("DIO-code", filename))} .{slash}{filename}")

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
            subprocess.run(f'gdb -ex run -ex bt --args {executablePath} {fName} {" -d " if arguments["debugArg"] == True else ""}', shell=True)
        else:
            subprocess.run(f"{executablePath} {fName} {" -d " if arguments["debugArg"] == True else ""}", shell=True)

except Exception as e:
    print(e)
    print("failed in compilation process")
    exit(1)

if arguments["release"]:
    makeRelease()

if arguments["webRelease"]:
    buildWASMRelease()

if arguments["tests"]:
    runTests()

if arguments["timeTest"]:
    runTimeTest()

if arguments["valgrind"]:
    runValgrindTests(f"{executablePath} {fName}")

if arguments["crashTests"]:
    testCrashed()
