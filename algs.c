/* Compendium of Algorithms Learnt in COMP10002
 * Insertion Sort added W4 Wednesday, 15/8/18.
 * Linear Search, Binary Search added W5 Wednesday, 22/8/18.
 * Bubble Sort, Selection Sort added W5 Thursday, 23/8/18.
 * Naive Match added W7 Thursday, 6/9/18.
 * Mergesort added W12 Wednesday, 17/10/18.
 * Quicksort, Heapsort, KMP Match, BMH Match added Saturday 26/10/18.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TRUE       1
#define FALSE      0
#define NOT_FOUND -1  // constant for returning from search functions
#define MAX_LEN   50  // maximum length of arrays read from stdin

// Sorting functions
void insertion_sort(int* array, int n);
void quicksort(int* array, int n);
void bubble_sort(int *array, int n);
void selection_sort(int *array, int n);
void mergesort(int* array, int n);
void heapsort(int* array, int n);

// Search functions
int linear_search(int* array, int n, int item);
int binary_search(int* array, int n, int item);

// Pattern matching functions
int naive_match(char* haystack, char* needle);
int kmp_match(char* haystack, char* needle);
int bmh_match(char* haystack, char* needle);
int suffix_match(char* haystack, char* needle);

// Helper functions
int* read_int_array(int max_len, int* len_ptr);
void print_int_array(int* array, int n);
int* ask_for_array(int maxlen, int* len_ptr);
char* ask_for_string(int maxlen, char* promptstr);
void int_swap(int* a, int* b);

void partition(int* array, int n, int pivot, int* first_eq, int* first_gt);
void mergesort_rec(int* array, int n, int* temp);
void sift_down(int* array, int i, int n);
int* kmp_shift_array(char* s, int s_len);
int* bmh_shift_array(char* s, int s_len);
char** make_suffix_array(char* s, int s_len);

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Error: Incorrect number of arguments given\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(argv[2], "search") == 0) {
		int (*search_f)(int*, int, int);
		if (strcmp(argv[1], "linear") == 0) search_f = linear_search;
		else if (strcmp(argv[1], "binary") == 0) search_f = binary_search;
		else {
			printf("Error: Search function '%s' not found\n", argv[1]);
			exit(EXIT_FAILURE);
		}

		printf("Enter a number to search for: ");
		fflush(stdout);
		int item;
		scanf(" %d", &item);
		printf("Got %d!\n", item);

		int *array, len;
		array = ask_for_array(MAX_LEN, &len);

		int result = search_f(array, len, item);
		if (result == NOT_FOUND) printf("Could not find %d in list!\n", item);
		else printf("Found %d at position %d!\n", item, result);

		free(array);
	}
	else if (strcmp(argv[2], "sort") == 0) {
		void (*sort_f)(int*, int);
		if (strcmp(argv[1], "insertion") == 0) sort_f = insertion_sort;
		else if (strcmp(argv[1], "quick") == 0) sort_f = quicksort;
		else if (strcmp(argv[1], "bubble") == 0) sort_f = bubble_sort;
		else if (strcmp(argv[1], "selection") == 0) sort_f = selection_sort;
		else if (strcmp(argv[1], "merge") == 0) sort_f = mergesort;
		else if (strcmp(argv[1], "heap") == 0) sort_f = heapsort;
		else {
			printf("Error: Sorting function '%s' not found\n", argv[1]);
			exit(EXIT_FAILURE);
		}

		int *array, len;
		array = ask_for_array(MAX_LEN, &len);

		sort_f(array, len);
		printf("Sorted list: ");
		print_int_array(array, len);

		free(array);
	}
	else if (strcmp(argv[2], "match") == 0) {
		int (*match_f)(char*, char*);
		if (strcmp(argv[1], "naive") == 0) match_f = naive_match;
		else if (strcmp(argv[1], "kmp") == 0) match_f = kmp_match;
		else if (strcmp(argv[1], "bmh") == 0) match_f = bmh_match;
		else {
			printf("Error: Match function '%s' not found\n", argv[1]);
			exit(EXIT_FAILURE);
		}

		char* haystack, *needle;
		haystack = ask_for_string(MAX_LEN, "haystack");
		needle = ask_for_string(MAX_LEN, "needle");

		int result = match_f(haystack, needle);
		if (result == NOT_FOUND) printf("Could not find '%s' in string!\n", needle);
		else printf("Found '%s' at position %d!\n", needle, result);

		free(haystack);
		free(needle);
	}
	else {
		printf("Error: Invalid argument(s)\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

// --- SEARCH FUNCTIONS ---

// Linear Search | Best: O(1), Average: O(n), Worst: O(n)
int linear_search(int* array, int n, int item) {
	for (int i = 0; i < n; i++) {
		if (array[i] == item) return i;
	}
	return NOT_FOUND;
}

// Binary Search | Best: O(1), Average: O(log n), Worst: O(log n)
int binary_search(int* array, int n, int item) {
	int lo = 0;
	int hi = n;  // n-1 is wrong -- why is that again?
	int mid = (lo + hi) / 2;
	while (lo < hi) {
		if (item < array[mid]) hi = mid;
		else if (item > array[mid]) lo = mid + 1;
		else return mid;
		mid = (lo + hi) / 2;
	}
	return NOT_FOUND;
}

// --- SORTING FUNCTIONS ---

// Insertion Sort | Best: O(n), Average: O(n^2), Worst: O(n^2)
void insertion_sort(int* array, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = i; j > 0 && array[j] < array[j-1]; j--) {
			int_swap(array+j, array+j-1);
		}
	}
}

// Quicksort | Best: O(nlog n), Average: O(nlog n), Worst: O(n^2)
void quicksort(int* array, int n) {
	if (n <= 1) return;
	int first_eq, first_gt, pivot = rand() % n;
	partition(array, n, array[pivot], &first_eq, &first_gt);
	quicksort(array, first_eq);
	quicksort(array+first_gt, n-first_gt);
}

/* Partition for Quicksort: O(n) on average.
 * Puts everything less than pivot before first_eq, everything greater than
 * pivot at first_gt and after, and everything equal to pivot in between.
 */
