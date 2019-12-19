#include <iostream>
#include <fstream>
#include "hello.h"
using namespace std;

int main (void)
{
    BYTE buffer[1024];
    st_hello msg;
    constru_message_Hello(&msg);

    fstream input("./log", ios::in | ios::binary);
    // get length of file:
    input.seekg (0, input.end);
    size_t file_len = input.tellg();
    input.seekg (0, input.beg);
    if(!input.read((LPSTR)buffer, file_len)) {
        cerr << "Failed to parse log." << endl;
        return -1;
    }

    decode_message_Hello(buffer, input.gcount(), &msg);
    cout << "id: " << msg.var_id << endl;
    cout << "flt: " << msg.var_flt << endl;
    cout << "str: " << msg.var_str.data << endl;
    return 0;
}
