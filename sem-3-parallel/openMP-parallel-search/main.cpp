#include <iostream>
#include <fstream>
#include <ctime>
#include <omp.h>

const unsigned long long int TEN = 10;
const unsigned long long int HUNDRED = 100;
const unsigned long long int THOUSAND = 1000;
const unsigned long long int TEN_THOUSAND = 10000;
const unsigned long long int HUNDRED_THOUSAND = 100000;
const unsigned long long int MILLION = 1000000;
const unsigned long long int TEN_MILLION = 10000000;
const unsigned long long int HUNDRED_MILLION = 100000000;
const unsigned long long int BILLION = 1000000000;

int TEN_ARR[TEN];
int HUNDRED_ARR[HUNDRED];
int THOUSAND_ARR[THOUSAND];
int TEN_THOUSAND_ARR[TEN_THOUSAND];
int HUNDRED_THOUSAND_ARR[HUNDRED_THOUSAND];
int MILLION_ARR[MILLION];
int TEN_MILLION_ARR[TEN_MILLION];
int HUNDRED_MILLION_ARR[HUNDRED_MILLION];
int BILLION_ARR[BILLION];

const int ITEM = 101;
std::string OUTPUT_FILE = "data2.txt";

void fill_with_random(int *a, const unsigned long long int size) {
    srand(time(nullptr));
    for (unsigned long long int i = 0; i < size; i++) {
        a[i] = rand() % 100;
    }
    a[rand() % size] = ITEM;
}

void search(unsigned long long int size, int* arr) {
    unsigned long long int idx;
    double start_time;
    double end_time;
    std::ofstream data;

    fill_with_random(arr, size);

    idx = -1;
    start_time = clock();
    for (unsigned long long int i = 0; i < size; i++) {
        if (arr[i] == ITEM) {
            idx = i;
            break;
        }
    }
    end_time = clock();
    
    data.open(OUTPUT_FILE, std::ios::app);
    data << "0 " << size << " " << (float)(end_time - start_time) /  CLOCKS_PER_SEC;
    data.close();

    arr[idx] = 0;
    arr[0] = ITEM;
    idx = -1;
    start_time = clock();
    for (unsigned long long int i = 0; i < size; i++) {
        if (arr[i] == ITEM) {
            idx = i;
            break;
        }
    }
    end_time = clock();
    
    data.open(OUTPUT_FILE, std::ios::app);
    data << " " << (float)(end_time - start_time) /  CLOCKS_PER_SEC;
    data.close();

    arr[idx] = 0;
    arr[size - 1] = ITEM;
    idx = -1;
    start_time = clock();
    for (unsigned long long int i = 0; i < size; i++) {
        if (arr[i] == ITEM) {
            idx = i;
            break;
        }
    }
    end_time = clock();
    
    data.open(OUTPUT_FILE, std::ios::app);
    data << " " << (float)(end_time - start_time) /  CLOCKS_PER_SEC << std::endl;
    data.close();

    if (idx == -1) {
        throw -1;
    }
}

void parallel_search(unsigned long long int size, int* arr, int _num_threads = 8, char ch = '1') {
    unsigned long long int idx;
    double start_time;
    double end_time;
    std::ofstream data;

    fill_with_random(arr, size);
    
    idx = -1;
    start_time = clock();
    #pragma omp parallel num_threads(_num_threads) shared(idx, arr)
    {
        #pragma omp for
        for (unsigned long long int i = 0; i < size; i++) {
            if (arr[i] == ITEM) {
                idx = i;
                end_time = clock();
            }
        }
    }

    data.open(OUTPUT_FILE, std::ios::app);
    data << ch << " " << size << " " << (float)(end_time - start_time) /  CLOCKS_PER_SEC;
    data.close();

    idx = -1;
    arr[0] = ITEM;
    start_time = clock();
    #pragma omp parallel num_threads(_num_threads) shared(idx, arr)
    {
        #pragma omp for
        for (unsigned long long int i = 0; i < size; i++) {
            if (arr[i] == ITEM) {
                idx = i;
                end_time = clock();
            }
        }
    }

    data.open(OUTPUT_FILE, std::ios::app);
    data << " " << (float)(end_time - start_time) /  CLOCKS_PER_SEC;
    data.close();

    idx = -1;
    arr[size - 1] = ITEM;
    start_time = clock();
    #pragma omp parallel num_threads(_num_threads) shared(idx, arr)
    {
        #pragma omp for
        for (unsigned long long int i = 0; i < size; i++) {
            if (arr[i] == ITEM) {
                idx = i;
                end_time = clock();
            }
        }
    }

    data.open(OUTPUT_FILE, std::ios::app);
    data << " " << (float)(end_time - start_time) /  CLOCKS_PER_SEC;
    ch == '2' ? data << " " << _num_threads << std::endl : data << std::endl;
    data.close();

    if (idx == -1) {
        throw -1;
    }
}

int main(int argc, char **argv) {
    if (argc > 0) {
        OUTPUT_FILE = argv[1];
    }

    std::ofstream data;
    data.open(OUTPUT_FILE, std::ios::out | std::ios::trunc);
    data.close();

    try {
        search(TEN, TEN_ARR);
        search(HUNDRED, HUNDRED_ARR);
        search(THOUSAND, THOUSAND_ARR);
        search(TEN_THOUSAND, TEN_THOUSAND_ARR);
        search(HUNDRED_THOUSAND, HUNDRED_THOUSAND_ARR);
        search(MILLION, MILLION_ARR);
        search(TEN_MILLION, TEN_MILLION_ARR);
        search(HUNDRED_MILLION, HUNDRED_MILLION_ARR);
        search(BILLION, BILLION_ARR);

        parallel_search(TEN, TEN_ARR);
        parallel_search(HUNDRED, HUNDRED_ARR);
        parallel_search(THOUSAND, THOUSAND_ARR);
        parallel_search(TEN_THOUSAND, TEN_THOUSAND_ARR);
        parallel_search(HUNDRED_THOUSAND, HUNDRED_THOUSAND_ARR);
        parallel_search(MILLION, MILLION_ARR);
        parallel_search(TEN_MILLION, TEN_MILLION_ARR);
        parallel_search(HUNDRED_MILLION, HUNDRED_MILLION_ARR);
        parallel_search(BILLION, BILLION_ARR);

        parallel_search(BILLION, BILLION_ARR, 2, '2');
        parallel_search(BILLION, BILLION_ARR, 3, '2');
        parallel_search(BILLION, BILLION_ARR, 4, '2');
        parallel_search(BILLION, BILLION_ARR, 5, '2');
        parallel_search(BILLION, BILLION_ARR, 6, '2');
        parallel_search(BILLION, BILLION_ARR, 7, '2');        
        parallel_search(BILLION, BILLION_ARR, 8, '2');
    } catch (int a) {
        std::cout << "Item Was Not Found =(" << std::endl;
    }

    return 0;
}