void partition(int* array, int n, int pivot, int* first_eq, int* first_gt) { //TODO: check
	// Strategy: Have sections '<', '=', 'unknown', '>' in that order
	int next = 0, fe = 0, fg = n;
	while (next < fg) {
		if (array[next] > pivot) {
			fg--;
			int_swap(array+next, array+fg);
		}
		else {
			if (array[next] < pivot) {
				int_swap(array+next, array+fe);
				fe++;
			}
			next++;
		}
	}
	*first_eq = fe;
	*first_gt = fg;
}

// Bubble Sort | Best: O(n), Average: O(n^2), Worst: O(n^2)
void bubble_sort(int *array, int n) {
	int sorted, i = 0;
	do {
		sorted = TRUE;
		for (int j = 0; j < n-i-1; j++) {
			if (array[j] > array[j+1]) {
				sorted = FALSE;
				int_swap(array+j, array+j+1);
			}
		}
		i++;
	} while (!sorted);
}

// Selection Sort | Best: O(n^2), Average: O(n^2), Best: O(n^2)
void selection_sort(int* array, int n) {
	// Continually find the max element, then swap it with the last element
	int current_max, max_index;
	for (int i = 0; i < n - 1; i++) {
		max_index = 0;
		current_max = array[0];
		for (int j = 1; j < n - i; j++) {
			if (array[j] > current_max) {
				current_max = array[j];
				max_index = j;
			}
		}
		int_swap(array+max_index, array+n-i-1);
	}
}

