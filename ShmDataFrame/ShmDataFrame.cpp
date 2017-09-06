#include "ShmDataFrame.h"
#include "linuxShm.h"

#include <iostream>
#include <algorithm>
#include <functional>
#include <string>

using namespace std;

ShmDataFrame::ShmDataFrame(){
    memset(this,0,sizeof(*this));
}

int ShmDataFrame::connect_shm(string ipckey){
    LinuxShmAdaptor shm_api(ipckey);
    int re = shm_api.create_mem_handler();
    if(re < 0){
        cout << "can not create shm" << endl;
        exit(-1);
    }
    void* _shm_addr = shm_api.create_start_addr();
    if(_shm_addr != NULL){
        memcpy(this,_shm_addr,sizeof(*this));
        this->shm_addr = _shm_addr;
        this->start_addr = this->shm_addr + sizeof(*this);
    }
    else{
        cout << "no such ipckey or can not connect to ipckey " << ipckey << endl;
        exit(-1);
    }
    return 0;
}

int ShmDataFrame::create_shm(string ipckey,long nColumns,long nIndexs,vector<long>& column_sizes){
    LinuxShmCreator shm_api(ipckey);
    size_t _row_size = std::accumulate(column_sizes.begin(),column_sizes.end(),0);
    size_t alloc_size = _row_size * nIndexs + sizeof(*this) + 10240;
    int re = shm_api.create_memory(alloc_size);
    if(re < 0){
        cout << "can not create shm" << endl;
        exit(-1);
    }
    void* _shm_addr = shm_api.create_start_addr();
    if(_shm_addr == NULL){
        cout << "can not connect shm" << endl;
        exit(-1);
    }
    else{
        this->nColumns = nColumns;
        this->nIndexes = nIndexs;
        this->row_size = _row_size;
        this->shm_addr = _shm_addr;
        this->start_addr = _shm_addr + sizeof(*this);
        int i = 0;
        long cumsum = 0;
        for(auto _i_column_size : column_sizes){
            this->column_sizes[i] = _i_column_size;
            this->column_offset[i] = cumsum;
            i++;
            cumsum += _i_column_size;
        }
        memcpy(_shm_addr,this,sizeof(*this));
    }
    return 0;
}

int ShmDataFrame::set_field(long index,long column,void* src){
    void* des = get_field(index,column);
    memcpy(des,src,this->column_sizes[column]);
    return 0;
}

void* ShmDataFrame::get_field(long index, long column){

    return ( this->start_addr + this->row_size * index + this->column_offset[column] );
}
