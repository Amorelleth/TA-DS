#include <cstdlib>
#include <iostream>
#include "sort.hpp"

template <typename T>
class BubbleSort : Sort {
    private: 
        bool (*f)(T*, T*) _compareFunc; 
    public:
        Sort(bool (*)(T*, T*) func) : Sort {
            _compareFunc = func;
        };
        ~Sort();
        sort(T* tmpl) : Sort {
            std::cout << "ok";
        };
};
