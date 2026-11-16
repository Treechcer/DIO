import shared.bcolors as bc
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

    times = []

    for test in tests:
        returnObj = []
        outputs = []
        failedOuts = []

        try:
            dio.changeFileName(test["fileName"])
        except:
            dio.changeFileName("temp.dio")

        for i in range(test["maxRuns"]):
            start = time.time()           
            returnObj.append(dio.executeCode(test["code"]))
            end = time.time()
            times.append({"len" : end - start, "file" : test["testIdentifier"]})

        for output in test["expectedOutput"]:
            if len(output) < 1 or output == "":
                output = "\n"
            elif output[-1] != "\n":
                output += "\n"

            passedCur = 0

            for curRetCheck in returnObj:
                if test["expectedReturnCode"] == curRetCheck.errorcode and curRetCheck.stdout in [p + "\n" if p[-1] != "\n" else p for p in output.split("|-|")]:
                    passedCur = 1
                    if not curRetCheck.stdout in outputs:
                        outputs.append(curRetCheck.stdout)
                    break
                elif len(output.split("ERROR:")) > 1:
                    out = output.split("ERROR:")[1].strip()

                    errorMap = {
                        "lexer.c" : 1,
                        "ast.c" : 2,
                        "parser.c" : 3,
                    #    "helper_functions.c" : 4,
                    }

                    if out in curRetCheck.stdout:
                        passedCur = 1
                    else:
                        passedCur = 0

                    if test["expectedReturnCode"] == 0:
                        errorCode = 5
                        try:
                            errorCode = errorMap[out]
                        except:
                            pass

                        #print(errorCode, curRetCheck.errorcode)

                        if errorCode == curRetCheck.errorcode:
                            pass
                        else:
                            passedCur = 3
                    else:
                        if test["expectedReturnCode"] == curRetCheck.errorcode:
                            pass
                        else:
                            passedCur = 3

                    outputs.append(curRetCheck.stdout)
                #elif test["expectedReturnCode"] == curRetCheck.errorcode and output == "ERROR\n":
                #    passedCur = 2
                #    if not curRetCheck.stdout in outputs:
                #        outputs.append(curRetCheck.stdout)
                #    break
                else:
                    failedOuts.append(curRetCheck.stdout)
            
            if passedCur == 0 or len(test["expectedOutput"]) != len(outputs):
                print(f"{bc.bcolors.FAIL} {test["testIdentifier"]} : Failed {bc.bcolors.ENDC}")
                print(f"({curRetCheck.errorcode})", test["expectedOutput"], "|", failedOuts)
                failed += 1
                break

        if passedCur == 1:
            print(f"{bc.bcolors.OKGREEN}{test["testIdentifier"]} : Passed {bc.bcolors.ENDC}")
            passed += 1
        if passedCur == 2:
            print(f"{bc.bcolors.WARNING}{test["testIdentifier"]} : Passed [err type] {bc.bcolors.ENDC}")
            passed += 1
        if passedCur == 3:
            print(f"{bc.bcolors.WARNING}{ test["testIdentifier"]} : Warning {bc.bcolors.ENDC}")
            warning += 1
    
    print()
    print(f"Passed {passed} out of {len(tests)}")
    print(f"Failed {failed} out of {len(tests)}")
    print(f"Warning for {warning} out of {len(tests)}")

    print("----\nLongest test times:")
    printLongestTimes(10, times)

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

    printLongestTimes(10, times)

    dio.disconnect()

def printLongestTimes(N, times):
    longest = getNLongestTimes(N if N <= len(times) else len(times), times)

    for i in range(len(longest)):
        #This makes the text the same length so it's formated better
        maxLen = 15
        print(f"{longest[i]["file"] + (" " * ((maxLen+2) - len(longest[i]["file"]))) if len(longest[i]["file"]) < maxLen else longest[i]["file"][0:maxLen-1] + "..."} : {longest[i]["len"]:.4f}s")

def getNLongestTimes(N, times):
    longest = []
    for i in range(N):
        times, longTemp = getLongestTime(times)
        longest.append(longTemp)

    return longest

def getLongestTime(times):
    maxTimeCopy = times[0]
    index = -1

    for i in range(1, len(times)):
        if times[i]["len"] > times[index]["len"]:
            index = i

    maxTimeCopy = times.pop(index)
    return times, maxTimeCopy

def executeFolder(counter = 0, folder = os.path.join(".", "examples")):
    dio = dioLib.dioTalker("./dio.exe" if platform.system() == "Windows" else "./dio")
    ex = os.listdir(folder)

    for example in ex:
        if os.path.isdir(os.path.join(folder, example)):
            counter = executeFolder(counter, os.path.join(folder, example))
        elif example[-3::] == "dio":
            with open(os.path.join(folder, example), "r") as f:
                content = f.read()

            returnObj = dio.executeCode(content, False, 0.75)

            if returnObj.errorcode != 0:
                print(f"{example} (RC: {returnObj.errorcode})")
                counter += 1

    dio.disconnect()
    return counter

def testCrashes():
    print(f"Crashed count: {executeFolder()}")


if __name__ == "__main__":
    runTests()
    print("----")
    timeTest()
    print("----")
    testCrashes()