#ifndef __HEADER_H__
#define __HEADER_H__

#include <string>
#include <memory>
#include <functional>
#include <iostream>
#include <vector>


struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};

class Animal:public std::enable_shared_from_this<Animal>{
public:
    virtual ~Animal() { }
    virtual std::string go(int n_times) = 0;
    virtual Animal* deep_copy_raw() = 0;
    virtual std::shared_ptr<Animal> deep_copy_shared() = 0;
    virtual void say_hi() = 0;
    virtual int use_count(){
        return shared_from_this().use_count() - 1;
    }
};


class PyAnimal : public Animal {
public:
    /* Inherit the constructors */
    using Animal::Animal;

    /* Trampoline (need one for each virtual function) */
    std::string go(int n_times) override {
        PYBIND11_OVERLOAD_PURE(
            std::string, /* Return type */
            Animal,      /* Parent class */
            go,         /* Name of function in C++ (must match Python name) */
            n_times/* Argument(s) */
        );
    }

    Animal* deep_copy_raw() override {
        PYBIND11_OVERLOAD_PURE(
            Animal*,
            Animal,
            deep_copy,
        );
    }

    std::shared_ptr<Animal> deep_copy_shared() override {
        PYBIND11_OVERLOAD_PURE(
            std::shared_ptr<Animal>,
            Animal,
            deep_copy,
        );
    }

    void say_hi() override {
        PYBIND11_OVERLOAD_PURE(
            void,           /* Return type */
            Animal,         /* Parent class */
            say_hi,         /* Name of function in C++ (must match Python name) */
        );
    }

    int use_count() override {
        PYBIND11_OVERLOAD(
            int,
            Animal,
            use_count,
        );
    }
};


class Dog : public Animal {
public:
    std::string go(int n_times) override {
        std::string result;
        for (int i=0; i<n_times; ++i)
            result += "woof! ";
        return result;
    }

    void say_hi(){
        std::cout << "hi puppy" << std::endl;
    }

    Animal* deep_copy_raw(){
        return new Dog;
    }

    std::shared_ptr<Animal> deep_copy_shared(){
        return std::make_shared<Dog>();
    }

    int use_count(){
        return shared_from_this().use_count() - 1;
    }

};

class Container:public std::enable_shared_from_this<Container>{
private:
    std::vector<std::shared_ptr<Animal>> _v;
public:
    void add(std::shared_ptr<Animal>& sp){
        _v.push_back(sp);
    }
    void exec_last(){
        auto sp = _v.back();
        sp->say_hi();
        _v.pop_back();
    }

};
#endif
