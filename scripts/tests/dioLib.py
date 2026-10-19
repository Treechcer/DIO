import subprocess
import os
import time

# No idea if this works btw.

try:
    import psutil
except:
    print("psutil not install or isn't being able to be imported for some reason, this is essential, ending.")
    exit(1)

class objectTransfer:
    def __init__(self, errorcode, stdout):
        self.errorcode = errorcode
        self.stdout = stdout

class dioTalker:
    fileName = "temp.dio"

    def __init__(self, exePos, fileName = "temp.dio"):
        self.fileName = fileName
        self.exePos = exePos

    def executeCode(self, code, text=True, maxTimeout = 5):
        with open(self.fileName, "w") as f:
            f.write(code)
        try:
            returner = subprocess.run(f"{self.exePos} -f {self.fileName}", capture_output=True, shell=True, timeout=maxTimeout, text=text) #5 seems like solid? worst time I've got on example was like 0.2s on iphone
        except subprocess.TimeoutExpired:
            time.sleep(0.01)
            return objectTransfer(1, "")

        time.sleep(0.01) #this prevents issue, without this test were faiking (likely because rewring may have not finished but not sure)
        return objectTransfer(returner.returncode, returner.stdout)

    def disconnect(self):
        os.remove(self.fileName)

        for process in psutil.process_iter(['name', 'pid', 'username']):
            if process.info["name"] == "dio":
                p = psutil.Process(process.info["pid"])
                p.terminate()