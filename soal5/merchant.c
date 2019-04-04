#include<stdio.h>
#include <termios.h>
#include <assert.h>
#include <string.h>
#include <sys/shm.h>
#define B10 <SISOP>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int *foodStock;

void merchant_scene();
int getch(void);   

int main(int argc, char const *argv[])
{
    /* Shared Memory */
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    foodStock = shmat(shmid, NULL, 0);

    /* Initializing the value of foodStock */
    *foodStock = 35;

    merchant_scene();

    shmdt(foodStock);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

int getch(void) {
    int c=0;

    struct termios org_opts, new_opts;
    int res=0;
        //-----  store old settings -----------
    res=tcgetattr(STDIN_FILENO, &org_opts);
    assert(res==0);
        //---- set new terminal parms --------
    memcpy(&new_opts, &org_opts, sizeof(new_opts));
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
    c=getchar();
        //------  restore old settings ---------
    res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
    assert(res==0);
    return(c);
}

void merchant_scene() {
    /* Merchant Scene Initiated */
    char input;
    while(1) {
        system("clear");
        puts("Shop");
        printf("Food stock : %d\n",*foodStock);
        puts("Choices");
        printf("1. Restock\n2. Exit\n\n\n");

        /* Achieving Input from keyboard */
        input = getch();

        switch (input)
        {
            case '1':
                *foodStock = 35;
                break;
            case '2':
                return;
                break;
            default:
                break;
        }
    }
}