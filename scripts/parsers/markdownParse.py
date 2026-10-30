import json

def parse(filePath, output):
    parsedObj = []

    with open(filePath, "r") as f:
        content = f.readlines()

    i = 0
    while i < len(content):
        line = content[i]
        level = 0

        if line.strip() == "" or line == "" or (line[0] == "!" and line[1] == "[") or (line[0] == "<" and line[-2] == ">"):
            i = i + 1
            continue            

        if (len(line.split("<!--")) > 1):
            while not len(line.split("-->")) > 1:
                line = content[i]
                i = i + 1

            i = i + 1
            continue

        if line[0] == "#":
            for j in range(6):
                if line[j] == "#":
                    level += 1
                else:
                    break

        print(f"{i}: {line}")
        #print(line.split("#" * (level)))
        
        headerText = line.split("#" * level)[1]

        textInParagraph = ""
        num = 0
        i += 1
        try:
            while (content[i + num][0] != "#"):
                textInParagraph += content[i + num]
                num = num + 1
        except IndexError:
            pass

        i = i + num - 1

        tempMap = {
            "level" : level,
            "headerText" : headerText,
            "textInParagraph" : textInParagraph
        }

        parsedObj.append(tempMap)
        
        i = i + 1

    with open(output, "w") as f:
        json.dump(parsedObj, f, indent=4)

if __name__ == "__main__":
    parse("README.md", "./scripts/parsers/markdown.json")