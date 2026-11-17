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

def buildOneForLater(retName, fPath, file):
    return {
        "retName" : retName,
        "fPath" : fPath,
        "file" : file
    }

def parseFolder(input : str = "examples", recCall = 0):
    dio = dioLib.dioTalker("./dio.exe" if platform.system() == "Windows" else "./dio")

    ret = {}
    fs = os.listdir(input)
    preJson = []
    count = 0
    later = []
    for file in fs:
        count += 1
        print(f"Progress {os.path.basename(input)}: {(count/len(fs))*100:.2f}%", end="\r")
        if os.path.isdir(os.path.join(input, file)):
            later.append(buildOneForLater(os.path.basename(input), os.path.join(os.curdir, input, file), file))
            continue
        #try:
        with open(os.path.join(input, file)) as f:
            try:
                num = re.findall("\\d+", file)[0]
            except:
                continue
            fileC = f.read()
            try:
                out = dio.executeCode(fileC, maxTimeout=1)
                preJson.append(generateJsonProperty(num, fileC, out.stdout, out.errorcode))
            except:
                continue
        #except:
        #    pass
    if recCall == 0:
        ret["main"] = preJson
    else:
        ret = preJson

    print()
    dio.disconnect()

    for l in later:
        try:
            ret["side"].append(parseFolder(l["fPath"], 1))
        except KeyError:
            ret["side"] = []
            ret["side"].append(parseFolder(l["fPath"], 1))
    
    return ret

def parse(output : str, input : str = "examples" ):
    preJson = parseFolder()
    with open(output, "w") as f:
        json.dump(preJson, f, indent=4)

if __name__ == "__main__":
    parse("./scripts/parsers/examples.json")