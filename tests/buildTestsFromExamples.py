import os
import subprocess
import platform

#changes dir from tests to DIO 
def checkDir():
    cdir = os.path.abspath(os.curdir)

    dir = []
    dir = cdir.split("/")
    if len(dir) == 0 or len(dir) == 1:
        dir = cdir.split("\\")

    if dir[-1] == "tests":
        os.chdir("..")

def getExecutable():
    executable = "lang"
    if platform.system() == "Windows":
        executable="lang.exe"

    if not os.path.exists(executable):
        print(os.path.abspath(executable))
        print("LANG NOT COMPILED!")
        exit(1)
    
    return executable

def buildTestFromFile(fileName : str, exectuable : str, last : bool):
    print(fileName)
    with open("./tests/results.json", "a") as f:
        try:
            stdout = str(subprocess.run([f"{exectuable}", "-file", f"{fileName}"], capture_output=True, text=True, timeout=1).stdout).replace('\\', '\\\\').replace('\"', '\\\"').replace("\n", "\\n")
        except subprocess.TimeoutExpired:
            stdout = "NOT FINISHED!"
        f.write(f"    \"{fileName.replace('\\', '\\\\').replace('\"', '\\\"')}\" : \"{stdout}\"")
        if not last:
            f.write(",")
        f.write("\n")

def main():
    checkDir()
    executable = getExecutable()

    with open("./tests/results.json", "w+") as f:
        f.write("{\n")

    examples = os.listdir("examples")

    for ex in examples:
        buildTestFromFile(os.path.join("examples", ex), executable, ex == examples[-1])

    with open("./tests/results.json", "a") as f:
        f.write("}")

if __name__ == "__main__":
    main()