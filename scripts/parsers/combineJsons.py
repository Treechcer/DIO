import os
import json

def combine():
    fs = os.listdir(os.path.join("scripts", "parsers"))

    nextJson = {}

    for f in fs:
        if f[-4::1] == "json" and f[0:-5] != "combined":
            with open(os.path.join("scripts", "parsers", f), "r") as fr:
                nextJson[f[0:-5]] = json.load(fr)

    with open(os.path.join("scripts", "parsers", "combined.json"), "w") as js:
        json.dump(nextJson, js, indent=4)