#ifndef FILTER_CONDITION_H
#define FILTER_CONDITION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nffile.h"

#define VIEW_LOG

typedef struct _icmp_cond{
    int type;
    int code;
}icmp_cond;

typedef struct _subnet_cond{
    ip_addr_t subnet;
    int       mask;
}subnet_cond;



typedef struct _filter_condition{    
    int Id;
    
    //versions
    int versions_count;
    int*           versions;    

    //protos
    int protos_count;
    int*           protos;

    //ips
    int ips_enable;
    int ips_comp;

    int saddrs_count;
    ip_addr_t *    saddrs;

    int daddrs_count;
    ip_addr_t *    daddrs;

    //ports
    int ports_enable;
    int ports_comp;

    int sports_count;
    int*           sports; 

    int dports_count;
    int*           dports;

    //routers
    int routers_enable;
    int routers_comp;

    int routers_count;
    ip_addr_t *    routers;


    //subnets
    int subnets_enable;
    int subnets_comp;

    int src_subnets_count;
    subnet_cond* src_subnets;
    int dst_subnets_count;
    subnet_cond* dst_subnets;

    //as
    int as_enable;
    int as_comp;    

    int src_as_count;
    int*    src_ass;

    int dst_as_count;
    int*    dst_ass;

    //input/output
    int inouts_enable;
    int inouts_comp;

    int inputs_count;
    int*    inputs;

    int outputs_count;
    int*    outputs;


    //icmps
    int icmp_enable;
    int icmp_comp;
    int icmps_count;
    icmp_cond*     icmps;


    //tcpflags
    int tcpflags_enable;
    int tcpflags_comp;
    int tcpflags_count;
    int*     tcpflags;

    //fwdstatus
    int fwdstatus_enable;
    int fwdstatus_comp;
    int fwdstatus_count;
    int*     fwdstatuss;
    


    //options
    int            packets_enable;
    int            packets;
    int            packets_comp; //-1,0, 1

    int            bytes_enable;
    int            bytes;
    int            bytes_comp; 

    int            packets_ps_enable;
    int            packets_ps;
    int            packets_ps_comp;

    int            duration_enable;
    int            duration;
    int            duration_comp;

    int            bps_enable;
    int            bps;
    int            bps_comp;

    int            bpp_enable;
    int            bpp;
    int            bpp_comp;

    int            tos_enable;
    int            tos;
}filter_condition;


#endif //FILTER_CONDITION_H