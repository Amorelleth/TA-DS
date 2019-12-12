#include <iostream>
#include <fstream>
#include <time.h>
#include <omp.h>

// g++-8 -std=c++17 -g main.cpp -fopenmp -o main && ./main && ./main

const int ITEM = 101;
std::string OUTPUT_FILE = "data2.txt";

void fill_with_random(int *a, const unsigned long long int size) {
    srand(time(nullptr));
    for (auto i = 0; i < size; i++) {
        a[i] = rand() % 100;
    }
    a[rand() % size] = ITEM;
}

float search(unsigned long long int size, int* arr) {
    fill_with_random(arr, size);
    auto idx = -1;
    auto start_time = clock();
    for (auto i = 0; i < size; i++) {
        if (arr[i] == ITEM) {
            idx = i;
            break;
        }
    }
    auto end_time = clock();
    if (idx == -1) throw -1;
    return (float)(end_time - start_time) /  CLOCKS_PER_SEC;
}

void avg_search(unsigned long long int size, int* arr) {
    float avg_time = 0;
    for(int j = 0; j < 10; j++) {
        avg_time += search(size, arr);
    }
    avg_time /= 10;
    std::fstream data;
    data.open(OUTPUT_FILE, std::ios::app);
    data << size << " " << avg_time << " " << std::endl;
    data.close();
}

float parallel_search(unsigned long long int size, int* arr, int _num_threads = 8) {
    fill_with_random(arr, size);
    auto idx = -1;
    auto start_time = omp_get_wtime();
    #pragma omp parallel num_threads(_num_threads) shared(idx, arr)
    {
        #pragma omp for
        for (auto i = 0; i < size; i++) {
            if (arr[i] == ITEM) {
                idx = i;
            }
        }
    }
    auto end_time = omp_get_wtime();
    if (idx == -1) throw -1;
    return (float)(end_time - start_time);
}


void avg_parallel_search(unsigned long long int size, int* arr, int num = 8) {
    float avg_time = 0;
    for(int j = 0; j < 10; j++) {
        avg_time += parallel_search(size, arr, num);
    }
    avg_time /= 10;
    std::fstream data;
    data.open(OUTPUT_FILE, std::ios::app);
    data << size << " " << avg_time << " " << std::endl;
    data.close();
}

