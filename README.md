# SoalShift_modul3_B10

## **Soal Nomor 1**

Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
    
Contoh:

./faktorial 5 3 4

3! = 6

4! = 24

5! = 120

### Penyelesaian

**Source Code**

```C
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
```

- Soal nomor 1 menggunakan argumen sebagai input dari program. Pada bahasa **C**, agar bisa menjalankan program dengan input berupa argumen, maka pada fungsi **main** menerima parameter:

   ```int main(int argc, char *argv[]) {}```

   Variabel **argc** digunakan untuk menyimpan jumlah argumen yang dimasukkan.

   Sedangkan variabel **argv** digunakan untuk menyimpan string argumen.

   Misalkan ```./faktorial 3 1 2```, maka **argc** nya adalah 4 dan **argv** nya berupa string:

   + ```argv[0] = "./faktorial"```
   + ```argv[1] = "3"```
   + dst.

- **Mengubah argumen menjadi integer**
   
   Pertama yang dilakukan adalah mengubah string **argv** menjadi integer. Dapat dilakukan dengan fungsi ```atoll``` (ll adalah long long).

   ```C
   LL num[argc-1];        // Jumlah array untuk menyimpan angka
   pthread_t tid[argc-1];
   
   for(int i=1; i<argc; i++) {
      num[i-1] = atoll(argv[i]); // Mengubah argv menjadi integer
   }
   ```
- **Mengurutkan array hasil konversi**

   Setelah itu, mengurutkan integer-integer tadi. Disini menggunakan fungsi Sort yang dibuat sendiri. Teknik *sorting*-nya adalah **Insertion Sort**.

   ```C
   Sort(num, argc-1);
   ```

   Fungsi ```Sort``` :

   ```C
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
   ```

- **Menggunakan thread untuk mencari faktorial integer-integer**

   Thread disini digunakan untuk mencari faktorial dari integer-integer hasil pengurutan. 

   ```C
   for(int i=0; i<argc-1; i++) {
      LL *x = malloc(sizeof(*x));
      *x = num[i];
      pthread_create(&(tid[i]), NULL, find_fact, x);
   }
   for(int i=0; i<argc-1; i++) pthread_join(tid[i], NULL);
   ```

   + variabel ```LL *x``` digunakan sebagai temporary untuk kemudian dipassing ke parameter saat membuat thread. Fungsi yang digunakan untuk mencari faktorial adalah **```find_fact()```**.

      ```C
      void *find_fact(void* argv) {
         LL num = *((long long*)argv);
         fact[0] = fact[1] = 1;
         for(int i=2; i<=num; i++) fact[i] = i * fact[i-1];
      }
      ```
   
   + Kemudian thread-thread dijoin.

- **Hasil-hasil faktorial kemudian dicetak**

   Untuk mencetak hasil faktorial, disini digunakan fungsi **```printResult()```**

   ```C
   void printResult(LL *arr, int n) {
      for(int i=0; i<n; i++)
         printf("%lld! = %lld\n",arr[i], fact[arr[i]]);
   }
   ```
----

## **Soal Nomor 2**

Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:
  1. Terdapat 2 server: server penjual dan server pembeli.
  2. 1 server hanya bisa terkoneksi dengan 1 client
  3. Server penjual dan server pembeli memiliki stok barang yang selalu sama
  4. Client yang terkoneksi ke server penjual hanya bisa menambah stok  
     - Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1
  5. Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok
     -  Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1  
  6. Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok
     -  Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”
     -  Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”
  7. Server penjual akan mencetak stok saat ini setiap 5 detik sekali
  8. **Menggunakan thread, socket, shared memory**

### Penyelesaian

