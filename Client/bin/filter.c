#include "filter.h"
#include <netinet/in.h>

int filterIPs(filter_condition* cond, master_record_t* mrd)
{
	int i;
	int matchedSAddr = 0;

	//source addr check
	if(cond->saddrs_count > 0)
	{
		for(i=0; i<cond->saddrs_count; i++)
		{
			// if(mrd->ip_union._v6.srcaddr[0] == cond->saddrs[i].ip_union._v6[0] && 
			// 	mrd->ip_union._v6.srcaddr[1] == cond->saddrs[i].ip_union._v6[1])
			// {
			// 	matchedSAddr = 1;
			// 	break;
			// }
			if(mrd->ip_union._v4.srcaddr == cond->saddrs[i].ip_union._v4)
			{
				matchedSAddr = 1;
				break;
			}			
		}
	}
	else 
		matchedSAddr = 1;		
	if(matchedSAddr==0) return -1;

	//dest addr check
	if(cond->daddrs_count > 0)
	{
		for(i=0; i<cond->daddrs_count; i++)
		{
			// if(mrd->ip_union._v6.dstaddr[0] == cond->daddrs[i].ip_union._v6[0] && 
			// 	mrd->ip_union._v6.dstaddr[1] == cond->daddrs[i].ip_union._v6[1])
			// {
			// 	return 0;
			// }
			if(mrd->ip_union._v4.dstaddr == cond->daddrs[i].ip_union._v4)
			{
				matchedSAddr = 1;
				break;
			}			
		}
		return -1;
	}
	return 0;
}


int filterSubNets(filter_condition* cond, master_record_t* mrd)
{
	int i;
	int matchedSAddr = 0;

	//source addr check
	if(cond->src_subnets_count > 0)
	{
		for(i=0; i<cond->src_subnets_count; i++)
		{
			// if(mrd->ip_union._v6.srcaddr[0] == cond->saddrs[i].ip_union._v6[0] && 
			// 	mrd->ip_union._v6.srcaddr[1] == cond->saddrs[i].ip_union._v6[1])
			// {
			// 	matchedSAddr = 1;
			// 	break;
			// }
			uint32_t mask = 0xFFFFFFFF <<  cond->src_subnets[i].mask;
			if( (mrd->ip_union._v4.srcaddr & mask) == (cond->src_subnets[i].subnet.ip_union._v4 & mask))
			{
				matchedSAddr = 1;
				break;
			}			
		}
	}
	else 
		matchedSAddr = 1;		
	if(matchedSAddr==0) return -1;

	//dest addr check
	if(cond->dst_subnets_count > 0)
	{
		for(i=0; i<cond->dst_subnets_count; i++)
		{
			// if(mrd->ip_union._v6.dstaddr[0] == cond->daddrs[i].ip_union._v6[0] && 
			// 	mrd->ip_union._v6.dstaddr[1] == cond->daddrs[i].ip_union._v6[1])
			// {
			// 	return 0;
			// }
			uint32_t mask = 0xFFFFFFFF <<  cond->dst_subnets[i].mask;
			if( (mrd->ip_union._v4.dstaddr & mask) == (cond->dst_subnets[i].subnet.ip_union._v4 & mask))
			{
				matchedSAddr = 1;
				break;
			}			
		}
		return -1;
	}
	return 0;
}


int filterRouters(filter_condition* cond, master_record_t* mrd)
{
	int i;
	if(cond->routers_count > 0)
	{
		for(i=0; i<cond->routers_count; i++)
		{
			if(mrd->ip_router.ip_union._v6[0] == cond->routers[i].ip_union._v6[0] && 
				mrd->ip_router.ip_union._v6[1] == cond->routers[i].ip_union._v6[1])
			return 0;
		}
		return -1;
	}
	return 0;
}

int filterPorts(filter_condition* cond, master_record_t* mrd)
{
	int i, matchedSrc = 0;
	if(cond->sports_count > 0)
	{
		for(i=0; i<cond->sports_count; i++)
		{
			if(mrd->srcport == cond->sports[i])
			{
				matchedSrc = 1;
				break;
			}
		}
	}else matchedSrc = 1;

	if(matchedSrc ==0) return -1;

	if(cond->dports_count > 0)
	{
		for(i=0; i<cond->sports_count; i++)
		{
			if(mrd->dstport == cond->dports[i])
			{
				return 0;
			}
		}
		return -1;
	}
	return 0;
}

