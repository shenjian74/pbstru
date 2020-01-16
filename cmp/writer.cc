#include <fstream>
#include "Hello.pb.h"
using namespace std;

typedef unsigned char BYTE;

char hex2asc(int hex)
{
    if (hex >= 0 && hex < 10) {
        return hex + 0x30;
    } else if (hex < 16) {
        return 'a' + hex - 10;
    } else {
        return '.';
    }
}

void print_buffer(BYTE * content, size_t filelen)
{
    size_t i;
    size_t line;
    size_t total_len;
    size_t total_line;
    char buffer[128];

    total_len = filelen;
    total_line = total_len / 16;
    fputs("\n", stderr);
    sprintf(buffer, "[Total %zu bytes]\n", filelen);
    fputs(buffer, stderr);

    for (line = 0; line < total_line; line++) {
        for (i = 0; i < 16; i++) {
            fputc(hex2asc(content[16 * line + i] / 16), stderr);
            fputc(hex2asc(content[16 * line + i] % 16), stderr);
            fputc(' ', stderr);
        }
        fputs("    ", stderr);
        for (i = 0; i < 16; i++) {
            fputc(isprint(content[16 * line + i]) ? content[16 * line + i] : '.', stderr);
        }
        fputc('\n', stderr);
    }
    for (i = 0; i < total_len % 16; i++) {
        fputc(hex2asc(content[16 * total_line + i] / 16), stderr);
        fputc(hex2asc(content[16 * total_line + i] % 16), stderr);
        fputc(' ', stderr);
    }
    for (i = total_len % 16; i < 16; i++) {
        fputs("   ", stderr);
    }
    fputs("    ", stderr);
    for (i = 0; i < total_len % 16; i++) {
        fputc(isprint(content[16 * total_line + i]) ? content[16 * total_line + i] : '.', stderr);
    }
    fputc('\n', stderr);
    fflush(stderr);
}

int main (void)
{
    unsigned char buffer[1024];
    Hello msg;
    msg.set_id (101);
    msg.set_flt (13.14);
    msg.set_str ("hello");
    msg.SerializeToArray(buffer, sizeof(buffer));

    fstream output ("./log", ios::out | ios::trunc | ios::binary);
    if(!output.write((const char *)buffer, msg.ByteSize())) {
        cerr << "Failed to write log." << endl;
        return -1;
    }

    ut_test_message tm;
    tm.Clear();
    tm.add_pf_fixed64(1);
    tm.add_pf_fixed64(2);
    tm.SerializeToArray(buffer, sizeof(buffer));
    print_buffer(buffer, tm.ByteSize());

    tm.Clear();
    tm.add_pf_fixed64(1);
    tm.SerializeToArray(buffer, sizeof(buffer));
    print_buffer(buffer, tm.ByteSize());

    tm.Clear();
    tm.add_pf_uint64(0x1234);
    tm.add_pf_uint64(0x5678);
    tm.SerializeToArray(buffer, sizeof(buffer));
    print_buffer(buffer, tm.ByteSize());

    return 0;
}
