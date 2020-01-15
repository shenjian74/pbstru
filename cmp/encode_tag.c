#include <stdio.h>

#define WIRE_TYPE_VARINT 0
typedef unsigned char BYTE;

main(){
    BYTE tag;
    BYTE wire_type = WIRE_TYPE_VARINT;

    for(tag = 8;tag <=24;++tag) {
        if(tag>15){
            BYTE byte1 = tag | 0x80;
            BYTE byte2 = ((BYTE)(tag/16) & 0x78) | wire_type;
            BYTE byte3 = (tag << 3) | wire_type | 0x80;
            BYTE byte4 = (tag >> 4);
            printf("%d) %02x-%02x => %02x-%02x\n", tag, byte1, byte2, byte3, byte4);
        } else {
            BYTE byte1 = (BYTE)(tag << 3) | wire_type;
            printf("%d) %02x\n", tag, byte1);
        }
    }
}

