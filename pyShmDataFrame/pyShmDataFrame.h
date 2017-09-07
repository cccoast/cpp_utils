#ifndef PYSHMDATAFRAME_INCLUDED
#define PYSHMDATAFRAME_INCLUDED

#include "../ShmDataFrame/ShmDataFrame.h"
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/str.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/tuple.hpp>
#include <memory.h>
#include <string>

using namespace std;

template< typename T >
inline std::vector<T> to_std_vector(const boost::python::object& iterable)
{
    return std::vector<T>(boost::python::stl_input_iterator<T>(iterable),
                          boost::python::stl_input_iterator<T>());
}

class pyDataFrame
{
    ShmDataFrame data_frame;
    string ipckey;

public:

    pyDataFrame(string _ipckey):ipckey(_ipckey)
    {
        memset(&data_frame,0,sizeof(ShmDataFrame));
    }

    long getCols()
    {
        return data_frame.nColumns;
    }

    long getRows()
    {
        return data_frame.nIndexes;
    }

    boost::python::list getUnitSizes()
    {
        boost::python::list v;
        for(int i = 0; i < data_frame.nColumns; i++)
        {
            v.append(data_frame.column_sizes[i]);
        }
        return v;
    }

    void set_field_names(boost::python::list vec)
    {
        vector<string> _vec = to_std_vector<string>(vec);
        this->data_frame.set_field_names(_vec);
    }

    boost::python::list get_field_names()
    {
        boost::python::list vec;
        for(string s:this->data_frame.get_field_names()){
            vec.append(boost::python::str(s));
        }
        return vec;
    }

    boost::python::str get_field_name(long index)
    {
        return boost::python::str(this->data_frame.get_field_name(index));
    }

    void set_field_name(long index,boost::python::str& s){
        string _s = boost::python::extract<std::string>(s);
        this->data_frame.set_field_name(index,_s);
    }

    void create_shm(long nColumns,long nIndexs,boost::python::list column_sizes)
    {
        auto std_vector_column_sizes = to_std_vector<long>(column_sizes);
        data_frame.create_shm(this->ipckey,nColumns,nIndexs,std_vector_column_sizes);
    }

    void connect_shm()
    {
        data_frame.connect_shm(this->ipckey);
    }

    /// set
    void set_field_double(long index,long column,double src)
    {
        data_frame.set_field(index,column,&src);
    }

    void set_field_long(long index,long column,long src)
    {
        data_frame.set_field(index,column,&src);
    }

    void set_field_int32(long index,long column,int32_t src)
    {
        data_frame.set_field(index,column,&src);
    }

    void set_field_str(long index,long column,boost::python::str& s)
    {
        string _s = boost::python::extract<std::string>(s);
        data_frame.set_field(index,column,(void *)_s.c_str());
    }

    void set_row(long index, boost::python::list& vec)
    {
        void* start_addr = data_frame.get_field(index,0);
        for(int col = 0; col < data_frame.nColumns; col++)
        {
            switch(data_frame.column_sizes[col])
            {
                case sizeof(int32_t):
                {
                    int32_t _v = boost::python::extract<int32_t>(vec[col]);
                    memcpy(start_addr,&_v,sizeof(int32_t));
                    break;
                }
                case sizeof(double):
                {
                    double _v = boost::python::extract<double>(vec[col]);
                    memcpy(start_addr,&_v,sizeof(double));
                    break;
                }
                default:
                {
                    string _v = boost::python::extract<std::string>(vec[col]);
                    memcpy(start_addr,_v.c_str(),data_frame.column_sizes[col]);
                    break;
                }
            }
            start_addr += data_frame.column_sizes[col];
        }
    }

    template<typename T>
    void _set_col_impl(long index, long col, boost::python::list& vec){
        int length = boost::python::len(vec);
        void* start_addr = this->data_frame.get_field(index,col);
        size_t row_size  = this->data_frame.row_size;
        size_t copy_size = this->data_frame.column_sizes[col];
        for(int i = 0; i < length; i++){
            T v = boost::python::extract<T>(vec[i]);
            memcpy(start_addr,&v,copy_size);
            start_addr += row_size;
        }
    }

    void _set_col_impl_str(long index, long col, boost::python::list& vec){
        int length = boost::python::len(vec);
        void* start_addr = this->data_frame.get_field(index,col);
        size_t row_size  = this->data_frame.row_size;
        size_t copy_size = this->data_frame.column_sizes[col];
        for(int i = 0; i < length; i++){
            string v = boost::python::extract<string>(vec[i]);
            memcpy(start_addr,v.c_str(),copy_size);
            start_addr += row_size;
        }
    }

    void set_col(long index, long col, boost::python::list& vec){

        switch(data_frame.column_sizes[col])
            {
                case sizeof(int32_t):
                {
                    _set_col_impl<int32_t>(index,col,vec);
                    break;
                }
                case sizeof(double):
                {
                    _set_col_impl<double>(index,col,vec);
                    break;
                }
                default:
                {
                    _set_col_impl_str(index,col,vec);
                    break;
                }
            }
    }


    /// get
    double get_field_double(long index, long column)
    {
        return *(double *)data_frame.get_field(index,column);
    }

    long get_field_long(long index, long column)
    {
        return *(long *)data_frame.get_field(index,column);
    }

    int get_field_int32(long index, long column)
    {
        return *(int32_t *)data_frame.get_field(index,column);
    }

    string get_field_str(long index, long column)
    {
        return string((char *)data_frame.get_field(index,column));
    }

    boost::python::list get_row(long index){
        boost::python::list vec;
        void* start_addr = data_frame.get_field(index,0);
        for(int col = 0; col < data_frame.nColumns; col++)
        {
            switch(data_frame.column_sizes[col])
            {
                case sizeof(int32_t):
                {
                    int32_t v = *((int32_t *)start_addr);
                    vec.append(v);
                    break;
                }
                case sizeof(double):
                {
                    double v = *((double *)start_addr);
                    vec.append(v);
                    break;
                }
                default:
                {
                    boost::python::str v(string(((char *)start_addr)));
                    vec.append(v);
                    break;
                }
            }
            start_addr += data_frame.column_sizes[col];
        }
        return vec;
    }

    template<typename T>
    boost::python::list _get_col_impl(long index, long col, long length){
        boost::python::list vec;
        void* start_addr = this->data_frame.get_field(index,col);
        size_t row_size  = this->data_frame.row_size;
        size_t copy_size = this->data_frame.column_sizes[col];
        for(int i = 0; i < length; i++){
            T v = *((T *)start_addr);
            vec.append(v);
            start_addr += row_size;
        }
        return vec;
    }

     boost::python::list _get_col_impl_str(long index, long col, long length){
        boost::python::list vec;
        void* start_addr = this->data_frame.get_field(index,col);
        size_t row_size  = this->data_frame.row_size;
        size_t copy_size = this->data_frame.column_sizes[col];
        for(int i = 0; i < length; i++){
            boost::python::str v(string((char *)start_addr));
            vec.append(v);
            start_addr += row_size;
        }
        return vec;
    }

     boost::python::list get_col(long index, long col, long length){

        switch(data_frame.column_sizes[col])
            {
                case sizeof(int32_t):
                {
                    return _get_col_impl<int32_t>(index,col,length);
                    break;
                }
                case sizeof(double):
                {
                    return _get_col_impl<double>(index,col,length);
                    break;
                }
                default:
                {
                    return _get_col_impl_str(index,col,length);
                    break;
                }
            }
    }

};


#endif // PYSHMDATAFRAME_INCLUDED
