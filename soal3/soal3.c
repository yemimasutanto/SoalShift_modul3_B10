#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int WakeUp_Status=0;
int Spirit_Status=100;
int p_agmal=0, p_iraj=0;//nilai awal
int flag=0;
int count_WakeUp=0;
int count_Spirit=0;

void *kegiatan_agmal(void *argv){
	while(1){
		if(count_Spirit==3){
			p_agmal=1;
			sleep(10);
			p_agmal=0;
			count_Spirit=0;
		}
	}
}

void *kegiatan_iraj(void *argv){
	while(1){
		if(count_WakeUp==3){
			p_iraj=1;
			sleep(10);
			p_iraj=0;
			count_WakeUp=0;
		}
	}
}

void *WakeUp_Agmal(void *argv){
	WakeUp_Status+=15;
	count_WakeUp++;
	if(WakeUp_Status>=100){
		printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n\n");
		flag=1;
	}
}

void *Spirit_Iraj(void *argv){
	Spirit_Status-=20;
	count_Spirit++;
	if(Spirit_Status<=0){
		printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n\n");
		flag=1;
	}
}

int main(){
	pthread_t tid[4];
	pthread_create(&(tid[0]),NULL,kegiatan_agmal,NULL);
	pthread_create(&(tid[1]),NULL,kegiatan_iraj,NULL);

	while(flag==0){
		char perintah[50];
		gets(perintah);
		if(strcmp(perintah,"All Status")==0){
			printf("Agmal WakeUp_Status = %d\n", WakeUp_Status);
			printf("Iraj Spirit_Status = %d\n", Spirit_Status);
		}
		else if(strcmp(perintah, "Agmal Ayo Bangun")==0){
			if(p_agmal==1) printf("Fitur Agmal Ayo Bangun disabled 10s\n");
			else {
				pthread_create(&(tid[2]),NULL,WakeUp_Agmal,NULL);
				pthread_join(tid[2], NULL);// memastikan thread selesai mengubah flag, karena main dan thread jalan sendiri2
			}
		}
		else if(strcmp(perintah,"Iraj Ayo Tidur")==0){
			if(p_iraj==1) printf("Fitur Iraj Ayo Tidur disabled 10s\n");
			else {
				pthread_create(&(tid[3]),NULL,Spirit_Iraj,NULL);
				pthread_join(tid[3], NULL); // memastikan thread selesai mengubah flag, karena main dan thread jalan sendiri2
			}
		}
	}
	printf("Selesai!\n");
	return 0;
}
