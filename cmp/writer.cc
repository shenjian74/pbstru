#include <fstream>
#include "Hello.pb.h"
using namespace std;

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

    return 0;
}
