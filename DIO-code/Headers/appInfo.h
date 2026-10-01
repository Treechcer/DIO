#ifndef APPINFO__H
#define APPINFO__H

typedef struct appInfo{
    char* version;
    char* platform;
    char* appName;
}appInfo;

void initAppInfo();
void getAppInfoOut();

#endif