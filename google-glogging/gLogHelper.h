#include <glog/logging.h>
#include <glog/raw_logging.h>

void SignalHandle(const char* data, int size);

class GLogHelper
{
public:

    GLogHelper(char* program);
    ~GLogHelper();
};
