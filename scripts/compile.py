import subprocess
import os

def getFiles():
    curPath = os.path.join(os.path.abspath(os.path.curdir), "C")
    files = os.listdir(curPath)
    string = ""
    for o in files: string += os.path.join(curPath, o) + " "
    return string[:-1]
try:
    subprocess.run("gcc " + getFiles() + " -o lang.exe", shell=True, check=True)
    with open(os.path.abspath(os.path.join("scripts", "CompileRunParams.txt")), "r") as f:
        print(".\\lang.exe" + " " + f.read())
        subprocess.run(".\\lang.exe" + " " + f.read())
except:
    print("failed :(")