int filterASs(filter_condition* cond, master_record_t* mrd)
{
	int i, matchedSrc = 0;
	if(cond->src_as_count > 0)
	{
		for(i=0; i<cond->src_as_count; i++)
		{
			if(mrd->srcas == cond->src_ass[i])
			{
				matchedSrc = 1;
				break;
			}
		}
	}else matchedSrc = 1;

	if(matchedSrc ==0) return -1;

	if(cond->dst_as_count > 0)
	{
		for(i=0; i<cond->dst_as_count; i++)
		{
			if(mrd->dstas == cond->dst_ass[i])
			{
				return 0;
			}
		}
		return -1;
	}
	return 0;
}

int filterInOuts(filter_condition* cond, master_record_t* mrd)
{
	int i, matchedSrc = 0;
	if(cond->inputs_count > 0)
	{
		for(i=0; i<cond->inputs_count; i++)
		{
			if(mrd->input == cond->inputs[i])
			{
				matchedSrc = 1;
				break;
			}
		}
	}else matchedSrc = 1;

	if(matchedSrc ==0) return -1;

	if(cond->outputs_count > 0)
	{
		for(i=0; i<cond->outputs_count; i++)
		{
			if(mrd->output == cond->outputs[i])
			{
				return 0;
			}
		}
		return -1;
	}
	return 0;
}



int filterProtos(filter_condition* cond, master_record_t* mrd)
{
	int i;
	if(cond->protos_count > 0)
	{
		for(i=0; i<cond->protos_count; i++ )
		{
			if(cond->protos[i] == mrd->prot)
				return 0;
		}
		return -1;
	}
	return 0;
}

int filterVersions(filter_condition* cond, common_record_t* rd, master_record_t* mrd)
{
		
	return 0;
}


int filterIcmps(filter_condition* cond, master_record_t* mrd)
{
	int i;
	if(cond->icmps_count >0)
	{
		for(i=0 ;i<cond->icmps_count; i++)
		{
			if(mrd->icmp_type == cond->icmps[i].type)
			{
				if(cond->icmps[i].code == 0) return 0;
				if(cond->icmps[i].code == mrd->icmp_code) return 0;
			}
		}
		return -1;
	}
	return 0;
}

int filterTcpFlags(filter_condition* cond, master_record_t* mrd)
{
	int i;
	if(cond->tcpflags_count >0)
	{
		for(i=0 ;i<cond->tcpflags_count; i++)
		{
			if(mrd->tcp_flags & cond->tcpflags[i])
			{
				return 0;
			}
		}
		return -1;
	}
	return 0;
}

int filterForwardStatus(filter_condition* cond, master_record_t* mrd)
{
	int i;
	if(cond->fwdstatus_count >0)
	{
		for(i=0 ;i<cond->fwdstatus_count; i++)
		{
			if(mrd->fwd_status == cond->fwdstatuss[i])
			{
				return 0;
			}
		}
		return -1;
	}
	return 0;
}


