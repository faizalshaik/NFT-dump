#include <errno.h>
#include<pthread.h>
#include "collecting.h"

#include "nf_common.h"
#include "nffile.h"
#include "collector.h"
#include "nfx.h"
#include "nflowcache.h"
#include "nffile.h"
#include "exporter.h"
#include "nfstat.h"
#include "nffile_inline.c"
#include "backendapi.h"


extern int g_nfcapdId;
extern int g_filterId;

int hash_hit = 0; 
int hash_miss = 0;
int hash_skip = 0;

char ConvertBuffer[65536];
extension_map_list_t *extension_map_list;
generic_exporter_t **exporter_list;

#define MAX_KEEP_RECORD  1000
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
static master_record_t g_records[MAX_KEEP_RECORD];
static uint16_t        g_nRecords = 0;


printmap_t printmap[] = {
	{ "raw",		format_file_block_record,  	NULL 			},
	{ "line", 		format_special,      		FORMAT_line 	},
	{ "long", 		format_special, 			FORMAT_long 	},
	{ "extended",	format_special, 			FORMAT_extended	},
	{ "biline", 	format_special,      		FORMAT_biline 	},
	{ "bilong", 	format_special,      		FORMAT_bilong 	},
	{ "pipe", 		flow_record_to_pipe,      	NULL 			},
//	{ "json", 		flow_record_to_json,      	NULL 			},
	{ "csv", 		flow_record_to_csv,      	NULL 			},
	{ "null", 		flow_record_to_null,      	NULL 			},
#ifdef NSEL
	{ "nsel",		format_special, 			FORMAT_nsel		},
	{ "nel",		format_special, 			FORMAT_nel		},
#endif

// add your formats here

// This is always the last line
	{ NULL,			NULL,                       NULL			}
};


void keepRecord(master_record_t* mrd)
{
    int i;
    master_record_t* rd;
    int exist =0;
    pthread_mutex_lock( &g_mutex );

    //find record
    for(i=0; i<g_nRecords; i++)
    {
        rd = &g_records[i];
        if(rd->ip_union._v4.srcaddr != mrd->ip_union._v4.srcaddr) continue;
        if(rd->ip_union._v4.dstaddr != mrd->ip_union._v4.dstaddr) continue;
        if(rd->srcport != mrd->srcport) continue;
        if(rd->dstport != mrd->dstport) continue;
        if(rd->prot != mrd->prot) continue;

        memcpy(&g_records[i], mrd, sizeof(master_record_t));
        exist = 1;
        break;
    }

    if(exist==0)
    {
        if(g_nRecords >= MAX_KEEP_RECORD)
        {
            memmove(g_records, &g_records[1], sizeof(master_record_t) * (MAX_KEEP_RECORD-1));
            g_nRecords = MAX_KEEP_RECORD-1;
        }
        memcpy(&g_records[g_nRecords], mrd, sizeof(master_record_t));
        g_nRecords++;
    }
    
    pthread_mutex_unlock( &g_mutex);
}

int  readRecords(master_record_t* out, int nCapacity)
{
    if(g_nRecords ==0) return 0;

    int nRet  = 0;
    pthread_mutex_lock( &g_mutex );

    if(g_nRecords >= nCapacity)
        nRet = nCapacity;
    else nRet = g_nRecords;

    memcpy(out, g_records, nRet * sizeof(master_record_t));
    g_nRecords -= nRet;

    if(g_nRecords >0)
        memmove(g_records, &g_records[nRet], g_nRecords * sizeof(master_record_t));

    pthread_mutex_unlock( &g_mutex);

    return nRet;
}


enum CntIndices { FLOWS = 0, INPACKETS, INBYTES, OUTPACKETS, OUTBYTES };
void* uploadThreadProc(void *arg)
{
    int nRecs;
    master_record_t  records[10];
    printf("\n uploadThread running\n");

    while(1)
    {
        nRecs = readRecords(records, 10);
        //send to server.
        if(nRecs >0)
            storeFlows(g_nfcapdId, g_filterId, records, nRecs);
        
        if(nRecs < 10)
            usleep(200000);
    }
    return NULL;
}


int init_record_process()
{
	extension_map_list = InitExtensionMaps(NEEDS_EXTENSION_LIST);	
	if ( !InitExporterList() ) {
		return -1;
	}
	if(!Init_FlowTable())
	{
		return -1;
	}
    ParseOutputFormat(FORMAT_line, 0, printmap); 

    //create upload thread

    pthread_t tid;
    pthread_create(&tid, NULL, &uploadThreadProc, NULL);
    return 0;
}

