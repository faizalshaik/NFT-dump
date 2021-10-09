#include "backendapi.h"
#include "webservice.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "nf_common.h"


int getFilterCondition(int nfcapd_id, filter_condition* cond)
{
	int  iRet, i;
	char buff[256];
	char jsonData[4096];
	char result[4096];
	json_val_t* root, *data, *data1, *data2;
	root = (json_val_t*)tree_create_structure(0, 1);

	snprintf(buff, 256, "%d", nfcapd_id);
	tree_append(root, (char*)"nfcapd_id", 9, tree_create_data(JSON_INT, buff, strlen(buff)));

	formatJson(root, jsonData, 4096);	
	tree_free(root);

	iRet = doPostJson("getFilterCondition", jsonData, result, 4096);
	if(iRet < 0)
	{
		printf("getting filter condition failed \n");
		return -1;
	}

	//printf("filtercondition= %s\n", result);

	root = parseJson(result);
	if(root==NULL)
	{
		printf("getting filter condition failed json error\n");		
		return -1;
	}

	//parse condition
	//cond
	int code = getJsonInt(root, "status");
	if(code != 200)
	{
		printf("getting filter condition failed =>%s\n", getJsonString1(root, "message", buff));

		tree_free(root);
		return -1;
	}

	
	data = getJsonObject(root, "data");

	#ifdef VIEW_LOG
		printf("//////////Filter Condition///////////\n");
	#endif	

	cond->Id = getJsonInt(data, "Id");

	//Versions
	data1 = getJsonObject(data, "versions");
	if (data1 && data1->type == JSON_ARRAY_BEGIN)
	{
		cond->versions_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tVersions  %d\n", data1->length);
		#endif		
		if(data1->length > 0)
			cond->versions = 	malloc(sizeof(int) * data1->length);

		printf("\t\t");
		for (i = 0; i < data1->length; i++)
		{
			data2 = data1->u.array[i];
			cond->versions[i] = getJsonInt(data2, "version");
			#ifdef VIEW_LOG
				printf("%s(%d),", getJsonString1(data2, "name", buff), cond->versions[i]);
			#endif			
		}
		printf("\n");
	}

	//PROTOS
	data1 = getJsonObject(data, "protos");
	if (data1 && data1->type == JSON_ARRAY_BEGIN)
	{
		cond->protos_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tProtocols  %d\n", data1->length);
		#endif		
		if(data1->length > 0)
			cond->protos = 	malloc(sizeof(int) * data1->length);

		printf("\t\t");
		for (i = 0; i < data1->length; i++)
		{
			data2 = data1->u.array[i];
			cond->protos[i] = getJsonInt(data2, "value");
			#ifdef VIEW_LOG
				printf("%s(%d),", getJsonString1(data2, "name", buff), cond->protos[i]);
			#endif
		}
		printf("\n");
	}

	//SA
	data1 = getJsonObject(data, "saddrs");
	if (data1 && data1->type == JSON_ARRAY_BEGIN)
	{
		cond->saddrs_count = data1->length;		
		#ifdef VIEW_LOG
			printf("\tSource IP Addressed %d\n", data1->length);
		#endif

		if(data1->length > 0)
			cond->saddrs = 	malloc(sizeof(ip_addr_t) * data1->length);

		for (i = 0; i < data1->length; i++)
		{
			data2 = data1->u.array[i];
			#ifdef VIEW_LOG
				printf("\t\t%s\n", data2->u.data);
			#endif
			cond->saddrs[i].ip_union._v4 = htonl(inet_addr(data2->u.data));
		}
	}

	//DA
	data1 = getJsonObject(data, "daddrs");
	if (data1 && data1->type == JSON_ARRAY_BEGIN)
	{
		cond->daddrs_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tDestination IP addresses %d\n", data1->length);
		#endif

		if(data1->length > 0)
			cond->daddrs = 	malloc(sizeof(ip_addr_t) * data1->length);

		for (i = 0; i < data1->length; i++)
		{
			data2 = data1->u.array[i];
			#ifdef VIEW_LOG
				printf("\t\t%s\n", data2->u.data);
			#endif
			cond->daddrs[i].ip_union._v4 = htonl(inet_addr(data2->u.data));
		}
	}

	//SPORTS
	data1 = getJsonObject(data, "sports");
	if (data1)
	{
		cond->sports_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tSource ports  %d\n", data1->length);
		#endif
		if(data1->length > 0)
		{
			cond->sports = 	malloc(sizeof(int) * data1->length);			
			getJsonIntArray(data1,  cond->sports, data1->length);
			printf("\t\t");
			for(i=0; i<cond->sports_count; i++)
				printf("%d,", cond->sports[i]);
			printf("\n");
		}
	}

	//DPORTS
	data1 = getJsonObject(data, "dports");
	if (data1)
	{
		cond->dports_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tDestination ports  %d\n", data1->length);
		#endif
		if(data1->length > 0)
		{
			cond->dports = 	malloc(sizeof(int) * data1->length);
			getJsonIntArray(data1,  cond->dports, data1->length);
			printf("\t\t");
			for(i=0; i<cond->sports_count; i++)
				printf("%d,", cond->sports[i]);
			printf("\n");
		}
	}

	//routers
	data1 = getJsonObject(data, "routers");
	if (data1 && data1->type == JSON_ARRAY_BEGIN)
	{
		cond->routers_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tRouters  %d\n", data1->length);
		#endif		
		if(data1->length > 0)
			cond->routers = 	malloc(sizeof(ip_addr_t) * data1->length);

		for (i = 0; i < data1->length; i++)
		{
			data2 = data1->u.array[i];
			getJsonString1(data2, "ip", buff);
			#ifdef VIEW_LOG
				printf("\t\t%s\n", buff);
			#endif			
			cond->routers[i].ip_union._v4 = htonl(inet_addr(buff));
		}
	}	


	//SRC subnets
	data1 = getJsonObject(data, "src_subnets");
	if (data1 && data1->type == JSON_ARRAY_BEGIN)
	{
		cond->src_subnets_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tSRC Subnets  %d\n", data1->length);
		#endif		
		if(data1->length > 0)
			cond->src_subnets = malloc(sizeof(subnet_cond) * data1->length);

		for (i = 0; i < data1->length; i++)
		{
			data2 = data1->u.array[i];
			getJsonString1(data2, "ip", buff);
			cond->src_subnets[i].subnet.ip_union._v4 = htonl(inet_addr(buff));
			cond->src_subnets[i].mask = getJsonInt(data2, "mask");			
			#ifdef VIEW_LOG
				printf("\t\t%s mask:%d\n", buff, cond->src_subnets[i].mask);
			#endif
		}
	}	

	//dst subnets
	data1 = getJsonObject(data, "dst_subnets");
	if (data1 && data1->type == JSON_ARRAY_BEGIN)
	{
		cond->dst_subnets_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tDST Subnets  %d\n", data1->length);
		#endif		
		if(data1->length > 0)
			cond->dst_subnets = malloc(sizeof(subnet_cond) * data1->length);

		for (i = 0; i < data1->length; i++)
		{
			data2 = data1->u.array[i];
			getJsonString1(data2, "ip", buff);
			cond->dst_subnets[i].subnet.ip_union._v4 = htonl(inet_addr(buff));
			cond->dst_subnets[i].mask = getJsonInt(data2, "mask");			
			#ifdef VIEW_LOG
				printf("\t\t%s mask:%d\n", buff, cond->dst_subnets[i].mask);
			#endif
		}
	}	


	//SRCAS
	data1 = getJsonObject(data, "srcass");
	if (data1)
	{
		cond->src_as_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tSRC ASs  %d\n", data1->length);
		#endif
		if(data1->length > 0)
		{
			cond->src_ass = 	malloc(sizeof(int) * data1->length);			
			getJsonIntArray(data1,  cond->src_ass, data1->length);
			printf("\t\t");
			for(i=0; i<cond->src_as_count; i++)
				printf("%d,", cond->src_ass[i]);
			printf("\n");
		}
	}

	//DSTAS
	data1 = getJsonObject(data, "dstass");
	if (data1)
	{
		cond->dst_as_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tDST ASs  %d\n", data1->length);
		#endif
		if(data1->length > 0)
		{
			cond->dst_ass = 	malloc(sizeof(int) * data1->length);
			getJsonIntArray(data1,  cond->dst_ass, data1->length);
			printf("\t\t");
			for(i=0; i<cond->dst_as_count; i++)
				printf("%d,", cond->dst_ass[i]);
			printf("\n");
		}
	}


	//INPUTS
	data1 = getJsonObject(data, "inputs");
	if (data1)
	{
		cond->inputs_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tInputs  %d\n", data1->length);
		#endif
		if(data1->length > 0)
		{
			cond->inputs = 	malloc(sizeof(int) * data1->length);			
			getJsonIntArray(data1,  cond->inputs, data1->length);
			printf("\t\t");
			for(i=0; i<cond->inputs_count; i++)
				printf("%d,", cond->inputs[i]);
			printf("\n");
		}
	}

	//OUTPUTS
	data1 = getJsonObject(data, "outputs");
	if (data1)
	{
		cond->outputs_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tOutputs  %d\n", data1->length);
		#endif
		if(data1->length > 0)
		{
			cond->outputs = 	malloc(sizeof(int) * data1->length);
			getJsonIntArray(data1,  cond->outputs, data1->length);
			printf("\t\t");
			for(i=0; i<cond->outputs_count; i++)
				printf("%d,", cond->outputs[i]);
			printf("\n");
		}
	}


	//icmps
	data1 = getJsonObject(data, "icmps");
	if (data1 && data1->type == JSON_ARRAY_BEGIN)
	{
		cond->icmps_count = data1->length;
		if(data1->length > 0)
			cond->icmps = 	malloc(sizeof(icmp_cond) * data1->length);

		for (i = 0; i < data1->length; i++)
		{
			data2 = data1->u.array[i];
			cond->icmps[i].type = getJsonInt(data2, "type");
			cond->icmps[i].code = getJsonInt(data2, "code");
		}
	}	

	//tcp flags
	data1 = getJsonObject(data, "tcpflags");
	if (data1 && data1->type == JSON_ARRAY_BEGIN)
	{
		cond->tcpflags_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tTCP Flags  %d\n", data1->length);
		#endif
		if(data1->length > 0)
		{
			cond->tcpflags = 	malloc(sizeof(int) * data1->length);
			getJsonIntArray(data1,  cond->tcpflags, data1->length);
			printf("\t\t");
			for(i=0; i<cond->tcpflags_count; i++)
				printf("%d,", cond->tcpflags[i]);
			printf("\n");
		}
	}	

	//fwd status
	data1 = getJsonObject(data, "fwdstatus");
	if (data1 && data1->type == JSON_ARRAY_BEGIN)
	{
		cond->fwdstatus_count = data1->length;
		#ifdef VIEW_LOG
			printf("\tFwd Status  %d\n", data1->length);
		#endif
		if(data1->length > 0)
		{
			cond->fwdstatuss = 	malloc(sizeof(int) * data1->length);
			getJsonIntArray(data1,  cond->fwdstatuss, data1->length);
			printf("\t\t");
			for(i=0; i<cond->fwdstatus_count; i++)
				printf("%d,", cond->fwdstatuss[i]);
			printf("\n");
		}
	}	


	printf("\tOptions\n");
	//Options
	data1 = getJsonObject(data, "options");
	if (data1)
	{
		cond->ips_enable = getJsonBool(data1, "ips_enable");
		cond->ips_comp = getJsonBoolAs(data1, "ips_comp", "AND");
		printf("\t\tIP filter"); 
		if(cond->ips_enable) printf("\tTRUE   Compare="); 
		else printf("\tFALSE   Compare=");
		if(cond->ips_comp) printf("AND\n"); 
		else printf("OR\n"); 

		cond->ports_enable = getJsonBool(data1, "ports_enable");
		cond->ports_comp = getJsonBoolAs(data1, "ports_comp", "AND");
		printf("\t\tPort filter"); 
		if(cond->ports_enable) printf("\tTRUE   Compare="); 
		else printf("\tFALSE   Compare=");
		if(cond->ports_comp) printf("AND\n"); 
		else printf("OR\n"); 


		cond->routers_enable = getJsonBool(data1, "routers_enable");
		cond->routers_comp = getJsonBoolAs(data1, "routers_comp", "AND");
		printf("\t\tRouter filter"); 
		if(cond->routers_enable) printf("\tTRUE   Compare="); 
		else printf("\tFALSE   Compare=");
		if(cond->routers_comp) printf("AND\n"); 
		else printf("OR\n"); 

		cond->subnets_enable = getJsonBool(data1, "subnets_enable");
		cond->subnets_comp = getJsonBoolAs(data1, "subnets_comp", "AND");
		printf("\t\tSubnet filter"); 
		if(cond->subnets_enable) printf("\tTRUE   Compare="); 
		else printf("\tFALSE   Compare=");
		if(cond->subnets_comp) printf("AND\n"); 
		else printf("OR\n");
		

		cond->as_enable = getJsonBool(data1, "as_enable");
		cond->as_comp = getJsonBoolAs(data1, "as_comp", "AND");
		printf("\t\tAS filter"); 
		if(cond->as_enable) printf("\tTRUE   Compare="); 
		else printf("\tFALSE   Compare=");
		if(cond->as_comp) printf("AND\n"); 
		else printf("OR\n");

		cond->inouts_enable = getJsonBool(data1, "inouts_enable");
		cond->inouts_comp = getJsonBoolAs(data1, "inouts_comp", "AND");
		printf("\t\tIn/Out filter"); 
		if(cond->inouts_enable) printf("\tTRUE   Compare="); 
		else printf("\tFALSE   Compare=");
		if(cond->inouts_comp) printf("AND\n"); 
		else printf("OR\n");		

		cond->icmp_enable = getJsonBool(data1, "icmp_enable");
		cond->icmp_comp = getJsonBoolAs(data1, "icmp_comp", "AND");
		printf("\t\tICMP filter"); 
		if(cond->icmp_enable) printf("\tTRUE   Compare="); 
		else printf("\tFALSE   Compare=");
		if(cond->icmp_comp) printf("AND\n"); 
		else printf("OR\n");

		cond->tcpflags_enable = getJsonBool(data1, "tcpflags_enable");
		cond->tcpflags_comp = getJsonBoolAs(data1, "tcpflags_comp", "AND");
		printf("\t\tTCP Flags filter"); 
		if(cond->tcpflags_enable) printf("\tTRUE   Compare="); 
		else printf("\tFALSE   Compare=");
		if(cond->tcpflags_comp) printf("AND\n"); 
		else printf("OR\n");

		cond->fwdstatus_enable = getJsonBool(data1, "fwdstatus_enable");
		cond->fwdstatus_comp = getJsonBoolAs(data1, "fwdstatus_comp", "AND");
		printf("\t\tFWD Status filter"); 
		if(cond->fwdstatus_enable) printf("\tTRUE   Compare="); 
		else printf("\tFALSE   Compare=");
		if(cond->fwdstatus_comp) printf("AND\n"); 
		else printf("OR\n");


		//packets 
		cond->packets_enable = getJsonBool(data1, "packets_enable");
		cond->packets = getJsonInt(data1, "packets");
		getJsonString1(data1, "packets_comp", buff);
		if(strcmp(buff, "LT")) cond->packets_comp = -1;
		else if(strcmp(buff, "GT")) cond->packets_comp = 1;
		else cond->packets_comp = 0;

		//bytes 
		cond->bytes_enable = getJsonBool(data1, "bytes_enable");
		cond->bytes = getJsonInt(data1, "bytes");
		getJsonString1(data1, "bytes_comp", buff);
		if(strcmp(buff, "LT")) cond->bytes_comp = -1;
		else if(strcmp(buff, "GT")) cond->bytes_comp = 1;
		else cond->bytes_comp = 0;

		//packets_ps 
		cond->packets_ps_enable = getJsonBool(data1, "packets_ps_enable");
		cond->packets_ps = getJsonInt(data1, "packets_ps");
		getJsonString1(data1, "packets_ps_comp", buff);
		if(strcmp(buff, "LT")) cond->packets_ps_comp = -1;
		else if(strcmp(buff, "GT")) cond->packets_ps_comp = 1;
		else cond->packets_ps_comp = 0;

		//duration 
		cond->duration_enable = getJsonBool(data1, "duration_enable");
		cond->duration = getJsonInt(data1, "duration");
		getJsonString1(data1, "duration_comp", buff);
		if(strcmp(buff, "LT")) cond->duration_comp = -1;
		else if(strcmp(buff, "GT")) cond->duration_comp = 1;
		else cond->duration_comp = 0;

		//bps
		cond->bps_enable = getJsonBool(data1, "bps_enable");
		cond->bps = getJsonInt(data1, "bps");
		getJsonString1(data1, "bps_comp", buff);
		if(strcmp(buff, "LT")) cond->bps_comp = -1;
		else if(strcmp(buff, "GT")) cond->bps_comp = 1;
		else cond->bps_comp = 0;

		//bpp
		cond->bpp_enable = getJsonBool(data1, "bpp_enable");
		cond->bpp = getJsonInt(data1, "bpp");
		getJsonString1(data1, "bpp_comp", buff);
		if(strcmp(buff, "LT")) cond->bpp_comp = -1;
		else if(strcmp(buff, "GT")) cond->bpp_comp = 1;
		else cond->bpp_comp = 0;

		//tos
		cond->tos_enable = getJsonBool(data1, "tos_enable");
		cond->tos = getJsonInt(data1, "tos");
	}
	#ifdef VIEW_LOG
		printf("************Filter Condition*******\n");
	#endif	


	tree_free(root);
	return 0;

}

