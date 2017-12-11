#include "pbstru_comm.h"

void encode_tag_byte(BYTE *buf, WORD tag, BYTE wire_type, size_t *offset) { 
	if (tag < 16) { 
		if (NULL != buf) { 
			buf[*offset] = (BYTE)(tag << 3) | wire_type; 
		} 
		*offset += 1; 
	} 
	else { 
		if (NULL != buf) { 
			buf[*offset] = ((BYTE)tag) | 0x80; 
			buf[(*offset) + 1] = ((BYTE)(tag >> 4) & 0x78) | wire_type; 
		} 
		*offset += 2; 
	} 
} 

void parse_tag_byte(const BYTE* buf, WORD *field_num, BYTE *wire_type, size_t *offset){ 
    if ((buf)[0] & 0x80) { 
	     *field_num = ((buf)[0] & 0x7F) + ((buf)[1] >> 3) * 128; 
	     *wire_type = (buf)[1] & 0x07; 
	     (*(offset)) += 2; 
    } else { 
        *field_num = (buf)[0] >> 3; 
	     *wire_type = (buf)[0] & 0x07; 
	     (*(offset)) += 1; 
    } 
} 

void deal_unknown_field(const BYTE wire_type, const BYTE* buf, size_t* offset) { 
	size_t tmp_field_len; 
	switch(wire_type){ 
	case WIRE_TYPE_VARINT: 
		decode_varint(buf, &tmp_field_len, offset); 
		break; 
	case WIRE_TYPE_FIX64: 
		(*(offset)) += 8; 
		break; 
	case WIRE_TYPE_LENGTH_DELIMITED: 
		decode_varint(buf, &tmp_field_len, offset); 
        (*(offset)) += tmp_field_len; 
		break; 
	case WIRE_TYPE_FIX32: 
        (*(offset)) += 4; 
		break; 
	default: 
		break; 
	} 
}
/* end of file */

