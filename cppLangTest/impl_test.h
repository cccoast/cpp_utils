#ifndef IMPL_TEST_H_INCLUDED
#define IMPL_TEST_H_INCLUDED

//##################################################################################
//###@author xudi                                                                  #
//###@Mail   xudi1989@ruc.edu.cn                                                   #
//###@Lisence GPL                                                                  #
//##################################################################################

#include "utility"
#include <iostream>

template<typename T>
class impl_test
{
private:
    class impl;
    impl* _impl;
public:
    impl_test();
    ~impl_test();
    T get_attr();
    void set_attr(T&& val);
    void set_attr(T& val);
};

template<typename T>
class impl_test<T>::impl{
public:
    T val;
    void set_attr(T&& v){
        std::cout << "impl &&" << std::endl;
        val = v;
    }
    void set_attr(T& v){
        std::cout << "impl &" << std::endl;
        val = v;
    }
    T get_attr(){
        return val;
    }
};

template<typename T>
impl_test<T>::impl_test(){
    _impl = new impl;
}

template<typename T>
impl_test<T>::~impl_test(){
    if(_impl != nullptr){
        delete _impl;
        _impl = nullptr;
    }
}

template<typename T>
T impl_test<T>::get_attr(){
    return this->_impl->get_attr();
}

template<typename T>
void impl_test<T>::set_attr(T&& v){
    std::cout << "&& right value" << std::endl;
    return this->_impl->set_attr(std::forward<T>(v));
}

template<typename T>
void impl_test<T>::set_attr(T& v){
    std::cout << "& left value" << std::endl;
    return this->_impl->set_attr(std::forward<T&>(v));
}


/*
template <typename T>
class impl_test{
private:
    T val;
public:
    impl_test();
    ~impl_test();
    T get_attr();
    void set_attr(T&& val);
};

template<typename T>
impl_test<T>::impl_test(){

};

template<typename T>
impl_test<T>::~impl_test(){

};

template<typename T>
T impl_test<T>::get_attr(){
    return val;
};

template<typename T>
void impl_test<T>::set_attr(T&& v){
    val = v;
};
*/

#endif // IMPL_TEST_H_INCLUDED
