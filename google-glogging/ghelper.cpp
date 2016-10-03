
#include <stdlib.h>
#include "gLogHelper.h"

#define LOGDIR "/home/xudi/log"

using namespace std;

void SignalHandle(const char* data, int size)
{
    std::string str = std::string(data,size);
    /*
    std::ofstream fs("glog_dump.log",std::ios::app);
    fs<<str;
    fs.close();
    */
    LOG(ERROR)<<str;

}

GLogHelper::GLogHelper(char* program)
{
    google::InitGoogleLogging(program);
    google::SetStderrLogging(google::INFO);
    FLAGS_colorlogtostderr=true;
    google::SetLogDestination(google::INFO,LOGDIR"/INFO_");
    google::SetLogDestination(google::WARNING,LOGDIR"/WARNING_");
    google::SetLogDestination(google::ERROR,LOGDIR"/ERROR_");
    FLAGS_logbufsecs =0;
    FLAGS_max_log_size =100;
    FLAGS_stop_logging_if_full_disk = true;
    google::SetLogFilenameExtension("91_");
    google::InstallFailureSignalHandler();
    google::InstallFailureWriter(&SignalHandle);
}

GLogHelper::~GLogHelper()
{
    google::ShutdownGoogleLogging();
}