int filterRecord(filter_condition* cond, common_record_t* rd, master_record_t* mrd)
{

	if(filterVersions(cond, rd, mrd) < 0) return -1;
	if(filterProtos(cond, mrd) < 0) return -1;

	//IP filter
	if(cond->ips_enable)
	{
		if(filterIPs(cond, mrd) < 0)
		{
			if(cond->ips_comp ==1)	//AND
				return -1;
		}
		else if((cond->saddrs_count || cond->daddrs_count) && cond->ips_comp ==0)
			return 0;
	}

	//Ports filter
	if(cond->ports_enable && (mrd->prot == IPPROTO_TCP || mrd->prot == IPPROTO_UDP))
	{
		if(filterPorts(cond, mrd) < 0) 
		{
			if(cond->ports_comp ==1)	//AND
				return -1;
		}else if((cond->sports_count || cond->dports_count) && cond->ports_comp ==0)
			return 0;
	}	
	else if(mrd->prot == IPPROTO_ICMP && cond->icmp_enable)
	{
		if(filterIcmps(cond, mrd) < 0) return -1;
	}


	//Router filter
	if(cond->routers_enable)
	{
		if(filterRouters(cond, mrd) < 0)
		{
			if(cond->routers_comp==1) return -1;
		}else if(cond->routers_count && cond->routers_comp ==0)
			return 0;
	}

	//sub net filter
	if(cond->subnets_enable)
	{
		if(filterSubNets(cond, mrd) < 0)
		{
			if(cond->subnets_comp ==1)	//AND
				return -1;
		}
		else if((cond->src_subnets_count || cond->dst_subnets_count) && cond->subnets_comp ==0)
			return 0;
	}

	//AS filter
	if(cond->as_enable)
	{
		if(filterASs(cond, mrd) < 0) 
		{
			if(cond->as_comp ==1)	//AND
				return -1;
		}else if((cond->src_as_count || cond->dst_as_count) && cond->as_comp ==0)
			return 0;
	}	


	//Input/Output filter
	if(cond->inouts_enable)
	{
		if(filterASs(cond, mrd) < 0) 
		{
			if(cond->inouts_comp ==1)	//AND
				return -1;
		}else if((cond->inputs_count || cond->outputs_count) && cond->inouts_comp ==0)
			return 0;
	}	

	//TCP flags filter
	if(cond->tcpflags_enable && mrd->prot==6) //TCP
	{
		if(filterTcpFlags(cond,mrd) < 0)
		{
			if(cond->tcpflags_comp==1) //AND
				return -1;
		}else if(cond->tcpflags_count && cond->tcpflags_comp==0)
			return 0;
	}

	//FWD status
	if(cond->fwdstatus_enable)
	{
		if(filterForwardStatus(cond, mrd) <0)
		{
			if(cond->fwdstatus_comp==1) //and
				return -1;				
		}else if(cond->fwdstatus_count && cond->fwdstatus_comp==0)
			return 0;
	}
	
	//TOS filter
	if(cond->tos_enable)
	{
		if(mrd->tos != cond->tos) return -1;
	}

	return 0;
}


enum CNT_IND { FLOWS = 0, INPACKETS, INBYTES, OUTPACKETS, OUTBYTES };
int filterRecord_After(filter_condition* cond, FlowTableRecord_t*  flowTbl, master_record_t* mrd)
{
	uint32_t secs, pktsIn, pktsOut, bytesIn, bytesOut;
	if(cond->duration_enable)
	{
		secs = flowTbl->flowrecord.last - flowTbl->flowrecord.first;
		if(cond->duration_comp ==-1)
		{
			if(secs >= cond->duration) return -1;
		}else if(cond->duration_comp == 0)
		{
			if(secs != cond->duration) return -1;
		}
		else if(cond->duration_comp == 1)
		{
			if(secs <= cond->duration) return -1;
		}
	}

	if(cond->packets_enable)
	{
		pktsIn = flowTbl->counter[INPACKETS];
		pktsOut = flowTbl->counter[OUTPACKETS];

		if(cond->packets_comp==-1)
		{
			if(pktsIn >= cond->packets || pktsOut >= cond->packets) return -1;
		}else if(cond->packets_comp==0)
		{
			if(pktsIn != cond->packets && pktsOut != cond->packets) return -1;
		}else if(cond->packets_comp==1)
		{
			if(pktsIn <= cond->packets || pktsOut <= cond->packets) return -1;
		}
	}

	if(cond->bytes_enable)
	{
		bytesIn =  flowTbl->counter[INBYTES];
		bytesOut = flowTbl->counter[OUTBYTES];
		if(cond->bytes_comp==-1)
		{
			if(bytesIn >= cond->bytes || bytesOut >= cond->bytes) return -1;
		}else if(cond->bytes_comp==0)
		{
			if(bytesIn != cond->bytes && bytesOut != cond->bytes) return -1;
		}else if(cond->bytes_comp==1)
		{
			if(bytesIn <= cond->bytes || bytesOut <= cond->bytes) return -1;
		}
	}

	return 0;
}


