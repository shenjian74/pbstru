#include <iostream>
#include <fstream>
#include "hello.h"
using namespace std;

int main (void)
{
    BYTE buffer[1024];
    st_hello msg;
    constru_message_Hello(&msg);
    
    msg.var_id = 101;
    msg.var_flt = 13.14;
    char str[] = "hello";
    msg.var_str.data = str;
    msg.var_str.length = sizeof(str);

    size_t len = encode_message_Hello_safe(&msg, buffer, sizeof(buffer));

    fstream output ("./log", ios::out | ios::trunc | ios::binary);
    if(!output.write((LPCSTR)buffer, len)) {
        cerr << "Failed to write log." << endl;
        return -1;
    }
    return 0;
}


