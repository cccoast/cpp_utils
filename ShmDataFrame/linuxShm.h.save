///@author xudi
///This file is used for map file to memory
///Base library for ShareMemory


#ifndef __LINUX_SHM__
#define __LINUX_SHM__

#include <iostream>
#include <string>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

using namespace std;

static bool createFixedSizeFile(string fileName, size_t reqSize){
    FILE* fd = fopen(fileName.c_str(), "w+");
	if (fd == NULL)
	{
		return false;
	}
	fseek(fd, reqSize, 0);
	char a = EOF;
	size_t ret = fwrite((void*)&a, sizeof(char),1,fd);
	fclose(fd);
	return (ret == sizeof(char));
}

static void fileSeek (FILE* fd, long long distance){
    fseek(fd, distance, SEEK_CUR);
}

static size_t getFileSizeSysCall(string fileName){
    struct stat sb;
    int fd = open(fileName.c_str(),O_RDONLY);
    if (fstat(fd, &sb) == -1){
        close(fd);
        return -1;
    }
    close(fd);
    return sb.st_size;
}

static size_t getFileSize(string fileName)
{
#if (0)
    FILE* fd = fopen(fileName.c_str(), "rb");
	if (fd == NULL)
	{
		return false;
	}
	fseek(fd, 0, SEEK_END);
	size_t fsize = ftell(fd);
	fclose(fd);
	return fsize;
#else
    return getFileSizeSysCall(fileName);
#endif
}

static bool deleteShm(string& key){
#if (1)
    int shmid;
    long long ipc_key = stoll(key,0,16);
    shmid = shmget(ipc_key, 0, 0);
    struct shmid_ds shmds;
    size_t ret = shmctl(shmid, IPC_STAT, &shmds);
    if (shmid >= 0 && ret != -1) {
        if (shmds.shm_nattch != 0)
        {
            cerr << "cann't delete shm, nattach more the one" << endl;
            return false;
        }
        int ret = shmctl(shmid, IPC_RMID, 0);
        return (ret == 0);
    } else {
        return false;
    }
#else
    boost::format s("ipcrm -M %1%");
    char buff[1024] = {0};
    s % key;
    FILE* fd = popen(s.str().c_str(),"r");
    fgets(buff,1023,fd);
    fclose(fd);
    return (atoi(buff) == 0);
#endif
}

class LinuxMMap{
private:
	string file_name;
	size_t fsize;
	void*  start_addr;
	int    fd;
public:
	LinuxMMap(string fname):file_name(fname),fsize(0),start_addr(NULL),fd(-1)
    {
        fsize = getFileSize(fname);
    }

	~LinuxMMap(){
        if(start_addr){
            munmap(start_addr,fsize);
            start_addr = NULL;
        }
        if(fd > 0) close(fd);
	}

	void* get_start_addr(){ return start_addr;}

    int create_handler(){
        if(fd < 0){
            fd = open(file_name.c_str(),O_RDWR);
        }
        return fd;
    }

	void* create_start_addr(){
        if(fd < 0) this->create_handler();
        if(fd > 0) {
            start_addr = mmap(0, fsize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
            return start_addr;
        }
        return NULL;
	}

	bool flush()
	{
		if(start_addr)
		{
			return ( msync(start_addr,fsize,MS_SYNC) == 0 );
        }
		return false;
	}

	void close_file(){
         if(start_addr){
            munmap(start_addr,fsize);
            start_addr = NULL;
            close(fd);
            fd = -1;
        }
	}
};

class LinuxShmAdaptor{
	string key;
	long long int_key;
	void*  start_addr;
	int    handler;
public:
	LinuxShmAdaptor(string _key){
		key = _key;
		int_key = stoll(_key,0,16);
		start_addr = NULL;
		handler = -1;
	}

	~LinuxShmAdaptor(){
	}

	string get_key(){
		return key;
	}
	long long get_int_key(){
        return int_key;
	}

	int get_mem_handler(){
		return handler;
	}
	void set_mem_handler(int _handler){
		handler = _handler;
	}

	int create_mem_handler(){
		if (handler >= 0){
			return handler;
		}
		else{
			if(this->key.length() > 0){
                handler = shmget(int_key,0,0);
			}
			return handler;
		}
	}

	void* get_start_addr(){
		return start_addr;
	}

	void* create_start_addr(){
		if (start_addr != NULL){
			return start_addr;
		}
		else{
			if(handler < 0){
				this->create_mem_handler();
			}

			if(handler >= 0){
                start_addr = shmat(handler,NULL,0);
                return start_addr;
			}
			else{
				return NULL;
			}
        }
	}
};

/// This Creator is used to create ipc Key only once
/// If Key already exits, return -1
class LinuxShmCreator:public LinuxShmAdaptor{
private:
	unsigned long long shm_size;
public:
	LinuxShmCreator(string key):LinuxShmAdaptor(key){}

	int create_memory(unsigned long long _size){
		this->shm_size = _size;
		if( (this->get_mem_handler() < 0) && _size > 0){
            if(shmget(this->get_int_key(), 0, 0) < 0){
                //cout << "not exist!" << endl;
                this->set_mem_handler( shmget(this->get_int_key(), _size, IPC_CREAT | 0666) );
            }
            else{
                //cout << "exist!" << endl;
                this->set_mem_handler(-1);
                return -1;
            }
		}
		else{
            return -1;
		}

		if(this->get_mem_handler() < 0){
            this->set_mem_handler(-1);
		}
		return this->get_mem_handler();
	}

	bool delete_memory(){
        string s = this->get_key();
        bool ret = deleteShm(s);
        if( ret ){
            this->set_mem_handler(-1);
            return true;
        }
        else return false;
	}
};

#endif