// Mergesort | Best: O(nlog n), Average: O(nlog n), Best: O(nlog n)
void mergesort(int* array, int n) {
	int* temp = malloc(n/2 * sizeof(int));
	mergesort_rec(array, n, temp);
	free(temp);
}

void mergesort_rec(int* array, int n, int* temp) {
	if (n <= 1) return;
	int mid = n/2;
	mergesort_rec(array, mid, temp);
	mergesort_rec(array+mid, n-mid, temp);

	// Now merge the two sorted halves
	int i, a = 0, b = mid;
	// Make a copy of the first half
	for (i = 0; i < mid; i++) temp[i] = array[i];

	// Use the copied first half and remaining second half to merge
	for (i = 0; a < mid && b < n; i++) {
		if (temp[a] <= array[b]) array[i] = temp[a++];
		else array[i] = array[b++];
	}
	// Clean up any remaining items from the copied data
	while (a < mid) array[i++] = temp[a++];
}

// Heapsort | Best: O(nlog n), Average: O(nlog n), Best: O(nlog n)
void heapsort(int* array, int n) {
	// Build the heap
	int i;
	for (i = n/2-1; i >= 0; i--) {
		sift_down(array, i, n);
	}
	// Continue popping the largest item off and to the back
	for (i = n-1; i > 0; i--) {
		int_swap(array+0, array+i);
		sift_down(array, 0, i);
	}
}

void sift_down(int* array, int parent, int n) {
	// Strategy: mess with the heap to make it a priority queue
	int child;
	// Check if the left child exists
	while ((child = parent*2 + 1) < n) {
		// Select the right child if it exists and is larger
		if (child + 1 < n && array[child+1] > array[child]) child++;
		if (array[parent] < array[child]) {
			// Parent is smaller than the larger child, so swap and continue
			int_swap(array+parent, array+child);
			parent = child;
		}
		else break;
	}
}

// --- PATTERN MATCHING FUNCTIONS ---

// Naive Match | Best: O(n+m), Average: O(n+m), Worst: O(nm)
int naive_match(char* haystack, char* needle) {
	int haystack_len = strlen(haystack), needle_len = strlen(needle);
	if (needle_len > haystack_len) return NOT_FOUND;

	int i, j;
	for (i = 0; i <= haystack_len-needle_len; i++) {
		for (j = 0; j < needle_len; j++) {
			if (haystack[i+j] != needle[j]) break;
		}
		if (j == needle_len) return i;
	}
	return NOT_FOUND;
}

// KMP Match | Best: O(n+m), Average: O(n+m), Worst: O(n+m)
int kmp_match(char* haystack, char* needle) {
	int haystack_len = strlen(haystack), needle_len = strlen(needle);
	if (needle_len > haystack_len) return NOT_FOUND;
	int* shift_array = kmp_shift_array(needle, needle_len);

	int i = 0, j;
	while (i < haystack_len-needle_len) {
		for (j = 0; j < needle_len; j++) {
			if (haystack[i+j] != needle[j]) {
				i += j - shift_array[j];
			}
		}
		if (j == needle_len) {
			free(shift_array);
			return i;
		}
	}
	free(shift_array);
	return NOT_FOUND;
}

int* kmp_shift_array(char* s, int s_len) {
	int* shift_array = malloc(s_len * sizeof(int));
	shift_array[0] = -1;
	shift_array[1] = 0;
	int i = 0, j = 2;
	while (j < s_len) {
		if (s[j-1] == s[i]) {
			shift_array[j] = shift_array[j-1] + 1;
			j++;
			i++;
		}
		else {
			i = 0;
			if (s[j-1] == s[i]) {
				shift_array[j++] = 1;
				i++;
			}
			else shift_array[j++] = 0;
		}
	}
	printf("Got suffix array: ");
	print_int_array(shift_array, s_len);
	return shift_array;
}