char* make_ip_string(uint32_t addr, char* buff)
{
	uint32_t	ip;
	ip = htonl(addr);
	inet_ntop(AF_INET, &ip, buff, 256);

	uint8_t* data=(uint8_t*)&addr;
	sprintf(buff, "%d.%d.%d.%d", data[3], data[2], data[1], data[0]);
	return buff;
}


json_val_t* recordToJson(int nfcapdId, int filterId, master_record_t	*rd)
{
	json_val_t* root, *record;
	char buff[256];

	root = (json_val_t*)tree_create_structure(0, 1);
	sprintf(buff, "%d", nfcapdId);
	tree_append(root, (char*)"nfcapd_id", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", filterId);
	tree_append(root, (char*)"filter_id", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	record = (json_val_t*)tree_create_structure(0, 1);
	sprintf(buff, "%d", rd->type);	tree_append(record, (char*)"type", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->size);	tree_append(record, (char*)"size", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->flags);	tree_append(record, (char*)"flags", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	sprintf(buff, "%d", rd->msec_first); tree_append(record, (char*)"msec_first", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->msec_last);	tree_append(record, (char*)"msec_last", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	sprintf(buff, "%d", rd->first);	tree_append(record, (char*)"first", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->last);	tree_append(record, (char*)"last", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->fwd_status);	tree_append(record, (char*)"fwd_status", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	sprintf(buff, "%d", rd->tcp_flags);	tree_append(record, (char*)"tcp_flags", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->prot);	tree_append(record, (char*)"prot", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->tos);	tree_append(record, (char*)"tos", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->srcport);	tree_append(record, (char*)"srcport", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->dstport);	tree_append(record, (char*)"dstport", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	
	make_ip_string(rd->ip_union._v4.srcaddr, buff);
	tree_append(record, (char*)"srcaddr", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	make_ip_string(rd->ip_union._v4.dstaddr, buff);
	tree_append(record, (char*)"dstaddr", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));


	sprintf(buff, "%d", rd->dPkts);	tree_append(record, (char*)"dPkts", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->dOctets);	tree_append(record, (char*)"dOctets", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	//sprintf(buff, "%d", rd->ip_nexthop);	tree_append(record, (char*)"ip_nexthop", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->bgp_nexthop);	tree_append(record, (char*)"bgp_nexthop", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	//sprintf(buff, "%d", rd->dst_tos);	tree_append(record, (char*)"dst_tos", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->dir);	tree_append(record, (char*)"dir", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->src_mask);	tree_append(record, (char*)"src_mask", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->dst_mask);	tree_append(record, (char*)"dst_mask", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	//sprintf(buff, "%d", rd->src_vlan);	tree_append(record, (char*)"src_vlan", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->dst_vlan);	tree_append(record, (char*)"dst_vlan", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	sprintf(buff, "%d", rd->out_pkts);	tree_append(record, (char*)"out_pkts", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	sprintf(buff, "%d", rd->out_bytes);	tree_append(record, (char*)"out_bytes", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->aggr_flows);	tree_append(record, (char*)"aggr_flows", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->in_src_mac);	tree_append(record, (char*)"in_src_mac", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->out_dst_mac);	tree_append(record, (char*)"out_dst_mac", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->mpls_label);	tree_append(record, (char*)"mpls_label", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));


	make_ip_string(rd->ip_router.ip_union._v4, buff);
	tree_append(record, (char*)"ip_router", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->fill);	tree_append(record, (char*)"fill", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->engine_type);	tree_append(record, (char*)"engine_type", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->engine_id);	tree_append(record, (char*)"engine_id", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->bgpNextAdjacentAS);	tree_append(record, (char*)"bgpNextAdjacentAS", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->bgpPrevAdjacentAS);	tree_append(record, (char*)"bgpPrevAdjacentAS", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));

	//sprintf(buff, "%d", rd->client_nw_delay_usec);	tree_append(record, (char*)"client_nw_delay_usec", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->server_nw_delay_usec);	tree_append(record, (char*)"server_nw_delay_usec", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	//sprintf(buff, "%d", rd->appl_latency_usec);	tree_append(record, (char*)"appl_latency_usec", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));



	sprintf(buff, "%d", rd->srcas);	tree_append(record, (char*)"srcas", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));	
	sprintf(buff, "%d", rd->dstas);	tree_append(record, (char*)"dstas", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));		
	sprintf(buff, "%d", rd->input);	tree_append(record, (char*)"input", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));		
	sprintf(buff, "%d", rd->output);	tree_append(record, (char*)"output", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));


	sprintf(buff, "%d", rd->received);	tree_append(record, (char*)"received", 0, tree_create_data(JSON_STRING, buff, strlen(buff)));
	tree_append(root, (char*)"record", 0, record);
	return root;
}


void storeFlows(int nfcapId, int filterId, master_record_t	*rd, int count)
{
	char buff[4096];
	char jsonData[8192];
	int i;

	if(count ==0) return;
	json_val_t* root = (json_val_t*)tree_create_structure(0, 0);
	for(i=0; i<count; i++)
	{
		json_val_t* entry = recordToJson(nfcapId, filterId, &rd[i]);
		tree_append(root, NULL, 0, entry);
	}

	formatJson(root, jsonData, 8192);

	printf(jsonData);
	printf("\n");

	tree_free(root);
	int iRet = doPostJson("store_flow_records", jsonData, buff, 4096);
	printf(buff);
	printf("\n");
}