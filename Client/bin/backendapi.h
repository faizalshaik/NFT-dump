#ifndef BACKENDAPI_H
#define BACKENDAPI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filter_condition.h"

int getFilterCondition(int nfcapd_id, filter_condition* cond);
void storeFlows(int nfcapId, int filterId,master_record_t	*rd, int count);

#endif //BACKENDAPI_H