- **Membuat program untuk 2 server (server pembeli dan server penjual)**

   Program server dibuat dengan menggunakan socket. Pada dasarnya, pembuatan program untuk server disini sama dengan di modul. Hanya saja, hal yang membedakan adalah alur jalannya program. Dan karena menggunakan dua server, maka terdapat perbedaan **PORT** yang digunakan oleh masing-masing server.

   + Server Penjual menggunakan PORT 9000
   + Server Pembeli menggunakan PORT 8080

   **Shared Memory**

   Server Penjual dan Pembeli selalu memiliki stok barang yang sama. Maka diperlukan mekanisme shared memory agar 2 server bisa saling berbagi jumlah stok yang tersisa.

   Hal ini dapat dilakukan dengan cara menggunakan variabel yang diassign ke memori yang sama pada kedua program.

   ```C
   void *build_shared_memory(void *argv) {
      key_t key = 1234;
      int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
      stock = shmat(shmid, NULL, 0);
      *stock = 10;      // Inisialisasi jumlah stock awal
   }
   ```

   Sehingga pada kedua program dapat mengakses memori yang sama secara bersamaan.

   **Pembuatan Socket Pada Server**

   Agar client dapat terhubung dengan server, maka pada server perlu dibuatkan socket. Proses pembuatan socket mirip dengan di modul.

   ```C
   struct sockaddr_in address;
   int opt = 1, addrlen =  sizeof(address);
   char *success = "server pembeli mengirim: transaksi berhasil";
   char *failed = "server pembeli mengirim: transaksi gagal";
   
   // Creating socket file descriptor
   if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      perror("Socket Failed");
      exit(EXIT_FAILURE);
   }

   // Memaksa socket agar terpasang pada port 8080
   if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
      perror("SetSockOpt Error");
      exit(EXIT_FAILURE);
   }
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(PORT);

   // Memaksa socket agar terpasang pada port 8080
   if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
      perror("Binding Failed");
      exit(EXIT_FAILURE);
   }

   // Listen to the client
   if (listen(server_fd, 3) < 0) {
      perror("Listen Error");
      exit(EXIT_FAILURE);
   }

   // Accept dari client
   if ((socket_new = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
      perror("Accept Error");
      exit(EXIT_FAILURE);
   }
   ```

   Perintah-perintah di atas diletakkan pada masing-masing server (server penjual dan pembeli), hanya saja dibedakan berdasarkan PORT yang digunakan.

   **Alur masing-masing server**

   + Pada server penjual, client yang terhubung dengannya hanya bisa melakukan "tambah" stok. Maka server penjual menerima pesan dari client berupa pesan **"tambah"** dan stok di server bertambah 1.

      ```c
      while(1){        
         char buffer[1024] = {0};
         valread = read(socket_new, buffer, 1024);
         if (!valread) break;
         
         if (strcmp(buffer, "tambah") == 0) {
            *stock += 1;
            send(socket_new, success, strlen(success), 0);
         }
      }
      ```

      > Setelah client mengirim pesan "tambah", server akan membalas dengan mengirim pesan sukses.

   + Server penjual akan menampilkan stok saat ini setiap 5 detik. Ini dapat dilakukan menggunakan thread.

      ```c
      pthread_create(&(tid1), NULL, print_stock, NULL);
      ```

      dengan fungsi **```print_stock()```** adalah :

      ```c
      void *print_stock(void* argv) {
         while(1) {
            printf("stok saat ini sebanyak : %d\n", *stock);
            sleep(5);
         }
      }
      ```

   + Sedangkan pada server pembeli, cient yang terhubung dengannya hanya bisa melakukan "beli". Maka server pembeli menerima pesan dari client berupa pesan **"beli"**.

      ```c
      while(1) {        
         char buffer[1024] = {0};
         valread = read(socket_new, buffer, 1024);
         if (!valread) break;
         if (strcmp(buffer, "beli") == 0) {
               if (*stock > 0) {
                  *stock -= 1;
                  send(socket_new, success, strlen(success), 0);
               } else send(socket_new, failed, strlen(failed), 0);
         }
      }
      ```
      > Setelah client mengirim pesan "beli", server akan membalas dengan mengirim pesan sukses jika stok masih tersedia (stok berkurang 1) dan pesan gagal jika stok sudah habis.

- **Membuat program 2 client (client yang terhubung ke server penjual dan client yang terhubung ke server pembeli)**

   Masing-masing client dibuat menggunakan socket. Sama seperti server, pembuatan socket disini hampir sama dengan socket yang ada pada modul. Namun dibedakan berdasarkan PORT nya saja.

   + PORT 9000 merupakan client yang terhubung ke server penjual
   + PORT 8080 merupakan client yang terhubung ke server pembeli

   ```c
   struct sockaddr_in address, serv_addr;
   int sock=0, valread;
   
   char *input = (char*)malloc(sizeof(char)*1024);
   
   // Creating Socket (var sock is socket descriptor)
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("Socket Creation Error\n");
      return -1;
   }
   memset(&serv_addr, '0', sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(PORT);

   // Convert IPv6/IPv4 to binary
   if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
      printf("Invalid Address\n");
      return -1;
   }

   if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      printf("can't connect to the server\n");
      return -1;
   }
   ```

   Potongan kode diatas diletakka pada masing-masing client, dengan PORT yang berbeda.

   **Menerima input dan mengirim pesan ke server**

   Client dapat menerima input pesan yang nantinya akan dikirim ke server masing-masing. Perintahnya adalah sebagai berikut.

   ```c
   while(1) {
      char buffer[1024] = {0};
      int error = 0;
      socklen_t len = sizeof(error);
      int retVal = getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len);

      if (error != 0) {
         printf("server disconnected\n");
         break;
      }
      scanf("%s",input); // menerima input
      send(sock, input, strlen(input), 0); // mengirim ke server
      valread = read(sock, buffer, 1024);
      printf("%s\n",buffer);
   }
   ```

