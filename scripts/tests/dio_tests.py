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
        returnObj = []
        outputs = []
        for i in range(test["maxRuns"]):
            returnObj.append(dio.executeCode(test["code"]))

        for output in test["expectedOutput"]:
            if output[-1] != "\n":
                output += "\n"

            passedCur = 0

            for curRetCheck in returnObj:
                if test["expectedReturnCode"] == curRetCheck.errorcode and output == curRetCheck.stdout:
                    passedCur = 1
                    if not curRetCheck.stdout in outputs:
                        outputs.append(curRetCheck.stdout)
                    break
                elif test["expectedReturnCode"] == curRetCheck.errorcode and output == "ERROR\n":
                    passedCur = 2
                    if not curRetCheck.stdout in outputs:
                        outputs.append(curRetCheck.stdout)
                    break
            
            if passedCur == 0 or len(test["expectedOutput"]) != len(outputs):
                print(f"{test["testIdentifier"]} : Failed")
                print(f"({curRetCheck.errorcode})", test["expectedOutput"], "|", outputs)
                failed += 1
                break

        if passedCur == 1:
            print(f"{test["testIdentifier"]} : Passed")
            passed += 1
        if passedCur == 2:
            print(f"{test["testIdentifier"]} : Passed [err type]")
            passed += 1

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
            dio.executeCode(content, False)
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

def testCrashes():
    dio = dioLib.dioTalker("./dio.exe" if platform.system() == "Windows" else "./dio")

    ex = os.listdir("./examples")

    for example in ex:
        if example[-3::] == "dio":
            with open(os.path.join(".", "examples", example), "r") as f:
                content = f.read()

            returnObj = dio.executeCode(content, False, 0.75)
            time.sleep(0.1)

            if returnObj.errorcode != 0:
                print(f"{example} (RC: {returnObj.errorcode})")

    dio.disconnect()

if __name__ == "__main__":
    runTests()
    print("----")
    timeTest()
    print("----")
    testCrashes()