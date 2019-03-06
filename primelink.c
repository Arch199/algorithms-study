/* Find primes with generate and test: storing in a linked list.
 * Idea from Joel.
 */

#include <stdio.h>
#include <stdlib.h>

typedef int data_t;
#include "linkedlist.c"

void find_primes(int n);

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Invalid arguments!\n");
        exit(EXIT_FAILURE);
    }
    else {
        int n = atoi(argv[1]);
        find_primes(n);
    }

    return 0;
}

/* 
 * Finds primes (looks at integers up to and including n)
 * Complexity is hard to calculate but is definitely better than O(n^2 / log(n))
 * based on the distributions of primes, see https://en.wikipedia.org/wiki/Prime_number_theorem
 */
void find_primes(int n) {
    if (n < 2) return;

    linked_t* primes = new_linked();
    append_linked(primes, 2);
    printf("%d\n", primes->head->data);

    for (int i = 3; i <= n; i += 2) {
        int isprime = 1;
        for (node_t* ptr = primes->head; ptr->next != NULL; ptr = ptr->next) {
            if (ptr->data * ptr->data > i) break; // might be slightly better than sqrt once
            if (i % ptr->data == 0) {
                isprime = 0;
                break;
            }
        }
        if (isprime) {
            append_linked(primes, i);
            printf("%d\n", primes->foot->data);
        }
    }

    free_linked(primes);
}
