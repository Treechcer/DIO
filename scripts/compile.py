import subprocess
import os
import platform

def getFiles():
    curPath = os.path.join(os.path.abspath(os.path.curdir), "C")
    files = os.listdir(curPath)
    string = ""
    for o in files: string += os.path.join(curPath, o) + " "
    return string[:-1]

try:
    subprocess.run("gcc " + getFiles() + " -o lang.exe", shell=True, check=True)
    filename = "lang"
    slash = "/"
    if platform.system() == "Windows":
        filename = "lang.exe"
        slash = "\\"
    with open(os.path.abspath(os.path.join("scripts", "CompileRunParams.txt")), "r") as f:
        subprocess.run(f".{slash}{filename}" + " " + f.read(), shell=True)
except Exception as e:
    print(e)
    print("failed :(")