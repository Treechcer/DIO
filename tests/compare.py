from tkinter import PIESLICE
from ntpath import basename
import subprocess, os
import json

import buildTestsFromExamples as btfe

failedTests = {
    "failed" : {},
    "warning" : {}
}
fileMetadata = {}

def runOne(fileName : str, exectuable : str, jsonContent : list[str], fileMetadata : list[str]):
    try:
        stdout = str(subprocess.run([f"{exectuable}", "-file", f"{fileName}"], capture_output=True, text=True, timeout=1).stdout)
    except subprocess.TimeoutExpired:
        stdout = "NOT FINISHED!"

    if stdout == jsonContent[fileName]:
        print(f"[PASSED]: {fileName}")
        return 0
    try:
        if fileMetadata[os.path.basename(fileName)]["randomness"]:
            failedTests["warning"][fileName] = {"run" : stdout.replace('\\', '\\\\').replace('\"', '\\\"').replace("\n", "\\n"), "test": jsonContent[fileName]}
            print(f"[WARNING (randomness)]: {fileName}")
            return 2
    except:
        pass

    failedTests["failed"][fileName] = {"run" : stdout.replace('\\', '\\\\').replace('\"', '\\\"').replace("\n", "\\n"), "test": jsonContent[fileName]}

    print(f"[FAILED]: {fileName}")
    return 1

def main():
    global failedTests, fileMetadata

    btfe.checkDir()
    executable = btfe.getExecutable()
    
    with open(os.path.join("tests", "base.json"), "r") as js:
        testResults = json.load(js)

    with open(os.path.join("examples", ".metadata.json"), "r") as js:
        fileMetadata = json.load(js)

    tests = [0, 0, 0] #passed, failed, warning
    examples = os.listdir("examples")

    for ex in examples:
        if os.path.splitext(ex)[1] != ".dio":
            continue
        tests[runOne(os.path.join("examples", ex), executable, testResults, fileMetadata)] += 1

    print(f"total tests: {tests[1] + tests[0]}")
    print(f"failed tests: {tests[1]}")
    print(f"warnings: {tests[2]}")
    print(f"passed tests: {tests[0]}")

    with open("tests/results.json", "w+") as res:
        res.write(json.dumps(failedTests, indent=4, sort_keys=True))

if __name__ == "__main__":
    main()