// BMH Match | Best: O(n+m), Average: O(n+m), Worst: O(nm)
int bmh_match(char* haystack, char* needle) {
	int haystack_len = strlen(haystack), needle_len = strlen(needle);
	if (needle_len > haystack_len) return NOT_FOUND;
	int* shift_array = bmh_shift_array(needle, needle_len);

	int i = 0, j;
	while (i <= haystack_len-needle_len) {
		for (j = needle_len-1; j >= 0; j--) {
			if (haystack[i+j] != needle[j]) {
				i += shift_array[(int)haystack[i+j]] - (needle_len-1-j);
				break;
			}
		}
		if (j < 0) {
			free(shift_array);
			return i;
		}
	}
	free(shift_array);
	return NOT_FOUND;
}

int* bmh_shift_array(char* s, int s_len) {
	// Assume the alphabet is the ASCII character set (ie 0 to 255)
	const int alpha_len = 256;
	int* shift_array = malloc(alpha_len * sizeof(int));
	int i;
	for (i = 0; i < alpha_len; i++) shift_array[i] = s_len;
	for (i = 0; i < s_len-1; i++) shift_array[(int)s[i]] = s_len - i - 1;
	shift_array[(int)s[s_len-1]] = 1;
	printf("Got shift array: [");
	for (i = 0; i < s_len-1; i++) {
		printf("(%c, %d), ", s[i], shift_array[(int)s[i]]);
	}
	printf("(%c, %d)]\n", s[s_len-1], shift_array[(int)s[s_len-1]]);
	return shift_array;
}

// Suffix Match | Best: O(), Average: O(), Worst: O()
// Searching is O(mlogn), Creation is O(n^2) at worst (with ternary quicksort)

// Gave up on this one cause it was a bit more irritating
// might come back and finish it later
/*int suffix_match(char* haystack, char* needle) {
	int haystack_len = strlen(haystack), needle_len = strlen(needle);
	if (needle_len > haystack_len) return NOT_FOUND;
	char** suffix_array = make_suffix_array(haystack, haystack_len);

	free(suffix_array);
	haystack[haystack_len] = '\0';
	return NOT_FOUND;
}

char** make_suffix_array(char* s, int s_len) {
	char** suffix_array = malloc(s_len * sizeof(char*));
	char min_char = 1;  // used instead of '\0' for our purposes
	s[s_len] = min_char;

}*/

// --- HELPER FUNCTIONS ---

// Integer swapper helper function, O(1)
void int_swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

// Reads an integer array from stdin
int* read_int_array(int max_len, int* len_ptr) {
	int* array = malloc(max_len * sizeof(int));
	int upto = 0;
	while (scanf("%d", array+upto) == 1 && upto <= max_len) upto++;
	if (upto == 0 || upto > max_len) {
		printf("Error: invalid array input!\n");
		free(array);
		*len_ptr = 0;
		return NULL;
	}
	*len_ptr = upto;
	return array;
}

// Prints out an integer array to stdout
void print_int_array(int* array, int n) {
	putchar('[');
	if (array != NULL && n > 0) {
		for (int i = 0; i < n-1; i++) printf("%d, ", array[i]);
		printf("%d]\n", array[n-1]);
	}
	else printf("]\n");
}

// Asks the user to enter an array from stdin
int* ask_for_array(int maxlen, int* len_ptr) {
	printf("Enter a list of %d or less integers separated by spaces, ", maxlen);
	printf("ctrl-D to finish: ");
	fflush(stdout);
	int* array = read_int_array(maxlen, len_ptr);
	printf("Received array ");
	print_int_array(array, *len_ptr);
	return array;
}

// Asks the user to enter a string from stdin
char* ask_for_string(int maxlen, char* promptstr) {
	char* string = malloc((maxlen+1) * sizeof(char));
	printf("Enter a string for '%s': ", promptstr);
	fflush(stdout);
	fgets(string, maxlen, stdin);
	string[strlen(string)-1] = '\0';  // remove trailing '\n'
	return string;
}