- **[Source code server pembeli disini](https://github.com/yemimasutanto/SoalShift_modul3_B10/blob/master/soal2/server_pembeli.c)**
- **[Source code server penjual disini](https://github.com/yemimasutanto/SoalShift_modul3_B10/blob/master/soal2/server_penjual.c)**
- **[Source code client ke server penjual disini](https://github.com/yemimasutanto/SoalShift_modul3_B10/blob/master/soal2/client_2.c)**
- **[Source code client ke server pembeli disini](https://github.com/yemimasutanto/SoalShift_modul3_B10/blob/master/soal2/client_1.c)**

------

## **Soal Nomor 3**

Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:
  
   1.  Terdapat 2 karakter Agmal dan Iraj
   2.  Kedua karakter memiliki status yang unik
       -  Agmal mempunyai WakeUp_Status, di awal program memiliki status 0
       -  Iraj memiliki Spirit_Status, di awal program memiliki status 100
       -  Terdapat 3 fitur utama
          * All Status, yaitu menampilkan status kedua sahabat
            Ex: Agmal WakeUp_Status = 75 
                Iraj Spirit_Status = 30
          * “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
          * “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point
       -   Terdapat kasus yang unik dimana:
           *   Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)
           *   Jika Fitur  “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)
       -   Program akan berhenti jika salah satu :
           *   WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)
           *   Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”)
        -   **Syarat Menggunakan Lebih dari 1 Thread**

### Penyelesaian

Untuk menyelesaikan soal nomer 3, diperlukan 4 thread, karena terdapat 4 fungsi, yaitu:

1. void* kegiatan_agmal

2. void* kegiatan_iraj

3. void* WakeUp_Agmal

4. void* Spirit_Iraj

No 1 dan 2 fungsinya untuk menampung berapa kali perintah "Agmal Ayo Bangun" dan "Iraj Ayo Tidur" dijalankan, serta melakukan sleep selama 10s jika perintah diatas telah dijalankan 3x berturut-turut (jika perintah "Agmal Ayo Tidur" maka fungsi **menidurkan Iraj** tidak bisa dijalankan selama 10s, jika perintah "Iraj Ayo Bangun" maka fungsi **membangunkan Agmal** tidak bisa dijalankan selama 10s).

No 3 dan 4 fungsinya untuk menghitung dan menampung penambahan dan pengurangan hitpoint Agmal dan Iraj (Agmal bertambah, Iraj berkurang). Fungsi ini juga akan menampilkan statement jika hitpoint dari salah satu anak telah "habis" (WakeUp_Status Agmal menjadi 100, statement yang akan ditampilkan:"Agmal Terbangun, mereka bangun pagi dan berolahraga" atau Spirit_Status Iraj menjadi 0, statement yang akan ditampilkan: "Iraj ikut tidur, dan bangun kesiangan bersama Agmal").

Untuk bagian main, fitur-fitur program dibuat, yaitu :

1. Statement "All status" : Menampilkan nilai dari WakeUp_Status dan Spirit Status

2. Statement "Agmal Ayo Bangun" 

3. Statement "Iraj Ayo Tidur"

No 2 dan 3 tidak mengeluarkan output apa-apa, namun jika salah satu diketikkan secara berturut-turut hingga 4x atau lebih, maka perintah lainnya akan disable selama 10s.

contoh:

Agmal Ayo Bangun

Agmal Ayo Bangun

Agmal Ayo Bangun

Iraj Ayo Tidur -> (Selanjutnya perintah "Iraj Ayo Tidur" akan disable sementara selama 10s, karena Agmal Ayo Bangun telah dijalankan 3x berturut-turut, begitu juga sebaliknya akan terjadi pada perintah "Agmal Ayo Bangun" jika diberi perlakuan yang sama).

