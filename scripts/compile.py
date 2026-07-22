import subprocess
import os
import platform
import sys

arguments = {
    "debug" : False
}

for arg in sys.argv[1:]:
    arg = arg.split("--") if arg[1] == "-" else arg.split("-")
    arguments[arg[1]] = not arguments[arg[1]]

def makeSTDlib():
    std = os.path.join(os.path.join(os.path.abspath(os.path.curdir), "scripts"), "STD.dio")
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
    files = os.listdir(curPath)
    string = ""
    for o in files: string += os.path.join(curPath, o) + " "
    return string[:-1]

makeSTDlib()

try:
    filename = "lang"
    slash = "/"
    if platform.system() == "Windows":
        filename = "lang.exe"
        slash = "\\"
    #subprocess.run("gcc -g -Wall -Wextra " + getFiles() + f" -o {filename}", shell=True, check=True)
    subprocess.run("gcc -g " + getFiles() + f" -o {filename}", shell=True, check=True)

    with open(os.path.abspath(os.path.join("scripts", "CompileRunParams.txt")), "r") as f:
        if arguments["debug"]:
            subprocess.run(f'gdb -ex run -ex bt --args .{slash}{filename} {f.read()}', shell=True)
        else:
            subprocess.run(f".{slash}{filename}" + " " + f.read(), shell=True)
except Exception as e:
    print(e)
    print("failed :(")
