import subprocess
import os

class objectTransfer:
    def __init__(self, errorcode, stdout):
        self.errorcode = errorcode
        self.stdout = stdout

class dioTalker:
    def __init__(self, exePos):
        self.exePos = exePos

    def executeCode(self, code):
        fileName = "temp.dio"
        with open(fileName, "w") as f:
            f.write(code)

        returner = subprocess.run(f"{self.exePos} -f {fileName}", capture_output=True, text=True)
        os.remove(fileName)
        return objectTransfer(returner.returncode, returner.stdout)
        