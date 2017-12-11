#ifndef __PBSTRU_COMM_H__
#define __PBSTRU_COMM_H__

#include <stdlib.h>
#include <memory.h>

#define WIRE_TYPE_VARINT 0
#define WIRE_TYPE_FIX64 1
#define WIRE_TYPE_LENGTH_DELIMITED 2
#define WIRE_TYPE_FIX32 5

#ifdef _TEST_CODEC
typedef unsigned char BYTE;
typedef unsigned char *PBYTE;
typedef char CHAR;
typedef unsigned short WORD;
typedef unsigned short WORD16;
typedef unsigned long DWORD;
typedef unsigned long WORD32;
typedef long SWORD32;
typedef char* LPSTR;
typedef const char* LPCSTR;
typedef unsigned long long WORD64;

#ifndef BOOL
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif

#else
#include "tulip.h"

#endif
typedef struct {
	char *data;
	size_t length;
} ps_string;

typedef struct {
	unsigned char *data;
	size_t length;
} ps_bytes;

void encode_tag_byte(BYTE *buf, WORD tag, BYTE wire_type, size_t *offset);
void parse_tag_byte(const BYTE* buf, WORD *field_num, BYTE *wire_type, size_t *offset);
void deal_unknown_field(const BYTE wire_type, const BYTE* buf, size_t* offset);

void *pbstru_malloc(size_t size);
void pbstru_free(void *buf);

#define encode_varint(len, buf, offset) do{ \
	unsigned long long remain_len = len; \
	size_t iloop; \
	for (iloop = 0;; ++iloop) { \
		if ((remain_len >> 7) > 0) { \
			if (NULL != buf) { \
				buf[(*(offset)) + iloop] = ((BYTE)remain_len) | 0x80; \
			} \
			remain_len = remain_len >> 7; \
		} \
		else { \
			if (NULL != buf) { \
				buf[(*(offset)) + iloop] = (BYTE)remain_len; \
			} \
			break; \
		} \
	} \
	*(offset) += 1 + iloop; \
} while(0)

#define decode_varint(buf, value, offset) do{ \
    size_t iloop; \
    (*(value)) = 0; \
    for(iloop=0;;++iloop){ \
	     (*(value)) += ((buf)[iloop] & 0x7F) << (7*iloop); \
        if(0 == ((buf)[iloop] & 0x80)){ \
            break; \
        } \
    } \
    (*(offset)) += 1 + iloop; \
} while(0)


#endif

/* end of file */