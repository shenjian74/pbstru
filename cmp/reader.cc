#include <fstream>
#include "Hello.pb.h"
using namespace std;

int main (void)
{
    unsigned char buffer[1024];
    Hello msg;

    fstream input("./log", ios::in | ios::binary);
    // get length of file:
    input.seekg (0, input.end);
    size_t file_len = input.tellg();
    input.seekg (0, input.beg);
    if(!input.read((char *)buffer, file_len)) {
        cerr << "Failed to parse log." << endl;
        return -1;
    }

    msg.ParseFromArray(buffer, file_len);
    cout << "id: " << msg.id() << endl;
    cout << "flt: " << msg.flt() << endl;
    cout << "str: " << msg.str() << endl;
    return 0;
}
