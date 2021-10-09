#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"

json_val_t* postJson(const char* path, const char* jsonData);
int doPostJson(const char* path, const char* jsonData, char* result, int resultLen);


#endif //WEBSERVICE_H