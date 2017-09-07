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
    typedef char account_number_t[32];
    typedef char account_name_t[128];
    typedef char trade_time_stamp_t[16];
    typedef long trade_date_t;
    typedef long direction_t;
    typedef long status_t;
    typedef char field_name_t[64];
public:
    long nColumns;
    long nIndexes;
    long column_sizes[MAX_FIELDS];
    long column_offset[MAX_FIELDS];
    field_name_t field_names[MAX_FIELDS];
    long row_size;
    void* shm_addr;
    void* start_addr;

public:
    ShmDataFrame();
    int create_shm(string ipckey,long nColumns,long nIndexs,vector<long>& column_sizes);
    int connect_shm(string ipckey);
    void* get_field(long index, long column);
    int set_field(long index,long column,void* src);

    int set_field_names(vector<string>& vec);
    int set_field_name(long index, string& s);

    vector<string> get_field_names();
    string get_field_name(long index);

};


#endif // SHMDATAFRAME_H_INCLUDED
