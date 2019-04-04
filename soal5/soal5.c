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

int getch(void);                    // Key detection function
typedef struct player {
    int health, hunger, hygiene;
    int foodStock, bathCoolDown, attackRate;
    char *name;
} Monster;

typedef struct npc {
    int health, attackRate;
} AI;

/* STORES ALL THE GLOBAL VARIABLES */
Monster monster;                    // Object of monster
AI npc;                             // Object of NPC
int onBattle;                       // Status whether BattleScene() is running
int shopStock;                      // Stores the stock of food in the stores
/* STORES ALL THE GLOBAL VARIABLES */

/* Initializing the initial value for monster's attribute */
void initialize();

/* Function to increment/decrement of monsters's attr */
void *monster_idle_onhunger(void *ptr);
void *monster_idle_onhealth(void *ptr);
void *monster_idle_onhygiene(void *ptr);

/* This is the scenes function */
void standby_scene(int *isExit);
void battle_scene();
void shop_scene();

int main(int argc, char const *argv[])
{    
    // char name[100];
    // printf("Enter the name of your monster :\n>> ");    // prompt input for name's input
    // scanf("%s",name);
    // monster.name = name;
    shopStock = 15;
    int isExit = FALSE;                         // whether if exit is pressed
    initialize();
    while(!isExit)
    {
        /* Go To StandBy Mode */
        standby_scene(&isExit);
    }

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

void standby_scene(int *isExit) {
    /* The standby scene function starts */
    char input;                          // Used to store the input from keyboard
    while(1) 
    {
        system("clear");
        puts("STANDBY MODE");
        printf("-----------------------------\n");

        /* Showing all status of the monsters */
        printf("Health : %d\n",monster.health);
        printf("Hunger : %d\n",monster.hunger);
        printf("Hygiene : %d\n",monster.hygiene);
        printf("Food left : %d\n",monster.foodStock);
        printf("Bath will be ready in %ds\n",monster.bathCoolDown);

        puts("Choices");
        printf("1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n\n\n");

        /* Achieve input from the keyboard */
        input = getch();

        /* Handling the user's input */
        switch (input)
        {
            case '1':
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                shop_scene();
                break;
            case '5':
                *isExit = TRUE;
                return;
                break;
            default :
                printf("\nWrong Input\n");
                sleep(2);
        }

    }
}

void shop_scene() {
    /* The shop_scene function starts */
    char input;
    while(1) {
        system("clear");
        /* Showing food stock status */
        puts("Shop Mode");
        printf("Shop food stock: %d\n",shopStock);
        printf("Your food stock: %d\n",monster.foodStock);
        puts("Choices");
        printf("1. Buy\n2. Back\n\n\n");

        /* Achieve input from keyboard */
        input = getch();

        /* Handling input */
        switch (input)
        {
            case '1':
                /* Check whether there is food stock available */
                if (shopStock > 0) {
                    monster.foodStock += 1;
                    shopStock--;            // Add the monster's food stock
                }
                break;
            case '2':
                return;
                break;
            default:
                printf("\nWrong Input\n");
                break;
        }

    }
}

void battle_scene() {
    /* The function battle_scene starts here */
    char input;
    while(1) {
        system("clear");
    }
}

void initialize() {
    monster.hunger = 200;
    monster.health = 300;
    monster.hygiene = 100;
    monster.attackRate = 20;
    monster.foodStock = 0;
}

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