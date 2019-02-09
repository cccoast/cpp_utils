
#include <pybind11/pybind11.h>
#include "header.h"

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
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
}
