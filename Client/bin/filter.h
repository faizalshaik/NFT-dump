#ifndef FILTER_H
#define FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filter_condition.h"
#include "nflowcache.h"

int filterRecord(filter_condition* cond, common_record_t* rd, master_record_t* mrd);
int filterRecord_After(filter_condition* cond, FlowTableRecord_t*  flowTbl, master_record_t* mrd);


#endif //FILTER_H