int main(int argc, char **argv) {
    if (argc > 1) {
        OUTPUT_FILE = argv[1];
    }

    auto TEN = 10;
    auto HUNDRED = 100;
    auto THOUSAND = 1000;
    auto TEN_THOUSAND = 10000;
    auto HUNDRED_THOUSAND = 100000;
    auto MILLION = 1000000;

    int* TEN_ARR = new int[TEN];
    int* HUNDRED_ARR = new int[HUNDRED];
    int* THOUSAND_ARR = new int[THOUSAND];
    int* TEN_THOUSAND_ARR = new int[TEN_THOUSAND];
    int* HUNDRED_THOUSAND_ARR = new int[HUNDRED_THOUSAND];
    int* MILLION_ARR = new int[MILLION];
    int* TEN_MILLION_ARR = new int[MILLION*10];
    int* TWO_MILLION_ARR = new int[MILLION*2];
    int* THREE_MILLION_ARR = new int[MILLION*3];
    int* FOUR_MILLION_ARR = new int[MILLION*4];
    int* FIVE_MILLION_ARR = new int[MILLION*5];
    int* SIX_MILLION_ARR = new int[MILLION*6];
    int* SEVEN_MILLION_ARR = new int[MILLION*7];
    int* EIGHT_MILLION_ARR = new int[MILLION*8];
    int* NINE_MILLION_ARR = new int[MILLION*9];

    std::ofstream data;
    data.open(OUTPUT_FILE, std::ios::out | std::ios::trunc);
    data.close();

    try {
        for (int i = 1; i < 17; i++) {
            float avg_time = 0;
            for(int j = 0; j < 10; j++) {
                avg_time += parallel_search(10*MILLION, TEN_MILLION_ARR, i);
            }
            avg_time /= 10;
            std::fstream data;
            data.open(OUTPUT_FILE, std::ios::app);
            data << 10*MILLION << " " << avg_time << " " << i << std::endl;
            data.close();
        }

        avg_parallel_search(TEN, TEN_ARR);
        avg_parallel_search(HUNDRED, HUNDRED_ARR);
        avg_parallel_search(THOUSAND, THOUSAND_ARR);
        avg_parallel_search(TEN_THOUSAND, TEN_THOUSAND_ARR);
        avg_parallel_search(HUNDRED_THOUSAND, HUNDRED_THOUSAND_ARR);
        avg_parallel_search(MILLION, MILLION_ARR);
        avg_parallel_search(2*MILLION, TWO_MILLION_ARR);
        avg_parallel_search(3*MILLION, THREE_MILLION_ARR);
        avg_parallel_search(4*MILLION, FOUR_MILLION_ARR);
        avg_parallel_search(5*MILLION, FIVE_MILLION_ARR);
        avg_parallel_search(6*MILLION, SIX_MILLION_ARR);
        avg_parallel_search(7*MILLION, SEVEN_MILLION_ARR);
        avg_parallel_search(8*MILLION, EIGHT_MILLION_ARR);
        avg_parallel_search(9*MILLION, NINE_MILLION_ARR);
        avg_parallel_search(10*MILLION, TEN_MILLION_ARR);

        avg_parallel_search(TEN, TEN_ARR, 1);
        avg_parallel_search(HUNDRED, HUNDRED_ARR, 1);
        avg_parallel_search(THOUSAND, THOUSAND_ARR, 1);
        avg_parallel_search(TEN_THOUSAND, TEN_THOUSAND_ARR, 1);
        avg_parallel_search(HUNDRED_THOUSAND, HUNDRED_THOUSAND_ARR, 1);
        avg_parallel_search(MILLION, MILLION_ARR, 1);
        avg_parallel_search(2*MILLION, TWO_MILLION_ARR, 1);
        avg_parallel_search(3*MILLION, THREE_MILLION_ARR, 1);
        avg_parallel_search(4*MILLION, FOUR_MILLION_ARR, 1);
        avg_parallel_search(5*MILLION, FIVE_MILLION_ARR, 1);
        avg_parallel_search(6*MILLION, SIX_MILLION_ARR, 1);
        avg_parallel_search(7*MILLION, SEVEN_MILLION_ARR, 1);
        avg_parallel_search(8*MILLION, EIGHT_MILLION_ARR, 1);
        avg_parallel_search(9*MILLION, NINE_MILLION_ARR, 1);
        avg_parallel_search(10*MILLION, TEN_MILLION_ARR, 1);

        avg_search(TEN, TEN_ARR);
        avg_search(HUNDRED, HUNDRED_ARR);
        avg_search(THOUSAND, THOUSAND_ARR);
        avg_search(TEN_THOUSAND, TEN_THOUSAND_ARR);
        avg_search(HUNDRED_THOUSAND, HUNDRED_THOUSAND_ARR);
        avg_search(MILLION, MILLION_ARR);
        avg_search(2*MILLION, TWO_MILLION_ARR);
        avg_search(3*MILLION, THREE_MILLION_ARR);
        avg_search(4*MILLION, FOUR_MILLION_ARR);
        avg_search(5*MILLION, FIVE_MILLION_ARR);
        avg_search(6*MILLION, SIX_MILLION_ARR);
        avg_search(7*MILLION, SEVEN_MILLION_ARR);
        avg_search(8*MILLION, EIGHT_MILLION_ARR);
        avg_search(9*MILLION, NINE_MILLION_ARR);
        avg_search(10*MILLION, TEN_MILLION_ARR);
    } catch (int a) {
        std::cout << "Item Was Not Found =(" << std::endl;
    }

    return 0;
}
