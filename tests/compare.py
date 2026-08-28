from tkinter import PIESLICE
from ntpath import basename
import subprocess, os
import json

import buildTestsFromExamples as btfe

failedTests = {
    "failed" : {},
    "warning" : {},
    "timeOuts" : []
}
fileMetadata = {}

def runOne(fileName : str, exectuable : str, jsonContent : list[str], fileMetadata : list[str]):
    try:
        time=1
        try:
            time=fileMetadata[os.path.basename(fileName)]["time"]
        except:
            pass
        stdout = str(subprocess.run([f"{exectuable}", "-file", f"{fileName}"], capture_output=True, text=True, timeout=time).stdout)
    except subprocess.TimeoutExpired:
        stdout = "NOT FINISHED!"

    if stdout == "NOT FINISHED!":
        try:
            if fileMetadata[os.path.basename(fileName)]["canTimeOut"]:
                failedTests["warning"][os.path.basename(fileName)] = {"warn" : "timeOut"}
                print(f"[WARNING (time outable file)]: {os.path.basename(fileName)}")
                return 0
        except:
            print(f"[TIMED OUT]: {os.path.basename(fileName)}")
            failedTests["timeOuts"].append(os.path.basename(fileName))
            return 3

    if stdout == jsonContent[fileName]:
        print(f"[PASSED]: {os.path.basename(fileName)}")
        return 0
    try:
        if fileMetadata[os.path.basename(fileName)]["randomness"]:
            failedTests["warning"][os.path.basename(fileName)] = {"warn" : "randomness", "run" : stdout.replace('\\', '\\\\').replace('\"', '\\\"').replace("\n", "\\n"), "test": jsonContent[fileName]}
            print(f"[WARNING (randomness)]: {os.path.basename(fileName)}")
            return 2
    except:
        pass

    failedTests["failed"][os.path.basename(fileName)] = {"run" : stdout.replace('\\', '\\\\').replace('\"', '\\\"').replace("\n", "\\n"), "test": jsonContent[fileName]}

    print(f"[FAILED]: {os.path.basename(fileName)}")
    return 1

def main():
    global failedTests, fileMetadata

    btfe.checkDir()
    executable = btfe.getExecutable()
    
    with open(os.path.join("tests", "base.json"), "r") as js:
        testResults = json.load(js)

    with open(os.path.join("examples", ".metadata.json"), "r") as js:
        fileMetadata = json.load(js)

    tests = [0, 0, 0, 0] #passed, failed, warning, timeout
    examples = os.listdir("examples")

    for ex in examples:
        if os.path.splitext(ex)[1] != ".dio":
            continue
        tests[runOne(os.path.join("examples", ex), executable, testResults, fileMetadata)] += 1

    print(f"total tests: {tests[1] + tests[0]}")
    print(f"failed tests: {tests[1]}")
    print(f"warnings: {tests[2]}")
    print(f"timed out tests: {tests[3]}")
    print(f"passed tests: {tests[0]}")

    with open("tests/results.json", "w+") as res:
        res.write(json.dumps(failedTests, indent=4, sort_keys=True))

if __name__ == "__main__":
    main()