#ifndef SHMDATAFRAME_H_INCLUDED
#define SHMDATAFRAME_H_INCLUDED

//##################################################################################
//###@author xudi                                                                  #
//###@Mail   xudi1989@ruc.edu.cn                                                   #
//###@Lisence GPL                                                                  #
//##################################################################################

#include <string>
#include <vector>

#define MAX_FIELDS 1024
#define MAX_INDEXES 1000000

using namespace std;

class ShmDataFrame{

public:
    typedef char account_number[32];
    typedef char account_name[128];
    typedef char trade_time_stamp[16];
    typedef long trade_date;
    typedef long direction;
    typedef long status;

public:
    long nColumns;
    long nIndexes;
    long column_sizes[MAX_FIELDS];
    long column_offset[MAX_FIELDS];
    long row_size;
    void* shm_addr;
    void* start_addr;

public:
    ShmDataFrame();
    int create_shm(string ipckey,long nColumns,long nIndexs,vector<long>& column_sizes);
    int connect_shm(string ipckey);
    int set_field(long index,long column,void* src);
    void* get_field(long index, long column);
};


#endif // SHMDATAFRAME_H_INCLUDED
