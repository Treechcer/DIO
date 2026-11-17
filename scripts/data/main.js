let dio = null

function getArgs(inputId){
    input = document.getElementById(inputId)
    let arrOut = ["-e", input.value];
    
    if (inputId != "inputText-1"){
        return arrOut
    }

    if (document.getElementById("debug").checked){
        arrOut.push("-d")
    }

    if (document.getElementById("AppInfo").checked){
        arrOut.push("-a")
    }

    if (document.getElementById("version").checked){
        arrOut.push("-v")
    }

    return arrOut;
}

function runDio(inputId, outputId) {
    dio?.terminate(inputId);

    arg = getArgs(inputId)

    if (arg[1].trim() == "" || !arg[1]){
        return
    }

    dio = new Worker("worker.js");
    dio.onmessage = ({ data }) => {
        document.getElementById(outputId).value += "DIO>>: " + data.text + "\n";
    };

    dio.postMessage({ args: arg });
}

function clearOutput() {
    document.getElementById("outputText").value = ""
}

function drawWiki(wikiJson) {
    div = document.getElementById("wiki")
    for (i = 0; i < wikiJson.length; i++){
        div.insertAdjacentHTML("beforeend", `<h${wikiJson[i].level}> ${wikiJson[i].headerText} </h${wikiJson[i].level}>`)
        div.insertAdjacentHTML("beforeend", `<pre>${wikiJson[i].textInParagraph}</pre>`)
    }
}

function drawExamples(exJson, type) {
    exJson.sort(function (a, b) {
        return a.file - b.file   })

    div = document.getElementById("example")
    div.insertAdjacentHTML("beforeend", `<h1 class="text-center"> ${type} Examples </h1>`)
    for (i = 0; i < exJson.length; i++){
        div.insertAdjacentHTML("beforeend", `<h2>Example ${exJson[i].file} </h2>`)
        div.insertAdjacentHTML("beforeend", `<div class="row"> 
                    <div class="col-3 mx-auto">
                        <div class="d-flex justify-content-center">
                            <button type="button" class="btn btn-primary position-relative px-4 w-100" onclick="runDio('inputText${exJson[i].file}', 'outputText${exJson[i].file}')">Run</button>
                        </div>
                    </div> 
                </div>
            <div class="row">
                <div class="col-12 col-sm-6">
                    <div class="card border border-warning rounded">
                        <div class="card-body">
                            <div class="form-group">
                                <label for="inputText">Code input</label>
                                <textarea class="form-control" id="inputText${exJson[i].file}" rows="15" readonly>${exJson[i].content}</textarea>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="col-12 col-sm-6">
                    <div class="card border border-warning rounded">
                        <div class="card-body">
                            <div class="form-group">
                                <label for="outputText">Code output</label>
                                <textarea class="form-control" id="outputText${exJson[i].file}" rows="15" readonly></textarea>
                            </div>
                        </div>
                    </div>
                </div>
            <div class="row">
                <div class="col-12">
                    <p>
                        <button class="btn btn-primary w-100" type="button" data-bs-toggle="collapse" data-bs-target="#collapseExample-${i}" aria-expanded="false" aria-controls="collapseExample-${i}">
                            Show expected output.
                        </button>
                    </p>
                    <div class="collapse" id="collapseExample-${i}">
                        <div class="card card-body">
                            Output:${exJson[i].output.stdout.replace("\n", "\\n").slice(0,exJson[i].output.stdout.length - 1 )} <br>
                            Should finish whole script: ${exJson[i].output.stderr ? "No": "Yes"}
                        </div>
                    </div>
                </div>
            </div
        `)
    }
}

function generateWiki() {
    //like bro what even
    fetch("combined.json").then(response => {return response.json()}).then(data => {drawWiki(data["wiki"]);})
}

function generateExamples() {
    fetch("combined.json").then(response => {return response.json()}).then(data => {drawExamples(data["examples"]["main"], "Main"); for (var i = 0; i < data["examples"]["side"].length; i++){drawExamples(data["examples"]["Side"][i], "side")}})
}