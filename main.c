#include <stdio.h>
#include <stdlib.h>

// pre-defined number of processes
#define SIZE 5

// To print a horizontal rule
void println() {
	printf("\n-----------------------------------------------------------------------------------------------------\n");
	//printf("\n_____________________________________________________________________________________________________\n");
	//printf("\n.....................................................................................................\n");
	//printf("\n--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--x--\n");
	return;
}

// To print elements of an array
void print_arr(int arr[]) {
	for (int i = 0; i < SIZE; i++) {
		printf("%d	", arr[i]);
	}
	return;
}

// Returns the smallest element's index in an array (only for priority scheduling [Non-preemptive])
int minimum_element_index(int arr[]) {
	int index = 0;
	int min = arr[0];
	// finds the minimum element in an array
	for (int i = 0; i < SIZE; i++) {
		if (arr[i] < min) {
			min = arr[i];
		}
	}
	// finds index of the minimum element in an array
	for (int i = 0; i < SIZE; i++) {
		if (arr[i] == min) {
			index = i;
			break;
		}
	}
	arr[index] = INT_MAX;
	return index;
}

// To swap values between two variables
void swap(int* xp, int* yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
	return;
}

// Selection Sort (for SJF scheduling)
void selection_sort(int arr[], int n)
{
	int i, j, min_idx;

	// One by one move boundary of unsorted subarray 
	for (i = 0; i < n - 1; i++)
	{
		// Find the minimum element in unsorted array 
		min_idx = i;
		for (j = i + 1; j < n; j++)
			if (arr[j] < arr[min_idx])
				min_idx = j;

		// Swap the found minimum element with the first element 
		if (min_idx != i)
			swap(&arr[min_idx], &arr[i]);
	}
	return;
}

// To return pointer to an array with arrival-times using burst-times as argument
int* arrival(int arr[]) {
	static int ret[SIZE];
	int arrival = 0;
	for (int i = 0; i < SIZE; ++i) {
		ret[i] = arrival;
		arrival += arr[i];
	}
	return ret;
}

// To return pointer to an array with completion-times using burst-times as argument
int* completion(int arr[]) {
	static int ret[SIZE];
	int completion = 0;
	for (int i = 0; i < SIZE; ++i) {
		completion += arr[i];
		ret[i] = completion;
	}
	return ret;
}

// FCFS scheduling
void fcfs(int burst_times[]) {
	// Pointer to calculated arrival-times
	int* arrival_times;
	arrival_times = arrival(burst_times);

	printf("\n\nFCFS scheduling for the given burst times: ");
	println();
	printf("Waiting-times (in ms):				");
	print_arr(arrival_times);

	float avg_waiting_time = 0;

	for (int i = 0; i < SIZE; ++i) {
		avg_waiting_time += arrival_times[i];
	}
	println();

	avg_waiting_time /= SIZE;
	printf("Avg waiting-time (in ms):			%0.2f", avg_waiting_time);
	println();
	return;
}

// SJF Scheduling
void sjf(int burst_times[]) {
	// Pointer to calculated arrival-times
	int* arrival_times;
	// To hold burst-times without modifying the global array
	int temp_times[SIZE];

	for (int i = 0; i < SIZE; ++i) {
		temp_times[i] = burst_times[i];
	}

	arrival_times = arrival(burst_times);

	printf("\n\nSJF scheduling for the given burst times: ");
	println();
	printf("Waiting-times for FCFS scheduling (in ms):	");
	print_arr(arrival_times);

	selection_sort(temp_times, SIZE);
	arrival_times = arrival(temp_times);
	println();
	printf("Waiting-times for SJF scheduling (in ms):	");
	print_arr(arrival_times);

	float avg_waiting_time = 0;

	for (int i = 0; i < SIZE; ++i) {
		avg_waiting_time += arrival_times[i];
	}
	println();

	avg_waiting_time /= SIZE;
	printf("Avg waiting-time (in ms):			%0.2f", avg_waiting_time);
	println();
	return;
}

