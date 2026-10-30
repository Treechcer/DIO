let dio = null

function getArgs(){
    input = document.getElementById("inputText")
    let arrOut = ["-e", input.value];

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

function runDio() {
    dio?.terminate();

    arg = getArgs()

    if (arg[1].trim() == "" || !arg[1]){
        return
    }

    dio = new Worker("worker.js");
    dio.onmessage = ({ data }) => {
        document.getElementById("outputText").value += "DIO>>: " + data.text + "\n";
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

function generateWiki() {
    //like bro what even
    fetch("wiki.json").then(response => {return response.json()}).then(data => {/*console.log(data);*/ drawWiki(data);})
}