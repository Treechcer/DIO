import tests.dioLib as dioLib
import platform
import json
import os
import sys
import time

def runTests():
    dio = dioLib.dioTalker("./dio.exe" if platform.system() == "Windows" else "./dio")

    with open(os.path.join("scripts", "tests", "tests.json"), "r") as f:
        tests = json.loads(f.read())

    passed = 0
    failed = 0
    warning = 0

    for test in tests:
        returnObj = dio.executeCode(test["code"])

        if test["expectedOutput"][-1] != "\n":
            test["expectedOutput"] += "\n"

        if test["expectedReturnCode"] == returnObj.errorcode and test["expectedOutput"] == returnObj.stdout:
            print(f"{test["testIdentifier"]} : Passed")
            passed += 1
        elif test["expectedReturnCode"] == returnObj.errorcode and test["expectedOutput"] != "ERROR": # BE CAREFUL WITH THIS!
            print(f"{test["testIdentifier"]} : Passed")
            passed += 1            
        elif test["expectedReturnCode"] == returnObj.errorcode and test["expectedOutput"] != returnObj.stdout:
            print(f"{test["testIdentifier"]} : Warning")
            print(test["expectedOutput"], returnObj.stdout)
            warning += 1
        else:
            print(f"{test["testIdentifier"]} : Failed")
            print(returnObj.stdout)
            failed += 1

    print(f"Passed {passed} out of {len(tests)}")
    print(f"Failed {failed} out of {len(tests)}")
    print(f"Warning for {warning} out of {len(tests)}")

    dio.disconnect()

def timeTest():
    dio = dioLib.dioTalker("./dio.exe" if platform.system() == "Windows" else "./dio")

    times = []

    with open(os.path.join("scripts", "tests", "timeTest.json"), "r") as f:
        timeTest = json.loads(f.read())

    for fileName_ in os.listdir("./examples"):
        if fileName_[-3::] == "dio":
            content = ""
            #print(fileName_)
            with open(os.path.join(".", "examples", fileName_), "r") as f:
                content = f.read()

            if fileName_ in timeTest:
                if "skip" in timeTest[fileName_]:
                    if timeTest[fileName_]["skip"]:
                        continue

            start = time.time()
            dio.executeCode(content)
            end = time.time()

            #print(end - start)

            times.append({"len" : end - start, "file" : fileName_})

    longest = []
    for i in range(3):
        times, longTemp = getLongestTime(times)
        longest.append(longTemp)

    for i in range(len(longest)):
        print(f"{longest[i]["file"]} : {longest[i]["len"]}s")

    dio.disconnect()

def getLongestTime(times):
    maxTimeCopy = times[0]
    index = -1

    for i in range(1, len(times)):
        if times[i]["len"] > times[index]["len"]:
            index = i

    maxTimeCopy = times.pop(index)
    return times, maxTimeCopy

if __name__ == "__main__":
    runTests()
    print("----")
    timeTest()