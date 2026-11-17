import tests.dioLib as dioLib
import json
import os
import platform
import re

def generateJsonProperty(fName : str, fileConten : str, expectedOutput : str, expectedError : int):
    return {
        "file" : fName,
        "content" : fileConten,
        "output" : {
            "stdout" : expectedOutput if expectedError == 0 else "",
            "stderr" : expectedError
        }
    }

def parse(output : str):
    dio = dioLib.dioTalker("./dio.exe" if platform.system() == "Windows" else "./dio")

    fs = os.listdir("examples")

    preJson = []
    count = 0
    for file in fs:
        count += 1
        print(f"Progress: {(count/len(fs))*100:.2f}%", end="\r")
        if os.path.isdir(os.path.join(".", "examples", file)):
            #temp skip
            continue
        with open(os.path.join(".", "examples", file)) as f:
            try:
                num = re.findall("\\d+", file)[0]
            except:
                continue
            fileC = f.read()
            try:
                out = dio.executeCode(fileC)
                preJson.append(generateJsonProperty(num, fileC, out.stdout, out.errorcode))
            except:
                continue

    with open(output, "w") as f:
        json.dump(preJson, f, indent=4)

    dio.disconnect()
    print()

if __name__ == "__main__":
    parse("./scripts/parsers/examples.json")