![alt text](https://github.com/yemimasutanto/SoalShift_modul3_B10/blob/master/soal3/soal3.PNG)

- **[Source code disini](https://github.com/yemimasutanto/SoalShift_modul3_B10/blob/master/soal3/soal3.c)**

-------

## **Soal Nomor 4**

Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 

**Dengan Syarat :** 
  - Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
  - Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
  - Ketika Mengekstrak file .zip juga harus secara bersama-sama
  - Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”
  - Wajib Menggunakan Multithreading
  - Boleh menggunakan system

### Penyelesaian

**Menggunakan multi threading untuk menyelesaikan soal nomor 4**

Source Code

```c
#include<stdio.h>
#include<string.h>
#define B10 SISOP
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void *save_process_1(void *ptr) {
    system("ps -aux | head -n 11 > $HOME/Documents/FolderProses1/SimpanProses1.txt");
}

void *save_process_2(void *ptr) {
    system("ps -aux | head -n 11 > $HOME/Documents/FolderProses2/SimpanProses2.txt");
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
    pthread_t tid[6];
    // Simpan ps -aux ke file .txt
    pthread_create(&(tid[0]), NULL, save_process_1, NULL);
    pthread_create(&(tid[1]), NULL, save_process_2, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    // Melakukan zip
    pthread_create(&(tid[2]), NULL, zip_1, NULL);
    pthread_create(&(tid[3]), NULL, zip_2, NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);

    printf("Menunggu 15 detik untuk mengekstrak kembali.\n");
    sleep(15);

    // Melakukan unzip
    pthread_create(&(tid[4]), NULL, unzip_1, NULL);
    pthread_create(&(tid[5]), NULL, unzip_2, NULL);
    pthread_join(tid[4], NULL);
    pthread_join(tid[5], NULL);
    
    return 0;
}
```

- **Menyimpan list proses (ps -aux) maksimal 10 proses ke dalam file SimpanProses[i].txt**

   Thread yang digunakan adalah sebagai berikut.

   ```c
   void *save_process_1(void *ptr) {
      system("ps -aux | head -n 11 > $HOME/Documents/FolderProses1/SimpanProses1.txt");
   }

   void *save_process_2(void *ptr) {
      system("ps -aux | head -n 11 > $HOME/Documents/FolderProses2/SimpanProses2.txt");
   }
   ```

   + Untuk menyimpan 10 proses, command yang digunakan adalah **head -n 11**, karena baris pertama adalah header dan 10 baris berikutnya adalah list proses.

   Agar kedua thread di atas berjalan bersama-sama, maka dilakukan join pada kedua thread.

   ```c
   // Simpan ps -aux ke file .txt
    pthread_create(&(tid[0]), NULL, save_process_1, NULL);
    pthread_create(&(tid[1]), NULL, save_process_2, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
   ```

- **Mengkompress file hasil .txt  menjadi file zip dengan sekaligus menghapus file .txt**

   Thread yang digunakan adalah sebagai berikut.

   ```c
   void *zip_1(void *ptr) {
      system("zip -rmj $HOME/Documents/FolderProses1/KompresProses1.zip $HOME/Documents/FolderProses1/SimpanProses1.txt");
   }

   void *zip_2(void *ptr) {
      system("zip -rmj $HOME/Documents/FolderProses2/KompresProses2.zip $HOME/Documents/FolderProses2/SimpanProses2.txt");
   }
   ```

   + Secara default command **zip** akan melakukan kompresi dengan mengikutsertakan struktur direktorinya juga. Agar hal itu tidak terjadi, maka ditambahkan argumen **-j**. Sedangkan, agar file yang dikompress otomatis terhapus, ditambahkan argumen **-rm**.

   Agar kedua thread di atas berjalan bersama-sama, maka dilakukan join pada kedua thread.

   ```c
   // Melakukan zip
   pthread_create(&(tid[2]), NULL, zip_1, NULL);
   pthread_create(&(tid[3]), NULL, zip_2, NULL);
   pthread_join(tid[2], NULL);
   pthread_join(tid[3], NULL);
   ```

- **Menunggu 15 detik sebelum mengekstrak**

   Menggunakan fungsi sleep() selama 15 detik.

   ```c
   printf("Menunggu 15 detik untuk mengekstrak kembali.\n");
   sleep(15);
   ```

- **Melakukan ekstrak file .zip**

   Thread yang digunakan adalah sebagai berikut.

   ```c
   void *unzip_1(void *ptr) {
      system("unzip $HOME/Documents/FolderProses1/KompresProses1.zip -d $HOME/Documents/FolderProses1/");
   }

   void *unzip_2(void *ptr) {
      system("unzip $HOME/Documents/FolderProses2/KompresProses2.zip -d $HOME/Documents/FolderProses2/");
   }
   ```

   + Ekstraksi dilakukan menggunakan command unzip kemudian ditentukan destinasi hasil ekstrak menggunakan **-d**.

   Agar kedua thread di atas berjalan bersama-sama, maka dilakukan join pada kedua thread.

   ```c
   // Melakukan unzip
   pthread_create(&(tid[4]), NULL, unzip_1, NULL);
   pthread_create(&(tid[5]), NULL, unzip_2, NULL);
   pthread_join(tid[4], NULL);
   pthread_join(tid[5], NULL);
   ```

------------

## **Soal Nomor 5**

