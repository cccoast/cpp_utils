
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <pybind11/pytypes.h>

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "header.h"

using map_type = std::map<std::string, int>;
PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::vector<shared_ptr<Animal>>);
PYBIND11_MAKE_OPAQUE(map_type);

namespace py = pybind11;

void map_say_hi(std::shared_ptr<Animal>& animal){
    animal->map_say_hi();
}

void map_go(std::shared_ptr<Animal>& animal,int n){
    int count = 0;
    cout << "container size = " << animal->get_container().size() << endl;
    for(shared_ptr<Animal>& sptr: animal->get_container()){
        cout << count++ << endl;
        cout << sptr->go(n) << endl;
    }
}

std::string call_go(std::shared_ptr<Animal>& animal) {
    return animal->go(animal.use_count());
}

void call_regist(std::shared_ptr<PyAnimal>& animal){
    animal->regist();
    animal->say_hi();
    animal->map_say_hi();
}

void delete_deepcopied_raw(std::shared_ptr<Animal>& animal){
    Animal* pet = animal->deep_copy_raw();
    delete pet;
}

void delete_deepcopied_shared(std::shared_ptr<Animal>& animal){
    std::shared_ptr<Animal> pet = animal->deep_copy_shared();
}

///Animal's memory is managed by shared_ptr, return raw ptr will cause undefined behaviour
Animal* make_deep_copy_rawptr(std::shared_ptr<Animal>& animal){
    Animal* pet = animal->deep_copy_raw();
    return pet;
}

///virtual property has lost, cpp don't now the true type of pet
std::shared_ptr<Animal> make_deep_copy_sharedptr(std::shared_ptr<Animal>& animal){
    std::shared_ptr<Animal> pet = animal->deep_copy_shared();
    return pet;
}

///cannot dynamic call inherited virtaul function
void test_deep_copy_sharedptr(std::shared_ptr<PyAnimal>& animal){
    shared_ptr<PyAnimal> pet = std::dynamic_pointer_cast<PyAnimal>(animal->deep_copy_shared());
    cout << pet->go(1) << endl;
    return ;
}

int add(int i, int j) {
    return i + j;
}

void append_1(std::vector<int> &v) {
   v.push_back(1);
}

py::list* get_range_ptr(int n){
    py::list* l = new py::list;
    for(int i = 0; i < n; i++)
        l->append(i);
    return l;
}

py::list get_range_copy(int n){
    py::list l;
    for(int i = 0; i < n; i++)
        l.append(i);
    return l;
}

py::list get_range_move(int n){
    py::list l;
    for(int i = 0; i < n; i++)
        l.append(i);
    return l;
}

void pylist_append_ref(py::list &l,int n) {
   l.append(n);
}

void pylist_append_ptr(py::list* l,int n){
    l->append(n);
}

void int_list_map_print(py::list& l){
    std::for_each(l.begin(),l.end(),[](auto v){ std::cout << py::cast<int>(v) << ' ';});
}

void list_map_print(py::list& l){
    std::for_each(l.begin(),l.end(),[](auto v){ std::cout << v << ' ';});
}

void int_list_map_plus(py::list& l,int n){
    for(int i = 0; i < l.size(); i++){
        l[i] = l[i].cast<int>() + n;
    }
}


PYBIND11_MODULE(libexample, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("add", &add, "A function which adds two numbers");

    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName);

    py::class_<Animal, PyAnimal /* <--- trampoline*/, std::shared_ptr<Animal> >(m, "Animal")
        .def(py::init<>())
        .def("go", &Animal::go)
        .def("say_hi", &Animal::say_hi)
        .def("use_count", &Animal::use_count)
        .def("deep_copy", &Animal::deep_copy_raw)
        .def("get_container", &Animal::get_container, py::return_value_policy::reference)
        .def("map_say_hi",&Animal::map_say_hi)
        .def("regist",&Animal::regist);

    py::class_<Dog, Animal,std::shared_ptr<Dog> >(m, "Dog")
        .def(py::init<>())
        .def("say_hi", &Dog::say_hi)
        .def("use_count", &Dog::use_count)
        .def("deep_copy", &Dog::deep_copy_raw);

    py::class_<Container,std::shared_ptr<Container> >(m, "Container")
        .def(py::init<>())
        .def("add", &Container::add)
        .def("exec_last", &Container::exec_last);

    m.def("call_go", &call_go);
    m.def("append_one",&append_1);
    m.def("get_range_ptr",&get_range_ptr);
    m.def("get_range_copy",&get_range_copy,py::return_value_policy::copy);
    m.def("get_range_move",&get_range_move,py::return_value_policy::move);
    m.def("pylist_append_ref",&pylist_append_ref);
    m.def("pylist_append_ptr",&pylist_append_ptr);
    m.def("int_list_map_print",&int_list_map_print);
    m.def("list_map_print",&list_map_print);
    m.def("int_list_map_plus",&int_list_map_plus);
    m.def("make_deep_copy_rawptr",&make_deep_copy_rawptr,py::return_value_policy::take_ownership);
    m.def("make_deep_copy_sharedptr",&make_deep_copy_sharedptr,py::return_value_policy::copy);
    m.def("delete_deepcopied_raw",&delete_deepcopied_raw,py::return_value_policy::take_ownership);
    m.def("delete_deepcopied_shared",&delete_deepcopied_shared,py::return_value_policy::copy);
    m.def("call_regist",&call_regist);
    m.def("test_deep_copy_sharedptr",&test_deep_copy_sharedptr);
    m.def("map_say_hi",&map_say_hi);
    m.def("map_go",&map_go);

    py::bind_vector<std::vector<int>>(m, "VectorInt");
    py::bind_map<map_type>(m, "MapStringInt");

    py::bind_vector<std::vector<std::shared_ptr<Animal>>>(m,"VectorAnimal");
}
