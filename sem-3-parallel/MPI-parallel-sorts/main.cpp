#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <ctime>

int* split(int arr[], int arrSize1, int arrSize2, int ind) {
	int *result;

	if (ind == 1) {
		result = new int[arrSize1];
		for (int i = 0; i < arrSize1; i++) {
			result[i] = arr[i];
		}
	} else {
		result = new int[arrSize2];
		for (int i = 0; i < arrSize2; i++) {
			result[i] = arr[i + arrSize1];
		}
	}
	return result;
}

void initArray(int arr[], int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		arr[i] = rand() % 100;
	}
}

void printArray(int arr[], int arrSize) {
	for (int i = 0; i < arrSize; i++) {
		std::cout << arr[i] << " " << std::endl;
	}
}

void shellSort(int arr[], int n) { 
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i += 1) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap]; 
            }
            arr[j] = temp; 
        }
    } 
} 

int* merge(int arr1[], int arrSize1, int arr2[], int arrSize2) {
	int i = 0;
	int j = 0;
	int current = 0;

	int* result = new int[arrSize1 + arrSize2];

	while ((i < arrSize1) && (j < arrSize2)) {
		if (arr1[i] > arr2[j]) {
			result[current] = arr2[j];
			j++;
		} else {
			result[current] = arr1[i];
			i++;
		}
		current++;
	}

	if (i == arrSize1) {
		while (j < arrSize2) {
			result[current] = arr2[j];
			j++;
			current++;
		}
	}
	else {
		while (i < arrSize1) {
			result[current] = arr1[i];
			i++;
			current++;
		}
	}
	return result;
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));

	int *arr, *arrLocal, *arrNear;
	int *sendCounts, *displs;
	int arrSize, arrSizeLocal, rest, arrNearSize;
	int ProcNum, ProcRank;
	MPI_Status status;
	double start, end;

	arr = NULL;
	arrSize = 10000000;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	if (ProcRank == 0) {
		arr = new int[arrSize];
		initArray(arr, arrSize);
	}

	arrSizeLocal = arrSize / ProcNum;
	rest = arrSize % ProcNum;

	sendCounts = new int[ProcNum];
	sendCounts[0] = arrSizeLocal + rest;
	for (int i = 1; i < ProcNum; i++) {
		sendCounts[i] = arrSizeLocal;
	}
	displs = new int[ProcNum];
	displs[0] = 0;
	for (int i = 1; i < ProcNum; i++) {
		displs[i] = i * arrSizeLocal + rest;
	}

	if (ProcRank == 0) {
		arrSizeLocal = arrSizeLocal + rest;
	}

	arrLocal = new int[arrSizeLocal];

	start = MPI_Wtime();

	MPI_Scatterv(arr, sendCounts, displs, MPI_INT, arrLocal, sendCounts[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
	shellSort(arrLocal, arrSizeLocal);
	
	for (int i = ProcNum; i > 0; i = i/2) {
		if ((ProcRank & i) == 0) {
			if (ProcRank + i < ProcNum) {
				MPI_Recv(&arrNearSize, 1, MPI_INT, ProcRank + i, 0, MPI_COMM_WORLD, &status);
				arrNear = new int[arrNearSize];
				MPI_Recv(arrNear, arrNearSize, MPI_INT, ProcRank + i, 0, MPI_COMM_WORLD, &status);
				arrLocal = merge(arrLocal, arrSizeLocal, arrNear, arrNearSize);
				arrNear = split(arrLocal, arrSizeLocal, arrNearSize, 2);
				arrLocal = split(arrLocal, arrSizeLocal, arrNearSize, 1);
				MPI_Send(arrNear, arrNearSize, MPI_INT, ProcRank + i, 1, MPI_COMM_WORLD);
			}
		} else {
			MPI_Send(&arrSizeLocal, 1, MPI_INT, ProcRank - i, 0, MPI_COMM_WORLD);
			MPI_Send(arrLocal, arrSizeLocal, MPI_INT, ProcRank - i, 0, MPI_COMM_WORLD);
			MPI_Recv(arrLocal, arrSizeLocal, MPI_INT, ProcRank - i, 1, MPI_COMM_WORLD, &status);
		}
	}
	
	for (int i = 0; i < ProcNum; i++) {
		if (i % 2 == 1) {
			if (ProcRank % 2 == 1) {
				if (ProcRank < ProcNum - 1) {
					MPI_Recv(&arrNearSize, 1, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD, &status);
					arrNear = new int[arrNearSize];
					MPI_Recv(arrNear, arrNearSize, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD, &status);
					arrLocal = merge(arrLocal, arrSizeLocal, arrNear, arrNearSize);
					arrNear = split(arrLocal, arrSizeLocal, arrNearSize, 2);
					arrLocal = split(arrLocal, arrSizeLocal, arrNearSize, 1);
					MPI_Send(arrNear, arrNearSize, MPI_INT, ProcRank + 1, 1, MPI_COMM_WORLD);
				}
			} else { 
				if (ProcRank > 0) {
					MPI_Send(&arrSizeLocal, 1, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
					MPI_Send(arrLocal, arrSizeLocal, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
					MPI_Recv(arrLocal, arrSizeLocal, MPI_INT, ProcRank - 1, 1, MPI_COMM_WORLD, &status);
				}
			}
		} else {
			if (ProcRank % 2 == 0) { 
				if (ProcRank < ProcNum - 1) {
					MPI_Recv(&arrNearSize, 1, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD, &status);
					arrNear = new int[arrNearSize];
					MPI_Recv(arrNear, arrNearSize, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD, &status);
					arrLocal = merge(arrLocal, arrSizeLocal, arrNear, arrNearSize);
					arrNear = split(arrLocal, arrSizeLocal, arrNearSize, 2);
					arrLocal = split(arrLocal, arrSizeLocal, arrNearSize, 1);
					MPI_Send(arrNear, arrNearSize, MPI_INT, ProcRank + 1, 1, MPI_COMM_WORLD);
				}
			}
			else {
				MPI_Send(&arrSizeLocal, 1, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
				MPI_Send(arrLocal, arrSizeLocal, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
				MPI_Recv(arrLocal, arrSizeLocal, MPI_INT, ProcRank - 1, 1, MPI_COMM_WORLD, &status);
			}
		}
	}

	end = MPI_Wtime();

	if (ProcRank == 0) {
		printf("time %f", end - start);
	}

	MPI_Finalize();
	return 0;
}
