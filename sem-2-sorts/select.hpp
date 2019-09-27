#include <cstdlib>
#include "sort.hpp"

template <typename T>
class SelectionSort : Sort {
    private: 
        bool* (T*, T*) _compareFunc; 
    public:
        Sort(bool* (T*, T*) func) : Sort {
            _compareFunc = func;
        };
        ~Sort();
        virtual void sort(T* tmpl) : Sort {
            
        };
};
