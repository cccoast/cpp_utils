#ifndef PYSHMDATAFRAME_INCLUDED
#define PYSHMDATAFRAME_INCLUDED

#include "../ShmDataFrame/ShmDataFrame.h"
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/stl_iterator.hpp>
#include <string>

using namespace std;
using namespace boost::python;

template< typename T >
inline std::vector<T> to_std_vector(const boost::python::object& iterable)
{
	return std::vector<T>(boost::python::stl_input_iterator<T>(iterable),
		boost::python::stl_input_iterator<T>());
}


class pyDataFrame{
    ShmDataFrame data_frame;
    string ipckey;
public:
    pyDataFrame(string ipckey):ipckey(ipckey)
    {
    }

    void create_shm(long nColumns,long nIndexs,boost::python::list column_sizes){
        auto std_vector_column_sizes = to_std_vector<long>(column_sizes);
        data_frame.create_shm(this->ipckey,nColumns,nIndexs,std_vector_column_sizes);
    }

    void connect_shm()
    {
        data_frame.connect_shm(this->ipckey);
    }

    void set_field_double(long index,long column,double src){
        data_frame.set_field(index,column,&src);
    }

    void set_field_long(long index,long column,long src){
        data_frame.set_field(index,column,&src);
    }

    //void set_field_account_number(long index,long column,ShmDataFrame::account_number src);
    //void set_field_account_name(long index,long column,ShmDataFrame::account_name src);

    double get_field_double(long index, long column){
        return *(double *)data_frame.get_field(index,column);
    }

    long   get_field_long(long index, long column){
        return *(long *)data_frame.get_field(index,column);
    }
};


#endif // PYSHMDATAFRAME_INCLUDED
