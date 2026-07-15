import subprocess
import os
import platform

def makeSTDlib():
    std = os.path.join(os.path.join(os.path.abspath(os.path.curdir), "scripts"), "STD.dio")
    stdC = os.path.join(os.path.join(os.path.abspath(os.path.curdir), "C"), "STD.c")
    strStd = ""

    with open(std, "r") as f:
        for i in f.readlines():
            strStd+= i
    os.remove(stdC)
    with open(stdC, "x") as f:
        f.write(strStd)

def getFiles():
    curPath = os.path.join(os.path.abspath(os.path.curdir), "C")
    files = os.listdir(curPath)
    string = ""
    for o in files: string += os.path.join(curPath, o) + " "
    return string[:-1]

#makeSTDlib()

try:
    filename = "lang"
    slash = "/"
    if platform.system() == "Windows":
        filename = "lang.exe"
        slash = "\\"
    #subprocess.run("gcc -g -Wall -Wextra " + getFiles() + f" -o {filename}", shell=True, check=True)
    subprocess.run("gcc -g " + getFiles() + f" -o {filename}", shell=True, check=True)

    with open(os.path.abspath(os.path.join("scripts", "CompileRunParams.txt")), "r") as f:
        subprocess.run(f".{slash}{filename}" + " " + f.read(), shell=True)
except Exception as e:
    print(e)
    print("failed :(")
