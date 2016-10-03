#include "gLogHelper.h"

int main(int argc,char* argv[])
{
    GLogHelper gh(argv[0]);

    LOG(INFO)<<"my name is xudi";
    LOG(ERROR)<<"hello world";
}
