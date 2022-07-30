#include "common_inc.h"


void OnAsciiCmd(const char* _cmd, size_t _len, StreamSink &_responseChannel)
{
    /*---------------------------- ↓ Add Your CMDs Here ↓ -----------------------------*/
    uint8_t argNum;

    // Check incoming packet type
    if (_cmd[0] == '1') // position control
    {
        float value;
        argNum = sscanf(_cmd, "p %f", &value);
        Respond(_responseChannel, false, "Got %d arg(s): %f\n", argNum, value);
    }
    /*---------------------------- ↑ Add Your CMDs Here ↑ -----------------------------*/
}