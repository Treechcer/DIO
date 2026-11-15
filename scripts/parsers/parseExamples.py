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
            "stdout" : expectedOutput,
            "stderr" : expectedError
        }
    }

def parse(output : str):
    dio = dioLib.dioTalker("./dio.exe" if platform.system() == "Windows" else "./dio")

    fs = os.listdir("examples")

    preJson = []    
    for file in fs:
        with open(os.path.join(".", "examples", file)) as f:
            try:
                num = re.findall("\\d+", file)[0]
            except:
                continue
            fileC = f.read()
            try:
                out = dio.executeCode(fileC)
                if out.errorcode != 0:
                    continue    
                preJson.append(generateJsonProperty(num, fileC, out.stdout, out.errorcode))
            except:
                continue

    with open(output, "w") as f:
        json.dump(preJson, f, indent=4)

    dio.disconnect()

if __name__ == "__main__":
    parse("./scripts/parsers/examples.json")