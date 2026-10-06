import dioLib as dioLib
import platform
import json
import os
import sys

def runTests():
    dio = dioLib.dioTalker("./dio.exe" if platform.system() == "Windows" else "./dio")

    with open(os.path.join("tests", "tests.json"), "r") as f:
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

if __name__ == "__main__":
    runTests()