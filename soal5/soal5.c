#include<stdio.h>
#include <termios.h>
#include <assert.h>
#include <sys/shm.h>
#include<string.h>
#define B10 <SISOP>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define TRUE 1
#define FALSE 0
#define STANDBY 1
#define BATTLE 2
#define SHOP 3

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
int *shopStock;                      // Stores the stock of food in the stores
pthread_t tid[5];
int scene;
/* STORES ALL THE GLOBAL VARIABLES */

/* Initializing the initial value for monster's attributes */
void initialize();

/* Function to increment/decrement of monsters's attr */
void *monster_idle_onhunger(void *ptr);
void *monster_idle_onhealth(void *ptr);
void *monster_idle_onhygiene(void *ptr);

/* This is the scenes function */
void standby_scene();
void battle_scene();
void shop_scene();

/* Function to take input */
void *takeInput(void *ptr);

/* Monster function */
void eat();                             // Function to eat
void take_bath();                       // Function to bath
void *bath_cooling_down(void *ptr);

int main(int argc, char const *argv[])
{    
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    shopStock = shmat(shmid, NULL, 0);
    
    char name[100];
    printf("Enter the name of your monster :\n>> ");    // prompt input for name's input
    scanf("%s",name);
    monster.name = name;
    
    /* Creating the thread */
    pthread_create(&(tid[0]), NULL, takeInput, NULL);
    pthread_create(&(tid[2]), NULL, monster_idle_onhealth, NULL);
    pthread_create(&(tid[3]), NULL, monster_idle_onhunger, NULL);
    pthread_create(&(tid[4]), NULL, monster_idle_onhygiene, NULL);

    scene = STANDBY;
    initialize();
    while(1)
    {
        standby_scene();
    }

    shmdt(shopStock);
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

void *takeInput(void *ptr) {
    char input;
    while(1) {
        input = getch();

        if (scene == STANDBY) 
        {
            switch (input) {
                case '1':
                    eat();
                    break;
                case '2':
                    take_bath();
                    break;
                case '3':
                    scene = BATTLE;
                    break;
                case '4':
                    scene = SHOP;
                    break;
                case '5':
                    exit(EXIT_SUCCESS);
            }
        } else if (scene == BATTLE) 
        {
            switch (input) {
                case '1':
                    npc.health -= 20;
                    monster.health -= 20;
                    break;
                case '2':
                    scene = STANDBY;
                    break;
            }
        } else if (scene == SHOP) 
        {
            switch (input) {
                case '1':
                /* Check whether there is food stock available */
                    if (*shopStock > 0) {
                        monster.foodStock += 1;
                        *shopStock -= 1;            // Add the monster's food stock
                    }
                break;
                case '2':
                    scene = STANDBY;
                    break;
            }
        }
    }
}

void eat() {
    if (monster.foodStock > 0) {
        monster.foodStock--;
        monster.hunger += 15;
        if (monster.hunger > 200) monster.hunger = 200;
    }
}

void take_bath() {
    if (monster.bathCoolDown > 0) return;
    monster.hygiene += 30;
    pthread_create(&(tid[1]), NULL, bath_cooling_down, NULL);
}

void *bath_cooling_down(void *ptr) {
    monster.bathCoolDown = 20;
    while(monster.bathCoolDown > 0) {
        sleep(1);
        monster.bathCoolDown--;
    }
}

void standby_scene() {
    /* The standby scene function starts */
    
    while(1) 
    {
        /* check current scene */
        if (scene == BATTLE) battle_scene();
        if (scene == SHOP) shop_scene();
        
        system("clear");
        puts("Standby Mode");
        // printf("-----------------------------\n");

        /* Showing all status of the monsters */
        printf("Health : %d\n",monster.health);
        printf("Hunger : %d\n",monster.hunger);
        printf("Hygiene : %d\n",monster.hygiene);
        printf("Food left : %d\n",monster.foodStock);
        
        /* Check whether bathroom is ready */
        if (monster.bathCoolDown > 0) {
            printf("Bath will be ready in %ds\n",monster.bathCoolDown);
        } else {
            printf("Bath is ready\n");
        }

        puts("Choices");
        printf("1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n\n\n");

        sleep(1);
    }
}

void shop_scene() {
    /* The shop_scene function starts */
    char input;
    while(1) {
        /* Check current scene */
        if (scene == STANDBY) {scene = STANDBY; return;}
        
        system("clear");
        /* Showing food stock status */
        puts("Shop Mode");
        printf("Shop food stock: %d\n",*shopStock);
        printf("Your food stock: %d\n",monster.foodStock);
        puts("Choices");
        printf("1. Buy\n2. Back\n\n\n");

        sleep(1);
    }
}

void battle_scene() {
    /* The function battle_scene starts here */
    /* Setting status onBattle becomes TRUE */
    onBattle = TRUE;
    npc.health = 100;
    char input;
    while(1) {
        /* Check the current scene */
        if (scene == STANDBY) {
            scene = STANDBY; 
            onBattle = FALSE; 
            return;
        }

        if (monster.health <= 0) {
            scene = STANDBY;
            monster.health = 0;
        }
        if (npc.health <= 0) {
            scene = STANDBY;
            npc.health = 0;
        }
        
        system("clear");
        /* Showing Monster's Health and Enemy's Health */
        puts("Battle Mode");
        printf("Monster's Health: %d\n",monster.health);
        printf("Enemy's Health: %d\n",npc.health);
        puts("Choices");
        printf("1. Attack\n2. Run\n\n\n");

        sleep(1);
    }
}

void initialize() {
    monster.hunger = 200;
    monster.health = 300;
    monster.hygiene = 100;
    monster.attackRate = 20;
    monster.foodStock = 0;
    monster.bathCoolDown = 0;
}

void *monster_idle_onhunger(void *ptr) {
    /* */
    while(1) {
        sleep(10);
        if (onBattle == FALSE) {
            monster.hunger -= 5;
            if (monster.hunger < 0) monster.hunger = 0;
        }
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
        if (onBattle == FALSE) {
            monster.hygiene -= 10;
            if (monster.hygiene < 0) monster.hygiene = 0;
        }
    }
}