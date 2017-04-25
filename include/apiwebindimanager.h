#ifndef APIWEBINDIMANAGER_H
#define APIWEBINDIMANAGER_H

#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include <json/json.h>

#define HOSTNAME "localhost"
#define HOSTNAMEPORT 8624

enum indiCmd_e {INDISTART, INDISTOP};
enum indiStatus_e {INDISTOPED, INDIRUNNING, INDISTATUSERROR};

CURLcode IndiWebManagerStart(indiCmd_e _cmd);
CURLcode IndiWebManagerStatus(indiStatus_e &_status);
CURLcode IndiWebManagerGetAutostartProfile(char *_profile);


#endif // APIWEBINDIMANAGER_H
