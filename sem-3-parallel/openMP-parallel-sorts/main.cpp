#include <iostream>
#include <omp.h>
#include <time.h>

// g++-8 -std=c++17 -g main.cpp -fopenmp -o main && ./main
const int SIZE = 10000;

void fill_with_random(int *a, const unsigned long long int size) {
    for (auto i = 0; i < size; i++) {
        a[i] = rand() % 100;
    }
}

void print_array(int* array, int size) {
    for (auto i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

float odd_even_sort(int* array, int size, int p = 8) {
    auto start_time = omp_get_wtime();

    #pragma omp parallel num_threads(p) shared(array, size)
    {
        for (int i = 0; i <= size / 2; i++) {
        #pragma omp for
            for (int j = 0; j < size - size % 2; j += 2) {
                if (array[j] > array[j + 1]) {
                    std::swap(array[j], array[j + 1]);
                }
            }
        #pragma omp for
            for (int j = 1; j < size - size % 2 - 1; j += 2) {
                if (array[j] > array[j + 1]) {
                    std::swap(array[j], array[j + 1]);                    
                }
            }
        }
    }
    return (float)(omp_get_wtime() - start_time);
}

float shell_sort(int* array, int size, int p = 8) {
    auto start_time = omp_get_wtime();

    #pragma omp parallel num_threads(p) shared(size)
    {
        int step = size / 2;
        while (step > 0) {
        #pragma omp for
            for (int i = 0; i < (size - step); i++) {
                int j = i;
                while (j >= 0 && array[j] > array[j + step]) {
                    std::swap(array[j], array[j + step]);
                    j--;
                }
            }
            step /= 2;
        }
    }
    return (float)(omp_get_wtime() - start_time);
}

int main() {
    srand(time(nullptr));

    int* array = new int[SIZE];
    float odd_even_time, shell_time;

    for (auto p = 1; p < 9; p++) {
        odd_even_time = 0;
        shell_time = 0;

        for (auto i = 0; i < 100; i++){
            // fill_with_random(array, SIZE);
            // odd_even_time += odd_even_sort(array, SIZE, p);
            fill_with_random(array, SIZE);
            shell_time += shell_sort(array, SIZE, p);
        }
        std::cout << "num_threads: " << p << " " << odd_even_time / 100 << " " << shell_time / 100 << std::endl;
    }
}
