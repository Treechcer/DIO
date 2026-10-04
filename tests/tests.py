import dioLib as dioLib
import platform

dio = dioLib.dioTalker("./dio.exe" if platform.system() == "Windows" else "./dio")

tests = [
    {
        "testIdentifier" : "testing out",
        "code" : "out(\"a\")",
        "expectedReturnCode" : 0,
        "expectedOutput" : "a\n"
    },
    {
        "testIdentifier" : "testing out int",
        "code" : "out(15)",
        "expectedReturnCode" : 0,
        "expectedOutput" : "15.0\n"
    }
]

passed = 0
failed = 0
warning = 0

for test in tests:
    returnObj = dio.executeCode(test["code"])

    if test["expectedReturnCode"] == returnObj.errorcode and test["expectedOutput"] == returnObj.stdout:
        print(f"{test["testIdentifier"]} : Passed")
        passed += 1
    elif test["expectedReturnCode"] == returnObj.errorcode and test["expectedOutput"] != returnObj.stdout:
        print(f"{test["testIdentifier"]} : Warning")
        #print(test["expectedOutput"], returnObj.stdout)
        warning += 1
    else:
        print(f"{test["testIdentifier"]} : Failed")
        failed += 1

print(f"Passed {passed} out of {len(tests)}")
print(f"Failed {failed} out of {len(tests)}")
print(f"Warning for {warning} out of {len(tests)}")