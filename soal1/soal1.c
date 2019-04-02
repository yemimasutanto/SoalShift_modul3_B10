#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
typedef long long int ll;

void *factorial(void *argv){
    ll a=strtol((char *)argv,NULL,10);//convert string to long long int
    ll num=1;
    for(ll i=1;i<=a;i++) num*=i;
    printf("Hasil %lld! = %lld\n", a, num);
    return NULL;
}

int main(int argc,char *argv[]){
    ll idx,j,i,temp;
    pthread_t tid[argc-1];
    for(i=0; i<(argc-1); i++){
        idx=i;
        for(j=i+1; j<argc; j++){
            if(*argv[j]<*argv[idx]){
                idx=j;
            }
        }
        temp=*argv[i];
        *argv[i]=*argv[idx];
        *argv[idx]=temp;
    }
    for(int i=0;i<argc-1;i++){
        pthread_create(&(tid[i]),NULL,&factorial,(void *)argv[i+1]);
    }
    for(int i=0;i<argc-1;i++){
        pthread_join(tid[i],NULL);
    }
}
