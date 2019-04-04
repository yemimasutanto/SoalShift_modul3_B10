#include<stdio.h>
#include <termios.h>
#include <assert.h>
#include<string.h>
#define B10 <SISOP>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define TRUE 1
#define FALSE 0

int getch(void);
typedef struct player {
    int health, hunger, hygiene;
    int foodStock, coolDown;
} Monster;

typedef struct npc {
    int health;
} AI;

Monster monster;
AI npc;

int onBattle;

void *monster_idle_onhunger(void *ptr) {
    /* */
    while(1) {
        sleep(10);
        if (onBattle == FALSE)
            monster.hunger -= 5;
    }
}

void *monster_idle_onhealth(void *ptr) {
    /* */
    while(1) {
        sleep(10);
        if (onBattle == FALSE) 
            monster.health += 5;
    }
}

void *monster_idle_onhygiene(void *ptr) {
    /* */
    while(1) {
        sleep(30);
        if (onBattle == FALSE)
            monster.hygiene -= 10;
    }
}

int main(int argc, char const *argv[])
{
    monster.hunger = 200;
    monster.health = 300;
    monster.hygiene = 100;
    
    char x = getch();
    

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