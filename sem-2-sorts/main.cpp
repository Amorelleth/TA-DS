#include <cstdlib>
#include <iostream>
#include "bubble.hpp"

bool compare_ints(int* a, int* b) {
    return *a < *b;
}

int main() {
    BubbleSort<int>* bubble = new BubbleSort<int>(&compare_ints);
}