#include <stdio.h>

#include "../Headers/appInfo.h"

appInfo app = {.appName = "dio", .version = "0.1.0", .platform = "UNKNOWN"};

void initAppInfo(){
    #if defined(_WIN32)
        app.platform = "WINDOWS";
    #elif defined(__linux__)
        app.platform = "LINUX";
    #elif defined(__APPLE__)
        app.platform = "APPLE";
    #elif defined(__FreeBSD__)
        app.platform = "FREEBSD";
    #elif defined(__ANDROID__)
        app.platform = "ANDROID";
    #endif 
}

void getAppInfoOut(){
    printf("---------------------------\n");
    printf("Software: '%s' ver.: %s\nplatform: %s\n", app.appName, app.version, app.platform);
    printf("---------------------------\n");
}