#include <cstdlib>
#include "sort.hpp"

template <typename T>
class BubbleSort : Sort {
    private: 
        bool* (T*, T*) _compareFunc; 
    public:
        Sort(bool* (T*, T*) func) : Sort;
        ~Sort();
        void sort(T* tmpl) : Sort;
};
