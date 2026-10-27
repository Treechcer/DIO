onmessage = (e) => {
    self.Module = {
        arguments: e.data.args,
        print: text => postMessage({ text }),
    };
    importScripts('dio.js');
};