void record_process(filter_condition* cond,  void* buff, int buffLen)
{
    int nProcessedSize = 0;
	common_record_t *record_ptr = (common_record_t *)buff;
    common_record_t *flow_record;
    uint32_t map_id;
    generic_exporter_t *exp_info;
    master_record_t		*master_record;
    char *string;
    FlowTableRecord_t*  flowTbl = NULL;

    while(nProcessedSize < buffLen)
    {
        flowTbl = NULL;
        flow_record = record_ptr;
        switch (record_ptr->type)
        {
            case ExporterInfoRecordType:
                // #ifdef VIEW_LOG
                //     printf("AddExporterInfo\n");
                // #endif
                AddExporterInfo((exporter_info_record_t *)record_ptr);
            break;
			case ExtensionMapType:
                // #ifdef VIEW_LOG
                //     printf("Insert_Extension_Map\n");
                // #endif
				Insert_Extension_Map(extension_map_list, (extension_map_t *)record_ptr);
            break;
            case CommonRecordV0Type: 
                if ( !ConvertCommonV0((void *)flow_record, (common_record_t *)ConvertBuffer) ) {
                    LogError("Corrupt data file. Unable to decode at %s line %d\n", __FILE__, __LINE__);
                    exit(255);

                }
                flow_record = (common_record_t *)ConvertBuffer;
                //dbg_printf("Converted type %u to %u record\n", CommonRecordV0Type, CommonRecordType);
            case CommonRecordType:

                map_id = flow_record->ext_map;
                exp_info = exporter_list[flow_record->exporter_sysid];

                if ( map_id >= MAX_EXTENSION_MAPS ) {
                    printf("Corrupt data file. Extension map id %u too big.\n", map_id);
                    exit(255);
                }
                if ( extension_map_list->slot[map_id] == NULL ) {
                    printf("Corrupt data file. Missing extension map %u. Skip record.\n", map_id);
                    record_ptr = (common_record_t *)((pointer_addr_t)record_ptr + record_ptr->size);
                    continue;
                } 

                master_record = &(extension_map_list->slot[map_id]->master_record);
                //Engine->nfrecord = (uint64_t *)master_record;
                ExpandRecord_v2( flow_record, extension_map_list->slot[map_id], 
                    exp_info ? &(exp_info->info) : NULL, master_record);

                if(filterRecord(cond, flow_record, master_record)==0)
                {
                    flowTbl = AddFlow(flow_record, master_record, extension_map_list->slot[map_id]);
                }
                // if we need to print out this record
                // if(master_record->prot==6)
                // {
                //     format_special(master_record, &string, 0);
                //     if ( string ) {
                //         printf("%s\n", string);
                //     }
                // }

            break;
            case ExporterStatRecordType:
                AddExporterStat((exporter_stats_record_t *)record_ptr);
                break;
            case SamplerInfoRecordype: 
                AddSamplerInfo((sampler_info_record_t *)record_ptr);
            break;

        }

        if(flowTbl!=NULL)
        {
            if(filterRecord_After(cond, flowTbl, master_record)==0)
            {
                master_record->dPkts 		= flowTbl->counter[INPACKETS];
                master_record->dOctets 	= flowTbl->counter[INBYTES];
                master_record->out_pkts 	= flowTbl->counter[OUTPACKETS];
                master_record->out_bytes 	= flowTbl->counter[OUTBYTES];
                master_record->aggr_flows = flowTbl->counter[FLOWS];
                keepRecord(master_record);
            }
        }

        nProcessedSize += record_ptr->size;
        record_ptr = (common_record_t *)((pointer_addr_t)record_ptr + record_ptr->size);
    }
    //PrintFlowTable(format_special, 0, 0, 0, extension_map_list);
}

void test(filter_condition* cond)
{
    uint32_t	is_anonymized;
    time_t 	t_first_flow, t_last_flow;
    static char		Ident[IDENTLEN];    

    nffile_t* nffile_r = OpenFile("/home/nfdump/nfcapd.test", nffile_r);
	if ( !nffile_r ) {
		LogError("GetNextFile() error in %s line %d: %s\n", __FILE__, __LINE__, strerror(errno) );
		return ;
	}

	// preset time window of all processed flows to the stat record in first flow file
	t_first_flow = nffile_r->stat_record->first_seen;
	t_last_flow  = nffile_r->stat_record->last_seen;

	// store infos away for later use
	// although multiple files may be processed, it is assumed that all 
	// have the same settings
	is_anonymized = IP_ANONYMIZED(nffile_r);
	strncpy(Ident, nffile_r->file_header->ident, IDENTLEN);
	Ident[IDENTLEN-1] = '\0';

    while(ReadBlock(nffile_r) != NF_EOF)
    {
        if ( nffile_r->block_header->id == Large_BLOCK_Type ) {
            // skip
            printf("Xstat block skipped ...\n");
            continue;
        }    

        if ( nffile_r->block_header->id != DATA_BLOCK_TYPE_2 ) {
            if ( nffile_r->block_header->id == DATA_BLOCK_TYPE_1 ) {
                LogError("Can't process nfdump 1.5.x block type 1. Add --enable-compat15 to compile compatibility code. Skip block.\n");
            } else {
                LogError("Can't process block type %u. Skip block.\n", nffile_r->block_header->id);
            }
            continue;
        }
        record_process(cond, nffile_r->buff_ptr, nffile_r->block_header->size);
    }

    // int i, totalFlow=0;
    // hash_FlowTable* FlowTable = GetFlowTable();
    // FlowTableRecord_t* r;
    // for ( i=0; i<=FlowTable->IndexMask; i++ ) {
    //     r = FlowTable->bucket[i];
    //     while ( r ) 
    //     {
    //         totalFlow++;
    //         r= r->next;
    //     }
    // }
    //printf("total flows=%d\n", totalFlow);


    PrintFlowTable(format_special, 0, 0, 0, extension_map_list);

}