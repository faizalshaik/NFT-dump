/*
 *  Copyright (c) 2017, Peter Haag
 *  Copyright (c) 2014, Peter Haag
 *  Copyright (c) 2009, Peter Haag
 *  Copyright (c) 2004-2008, SWITCH - Teleinformatikdienste fuer Lehre und Forschung
 *  All rights reserved.
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met:
 *  
 *   * Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice, 
 *     this list of conditions and the following disclaimer in the documentation 
 *     and/or other materials provided with the distribution.
 *   * Neither the name of the author nor the names of its contributors may be 
 *     used to endorse or promote products derived from this software without 
 *     specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE.
 *  
 */

#ifndef _NF_COMMON_H
#define _NF_COMMON_H 1

#include "config.h"

#include <sys/types.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include <time.h>
#include "nffile.h"

typedef void (*printer_t)(void *, char **, int);

#if ( SIZEOF_VOID_P == 8 )
typedef uint64_t	pointer_addr_t;
#else
typedef uint32_t	pointer_addr_t;
#endif

typedef struct msec_time_s {
	time_t		sec;
	uint16_t	msec;
} msec_time_tt;

/* common minimum netflow header for all versions */
typedef struct common_flow_header {
  uint16_t  version;
  uint16_t  count;
} common_flow_header_t;

typedef struct printmap_s {
	char		*printmode;		// name of the output format
	printer_t	func;			// name of the function, which prints the record
	char		*Format;		// output format definition
} printmap_t;

#define NSEL_EVENT_IGNORE 0LL
#define NSEL_EVENT_CREATE 1LL
#define NSEL_EVENT_DELETE 2LL
#define NSEL_EVENT_DENIED 3LL
#define NSEL_EVENT_ALERT  4LL
#define NSEL_EVENT_UPDATE 5LL

#define NEL_EVENT_INVALID 0LL
#define NEL_EVENT_ADD	  1LL
#define NEL_EVENT_DELETE  2LL

/* prototypes */

int InitSymbols(void);

void Setv6Mode(int mode);

int Getv6Mode(void);

int Proto_num(char *protostr);

void format_file_block_header(void *header, char **s, int tag);

char *format_csv_header(void);

char *get_record_header(void);

void set_record_header(void);

void format_file_block_record(void *record, char **s, int tag);

void flow_record_to_pipe(void *record, char ** s, int tag);

void flow_record_to_csv(void *record, char ** s, int tag);

void flow_record_to_null(void *record, char ** s, int tag);

int ParseOutputFormat(char *format, int plain_numbers, printmap_t *printmap);

void format_special(void *record, char ** s, int tag);


uint32_t Get_fwd_status_id(char *status);

char *Get_fwd_status_name(uint32_t id);

void Proto_string(uint8_t protonum, char *protostr);

void condense_v6(char *s);


/* prototypes */
// static inline void ICMP_Port_decode(master_record_t *r, char *string);

// static void InitFormatParser(void);

// static void AddToken(int index);

// static void AddString(char *string);

// static void String_FlowFlags(master_record_t *r, char *string);

// static void String_FirstSeen(master_record_t *r, char *string);

// static void String_LastSeen(master_record_t *r, char *string);

// static void String_Received(master_record_t *r, char *string);

// static void String_FirstSeenRaw(master_record_t *r, char *string);

// static void String_LastSeenRaw(master_record_t *r, char *string);

// static void String_ReceivedRaw(master_record_t *r, char *string);

// static void String_Duration(master_record_t *r, char *string);

// static void String_Protocol(master_record_t *r, char *string);

// static void String_SrcAddr(master_record_t *r, char *string);

// static void String_DstAddr(master_record_t *r, char *string);

// static void String_SrcAddrPort(master_record_t *r, char *string);

// static void String_DstAddrPort(master_record_t *r, char *string);

// static void String_SrcNet(master_record_t *r, char *string);

// static void String_DstNet(master_record_t *r, char *string);

// static void String_NextHop(master_record_t *r, char *string);

// static void String_BGPNextHop(master_record_t *r, char *string);

// static void String_RouterIP(master_record_t *r, char *string);

// static void String_SrcPort(master_record_t *r, char *string);

// static void String_DstPort(master_record_t *r, char *string);

// static void String_ICMP_code(master_record_t *r, char *string);

// static void String_ICMP_type(master_record_t *r, char *string);
 
// static void String_SrcAS(master_record_t *r, char *string);

// static void String_DstAS(master_record_t *r, char *string);

// static void String_NextAS(master_record_t *r, char *string);

// static void String_PrevAS(master_record_t *r, char *string);

// static void String_Input(master_record_t *r, char *string);

// static void String_Output(master_record_t *r, char *string);

// static void String_InPackets(master_record_t *r, char *string);

// static void String_OutPackets(master_record_t *r, char *string);

// static void String_InBytes(master_record_t *r, char *string);

// static void String_OutBytes(master_record_t *r, char *string);

// static void String_Flows(master_record_t *r, char *string);

// static void String_Tos(master_record_t *r, char *string);

// static void String_Dir(master_record_t *r, char *string);

// static void String_SrcTos(master_record_t *r, char *string);

// static void String_DstTos(master_record_t *r, char *string);

// static void String_SrcMask(master_record_t *r, char *string);

// static void String_DstMask(master_record_t *r, char *string);

// static void String_SrcVlan(master_record_t *r, char *string);

// static void String_DstVlan(master_record_t *r, char *string);

// static void String_FwdStatus(master_record_t *r, char *string);

// static void String_Flags(master_record_t *r, char *string);

// static void String_InSrcMac(master_record_t *r, char *string);

// static void String_OutDstMac(master_record_t *r, char *string);

// static void String_InDstMac(master_record_t *r, char *string);

// static void String_OutSrcMac(master_record_t *r, char *string);

// static void String_MPLS_1(master_record_t *r, char *string);

// static void String_MPLS_2(master_record_t *r, char *string);

// static void String_MPLS_3(master_record_t *r, char *string);

// static void String_MPLS_4(master_record_t *r, char *string);

// static void String_MPLS_5(master_record_t *r, char *string);

// static void String_MPLS_6(master_record_t *r, char *string);

// static void String_MPLS_7(master_record_t *r, char *string);

// static void String_MPLS_8(master_record_t *r, char *string);

// static void String_MPLS_9(master_record_t *r, char *string);

// static void String_MPLS_10(master_record_t *r, char *string);

// static void String_MPLSs(master_record_t *r, char *string);

// static void String_Engine(master_record_t *r, char *string);

// static void String_Label(master_record_t *r, char *string);

// static void String_ClientLatency(master_record_t *r, char *string);

// static void String_ServerLatency(master_record_t *r, char *string);

// static void String_AppLatency(master_record_t *r, char *string);

// static void String_bps(master_record_t *r, char *string);

// static void String_pps(master_record_t *r, char *string);

// static void String_bpp(master_record_t *r, char *string);

// static void String_ExpSysID(master_record_t *r, char *string);



#define TAG_CHAR ''

#endif //_NF_COMMON_H

