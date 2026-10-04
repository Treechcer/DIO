import subprocess
import os
import time

class objectTransfer:
    def __init__(self, errorcode, stdout):
        self.errorcode = errorcode
        self.stdout = stdout

class dioTalker:
    fileName = "temp.dio"

    def __init__(self, exePos):
        self.exePos = exePos

    def executeCode(self, code):
        with open(self.fileName, "w") as f:
            f.write(code)

        returner = subprocess.run(f"{self.exePos} -f {self.fileName}", capture_output=True, text=True)
        time.sleep(0.01)
        return objectTransfer(returner.returncode, returner.stdout)

    def disconnect(self):
        os.remove(self.fileName)