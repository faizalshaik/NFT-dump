#ifndef COLLECTING_H
#define COLLECTING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filter.h"

#define FORMAT_line "%ts %td %pr %sap -> %dap %pkt %byt %fl"

#define FORMAT_long "%ts %td %pr %sap -> %dap %flg %tos %pkt %byt %fl"

#define FORMAT_extended "%ts %td %pr %sap -> %dap %flg %tos %pkt %byt %pps %bps %bpp %fl"

#define FORMAT_biline "%ts %td %pr %sap <-> %dap %opkt %ipkt %obyt %ibyt %fl"

#define FORMAT_bilong "%ts %td %pr %sap <-> %dap %flg %tos %opkt %ipkt %obyt %ibyt %fl"

#define FORMAT_nsel "%ts %evt %xevt %pr %sap -> %dap %xsap -> %xdap %ibyt %obyt"

#define FORMAT_nel "%ts %nevt %pr %sap -> %dap %nsap -> %ndap"



int init_record_process();
void record_process(filter_condition* cond, void* buff, int buffLen);

void test(filter_condition* cond);

#endif //COLLECTING_H