// SRJF Scheduling
void srjf(int burst_times[], int arrival_times[]) {
	int total_time = 0;
	int completion_times[SIZE];
	int TAT_times[SIZE];
	int waiting_times[SIZE];
	printf("\n\nSRJF (Preemptive SJF) scheduling for the given burst times: ");
	println();
	printf("Burst-times (in ms):				");
	print_arr(burst_times); // Prints burst-times
	println();
	printf("Arrival-times (in ms):				");
	print_arr(arrival_times); // Prints arrival-times
	println();

	for (int i = 0; i < SIZE; i++) {
		total_time += burst_times[i];
	}
	printf("Total CPU Execution Time (in ms):		%d", total_time);

	int counter = 0;
	int arrival_counter = 0;
	int* arr = (int*)malloc(sizeof(NULL));
	while (counter < total_time) {
		if (/*counter < SIZE*/arrival_times[arrival_counter] == counter) {
			int* tmp = (int*)realloc(arr, (arrival_counter+1) * sizeof(int));
			if (tmp != NULL) {
				arr = tmp;
			}
			else {
				printf("\nReallocated NULL\n");
				exit(1);
			}
			arr[arrival_counter] = burst_times[arrival_counter];
			arrival_counter++;
		}
		int condition = (arrival_counter - 1 < SIZE/*arrival_times[arrival_counter] == counter*/) ? arrival_counter : SIZE;
		int index = 0;
		int min = INT_MAX;
		// finds the minimum element in an array
		for (int i = 0; i < condition; ++i) {
			if (arr[i] < min && arr[i] != 0) {
				min = arr[i];
			}
		}
		// finds index of the minimum element in an array
		for (int i = 0; i < condition; i++) {
			if (arr[i] == min) {
				index = i;
				break;
			}
		}
		arr[index]--;

		//printf("\nArr:	");
		//for (int i = 0; i < condition; i++) {
		//	printf("%d ", arr[i]);
		//}
		//printf("\n");
		//printf("Index:	%d\n", index);
		//printf("Counter:%d\n", counter);

		if (arr[index] == 0) {
			completion_times[index] = counter + 1;
		}
		counter++;
	}
	free(arr);
	println();
	printf("Completion-times (in ms):			");
	for (int i = 0; i < SIZE; i++) {
		printf("%d	", completion_times[i]);
	}
	println();
	printf("Turnaround-times (in ms):			");
	for (int i = 0; i < SIZE; i++) {
		TAT_times[i] = completion_times[i] - arrival_times[i];
		printf("%d	", TAT_times[i]);
	}
	println();
	printf("Waiting-times (in ms):				");
	for (int i = 0; i < SIZE; i++) {
		waiting_times[i] = TAT_times[i] - burst_times[i];
		printf("%d	", waiting_times[i]);
	}
	float avg_waiting_time = 0;

	for (int i = 0; i < SIZE; ++i) {
		avg_waiting_time += waiting_times[i];
	}
	println();

	avg_waiting_time /= SIZE;
	printf("Avg waiting-time (in ms):			%0.2f", avg_waiting_time);
	println();
	return;
}

// Priority Scheduling (Non-Preemptive)
void priority(int burst_times[], int priority_order[]) {
	int priority_list[SIZE];
	for (int i = 0; i < SIZE; i++) {
		priority_list[i] = priority_order[i];
	}
	int waiting_times[SIZE];
	printf("\n\nPriority scheduling (Non-Preemptive) for the given burst times: ");
	println();
	printf("Burst-times (in ms):				");
	print_arr(burst_times); // Prints burst-times
	println();
	printf("Priority:					");
	print_arr(priority_list); // Prints priority
	println();
	int counter = 0;
	for (int i = 0; i < SIZE; i++) {
		int value = minimum_element_index(priority_list);
		waiting_times[value] = counter;
		counter += burst_times[value];
	}
	printf("Waiting-times:					");
	print_arr(waiting_times); // Prints burst-times
	float avg_waiting_time = 0;

	for (int i = 0; i < SIZE; ++i) {
		avg_waiting_time += waiting_times[i];
	}
	println();

	avg_waiting_time /= SIZE;
	printf("Avg waiting-time (in ms):			%0.2f", avg_waiting_time);
	println();
	return;
}

