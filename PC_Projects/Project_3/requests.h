#ifndef _REQUESTS_
#define _REQUESTS_
#include "parson.h"

char *compute_request(char *host, char* auth, char cookie[], JSON_Object *json);

#endif