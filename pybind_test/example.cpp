
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <pybind11/pytypes.h>
#include <vector>
#include <map>
#include <string>
#include "header.h"

using map_type = std::map<std::string, int>;
PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(map_type);

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

void append_1(std::vector<int> &v) {
   v.push_back(1);
}

py::list* get_range_list(int n){
    py::list* l = new py::list;
    for(int i = 0; i < n; i++)
        l->append(i);
    return l;
}

void pylist_append_1ref(py::list &v) {
   v.append(1);
}

void pylist_append_1p(py::list* l){
    l->append(1);
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
        .def("use_count", &Animal::use_count);

    py::class_<Dog, Animal,std::shared_ptr<Dog> >(m, "Dog")
        .def(py::init<>())
        .def("say_hi", &Dog::say_hi)
        .def("use_count", &Dog::use_count);

    py::class_<Container,std::shared_ptr<Container> >(m, "Container")
        .def(py::init<>())
        .def("add", &Container::add)
        .def("exec_last", &Container::exec_last);

    m.def("call_go", &call_go);
    m.def("append_one",&append_1);
    m.def("get_range_list",&get_range_list);
    m.def("pylist_append_1ref",&pylist_append_1ref);
    m.def("pylist_append_1p",&pylist_append_1p);

    py::bind_vector<std::vector<int>>(m, "VectorInt");
    py::bind_map<map_type>(m, "MapStringInt");
}
