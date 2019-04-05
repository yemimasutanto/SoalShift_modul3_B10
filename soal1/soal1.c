/* Menampilkan faktorial secara berurutan */
#define SISOP B10
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
typedef long long int LL;

LL fact[100];

void *find_fact(void* argv);
void Sort(LL *arr, int n);
void printResult(LL *arr, int n);

int main(int argc, char *argv[]) {
    LL num[argc-1];
    pthread_t tid[argc-1];
    
    for(int i=1; i<argc; i++) {
        num[i-1] = atoll(argv[i]);
    }
    Sort(num, argc-1);
    for(int i=0; i<argc-1; i++) {
        LL *x = malloc(sizeof(*x));
        *x = num[i];
        pthread_create(&(tid[i]), NULL, find_fact, x);
    }
    for(int i=0; i<argc-1; i++) pthread_join(tid[i], NULL);
    printResult(num, argc-1);
    return 0;
}

void *find_fact(void* argv) {
    LL num = *((long long*)argv);
    fact[0] = fact[1] = 1;
    for(int i=2; i<=num; i++) fact[i] = i * fact[i-1];
}

void printResult(LL *arr, int n) {
    for(int i=0; i<n; i++)
        printf("%lld! = %lld\n",arr[i], fact[arr[i]]);
}

void Sort(LL *arr, int n) {
    LL key; int i, j;
    for(i=1; i<n; i++) {
        key = arr[i];
        j = i-1;

        while(j>=0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}
/* SISOP B10 */