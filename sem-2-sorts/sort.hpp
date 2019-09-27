#include <cstdlib>

template <typename T>
class Sort {
    public:
        Sort(bool* (T*, T*) func);
        ~Sort();
        virtual void sort(T* tmpl);
};