// Priority Scheduling (Preemptive)
void priority_preemptive(int burst_times[], int priority_list[], int arrival_times[]) {
	int total_time = 0;
	int completion_times[SIZE];
	int TAT_times[SIZE];
	int waiting_times[SIZE];

	printf("\n\nPriority scheduling (Preemptive) for the given burst times: ");
	println();
	printf("Burst-times (in ms):				");
	print_arr(burst_times); // Prints burst-times
	println();
	printf("Arrival-times (in ms):				");
	print_arr(arrival_times); // Prints arrival-times
	println();
	printf("Priority:					");
	print_arr(priority_list); // Prints priority order
	println();
	for (int i = 0; i < SIZE; i++) {
		total_time += burst_times[i];
	}
	printf("Total CPU Execution Time (in ms):		%d", total_time);
	int counter = 0;
	int arrival_counter = 0;
	int* arr = (int*)malloc(sizeof(NULL));
	int* pri = (int*)malloc(sizeof(NULL));
	while (counter < total_time) {
		if (/*counter < SIZE*/arrival_times[arrival_counter] == counter) {
			int* tmp =     (int*)realloc(arr, (arrival_counter + 1) * sizeof(int));
			int* pri_tmp = (int*)realloc(pri, (arrival_counter + 1) * sizeof(int));
			if (tmp != NULL && pri_tmp != NULL) {
				arr = tmp;
				pri = pri_tmp;
			}
			else {
				printf("\nReallocated NULL\n");
				exit(1);
			}
			arr[arrival_counter] = burst_times[arrival_counter];
			pri[arrival_counter] = priority_list[arrival_counter];
			++arrival_counter;
		}
		int condition = ((arrival_counter - 1) < SIZE/*arrival_times[arrival_counter] == counter*/) ? arrival_counter : SIZE;
		int index = 0;
		int min = INT_MAX;
		// finds the minimum element in an array
		for (int i = 0; i < condition; ++i) {
			if (pri[i] < min && arr[i] != 0) {
				min = pri[i];
			}
		}
		// finds index of the minimum element in an array
		int* check_arr = (int*)malloc(sizeof(NULL));
		for (int i = 0; i < condition; i++) {
			if (pri[i] == min) {
				int* check_tmp = (int*)realloc(check_arr, (1 * sizeof(int)));
				if (check_tmp != NULL) {
					check_arr = check_tmp;
				}
				else {
					printf("\nReallocated NULL\n");
					exit(1);
				}
				check_arr[0] = i;
				int j_counter = 1;
				for (int j = 0; j < condition; j++) {
					if ((pri[j] == pri[i]) && (j != i)) {
						int* check_tmp_2 = (int*)realloc(check_arr, ((j_counter + 1) * sizeof(int)));
						if (check_tmp_2 != NULL) {
							check_arr = check_tmp_2;
						}
						else {
							printf("\nReallocated NULL\n");
							exit(1);
						}
						check_arr[j_counter] = j;
						j_counter++;
					}
				}
				int check_index = 0;
				int check_min = check_arr[0];
				// finds the minimum element in an array
				for (int k = 0; k < j_counter; ++k) {
					if ((arr[check_arr[k]] < arr[check_min]) && (check_arr[k] != INT_MAX) && (arr[check_arr[k]] != 0)) {
						check_min = check_arr[k];
					}
				}
				for (int k = 0; k < j_counter; k++) {
					if (check_arr[k] == check_min) {
						check_index = k;
						break;
					}
				}
				index = check_arr[check_index];
				break;
			}
		}
		free(check_arr);
		arr[index]--;
		/*printf("\nArr:	");
		for (int i = 0; i < condition; i++) {
			printf("%d ", arr[i]);
		}
		printf("\nPri:	");
		for (int i = 0; i < condition; i++) {
			printf("%d ", pri[i]);
		}
		printf("\n");
		printf("Index:	%d\n", index);
		printf("Counter:%d\n", counter);*/

		if (arr[index] == 0) {
			completion_times[index] = counter + 1;
		}
		counter++;
	}
	free(arr);
	free(pri);
	println();
	printf("Completion-times (in ms):			");
	for (int i = 0; i < SIZE; i++) {
		printf("%d	", completion_times[i]);
	}
	println();
	printf("Turnaround-times (in ms):			");
	for (int i = 0; i < SIZE; i++) {
		TAT_times[i] = completion_times[i] - arrival_times[i];
		printf("%d	", TAT_times[i]);
	}
	println();
	printf("Waiting-times (in ms):				");
	for (int i = 0; i < SIZE; i++) {
		waiting_times[i] = TAT_times[i] - burst_times[i];
		printf("%d	", waiting_times[i]);
	}
	float avg_waiting_time = 0;

	for (int i = 0; i < SIZE; ++i) {
		avg_waiting_time += waiting_times[i];
	}
	println();

	avg_waiting_time /= SIZE;
	printf("Avg waiting-time (in ms):			%0.2f", avg_waiting_time);
	println();
	return;

}

int main() {
	// Declared burst-times
	int burst_times[SIZE] = { 4, 3, 1, 5, 2 }; // Insert your burst times here
	printf("Practical 3: CPU Scheduling Problems");
	println();
	printf("Operating Systems: CSE3003");
	println();
	printf("Aaditya Singh: 21BCE10768");
	println();
	printf("Burst-times (in ms):				");
	print_arr(burst_times); // Prints burst-times
	println();
	
	fcfs(burst_times);
	sjf(burst_times);

	int arrival_times[SIZE] = { 0, 1, 2, 3, 4 }; // Insert your preemptive arrival times
	srjf(burst_times, arrival_times);

	int priority_list[SIZE] = { 2, 3, 4, 5, 5 };      // Insert the priority list
	priority(burst_times, priority_list);

	priority_preemptive(burst_times, priority_list, arrival_times);

	printf("\n");
	return 0;
}
