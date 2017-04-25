#include "include/apiwebindimanager.h"

/* curl -H "Content-Type: application/json" -X POST http://localhost:8624/api/server/stop */
using namespace std;

struct MemoryStruct {
  char *memory;
  size_t size;
};

#include <string.h>
string data; //will hold the url's contents

/***********************************************************************************
*  CallBack function WriteMemoryCallback
***********************************************************************************/
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (unsigned int c = 0; c < size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = static_cast<char*>(realloc(mem->memory, mem->size + realsize + 1));
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

/***********************************************************************************
*  Function removeBrakets
***********************************************************************************/
char* removeBrackets(char* jsonString)
{
    char * newstring = new char[strlen(jsonString) + 1];
    int len = strlen(jsonString);
    int offset = 0;
    int i = 0;

    // Should begin with curle bracket {
    do
    {
        if (jsonString[i] == '{')
        {
            while (i < len)
            {
                newstring[i - offset] = jsonString[i];
                i++;
            }
        } else
        {
            i++;
            offset++;
        }
    } while (i < len);

    // Should finish with curle bracket }
    i = strlen(newstring);
    while ((newstring[i] != '}') && (i >= 0))
    {
        i--;
    }
    newstring[i + 1] = '\0';

    return newstring;
}

/***********************************************************************************
*  Function IndiWebManagerStart
***********************************************************************************/
CURLcode IndiWebManagerStart(indiCmd_e _cmd)
{
    CURL *curl;
    CURLcode res;

    //static const char *postthis= "http://localhost:8624/api/server/stop";
    // http headers to send with request
    struct curl_slist *headers = NULL;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl)
    {
        if (_cmd == INDISTART)
        {
            curl_easy_setopt(curl, CURLOPT_URL, 'http://{HOSTNAME}:{HOSTNAMEPORT}/api/server/start/Simulators');
        } else
        {
            curl_easy_setopt(curl, CURLOPT_URL, 'http://{HOSTNAME}:{HOSTNAMEPORT}/api/server/stop');
        }
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);
        // not follow redirection
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
        // enable progress meter
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        // set content type
        headers = curl_slist_append(headers, "Content-Type: application/json");
        // some servers don't like requests that are made without a user-agent field, so we provide one
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        // Check for errors
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        // always cleanup
        curl_easy_cleanup(curl);
    }
    return res;
}

/***********************************************************************************
*  Function IndiWebManagerStatus
***********************************************************************************/
CURLcode IndiWebManagerStatus(indiStatus_e &_status)
{
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    _status = INDISTATUSERROR;

    // will be grown as needed by the realloc above
    chunk.memory = static_cast<char*>(malloc(1));
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, 'http://{HOSTNAME}:{HOSTNAMEPORT}/api/server/status');
        // not follow redirection
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
        // enable progress meter
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        // send all data to this function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteMemoryCallback);
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
        // we pass our 'chunk' struct to the callback function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        // some servers don't like requests that are made without a user-agent field, so we provide one
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        // Check for errors
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            _status = INDISTATUSERROR;
        }
        else
        {
            // Parse the reply
            json_object *jobj, *temp;

            char *y = new char[data.length() + 1]; // or
            // char y[100];

            strcpy(y, data.c_str());

  //          jobj = json_tokener_parse(removeBrackets(y));
            jobj = json_tokener_parse(removeBrackets(chunk.memory));

            json_object_get_array(jobj);
            // Get the status value
            json_object_object_get_ex(jobj, "status", &temp);
            if (strcmp(json_object_get_string(temp), "True") == 0)
                _status = INDIRUNNING;
                else _status = INDISTOPED;
        }
        // always cleanup
        free(chunk.memory);
        curl_easy_cleanup(curl);
    }
    return res;
}

/***********************************************************************************
*  Function IndiWebManagerGetAutostartProfile
***********************************************************************************/
CURLcode IndiWebManagerGetAutostartProfile(char *_profile)
{
//    CURL *curl;
    CURLcode res = CURLE_OK;

    return res;
}
