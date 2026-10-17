import subprocess
import os
import time

class objectTransfer:
    def __init__(self, errorcode, stdout):
        self.errorcode = errorcode
        self.stdout = stdout

class dioTalker:
    fileName = "temp.dio"

    def __init__(self, exePos, fileName = "temp.dio"):
        self.fileName = fileName
        self.exePos = exePos

    def executeCode(self, code, text=True):
        with open(self.fileName, "w") as f:
            f.write(code)

        returner = subprocess.run(f"{self.exePos} -f {self.fileName}", capture_output=True, shell=True, timeout=5, text=text) #5 seems like solid? worst time I've got on example was like 0.2s on iphone
        time.sleep(0.01) #this prevents issue, without this test were faiking (likely because rewring may have not finished but not sure)
        return objectTransfer(returner.returncode, returner.stdout)

    def disconnect(self):
        os.remove(self.fileName)