#include<stdio.h>
#include<string.h>
#define B10 SISOP
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void *save_process_1(void *ptr) {
    system("ps -aux | head -n 10 > $HOME/Documents/FolderProses1/SimpanProses1.txt");
}

void *save_process_2(void *ptr) {
    system("ps -aux | head -n 10 > $HOME/Documents/FolderProses2/SimpanProses2.txt");
}

void *zip_1(void *ptr) {
    system("zip -rmj $HOME/Documents/FolderProses1/KompresProses1.zip $HOME/Documents/FolderProses1/SimpanProses1.txt");
}

void *zip_2(void *ptr) {
    system("zip -rmj $HOME/Documents/FolderProses2/KompresProses2.zip $HOME/Documents/FolderProses2/SimpanProses2.txt");
}

void *unzip_1(void *ptr) {
    system("unzip $HOME/Documents/FolderProses1/KompresProses1.zip -d $HOME/Documents/FolderProses1/");
}

void *unzip_2(void *ptr) {
    system("unzip $HOME/Documents/FolderProses2/KompresProses2.zip -d $HOME/Documents/FolderProses2/");
}

int main() {
    pthread_t tid1, tid2;
    // Simpan ps -aux ke file .txt
    pthread_create(&(tid1), NULL, save_process_1, NULL);
    pthread_create(&(tid2), NULL, save_process_2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Melakukan zip
    pthread_create(&(tid1), NULL, zip_1, NULL);
    pthread_create(&(tid2), NULL, zip_2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("Menunggu 15 detik untuk mengekstrak kembali.\n");
    sleep(15);

    // Melakukan unzip
    pthread_create(&(tid1), NULL, unzip_1, NULL);
    pthread_create(&(tid2), NULL, unzip_2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    return 0;
}