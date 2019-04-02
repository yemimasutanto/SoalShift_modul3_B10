# SoalShift_modul3_B10

## **Soal Nomor 1**

Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
    Contoh:
    ./faktorial 5 3 4
    3! = 6
    4! = 24
    5! = 120

## Penyelesaian


## **Soal Nomor 2**

Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:
  
  1. Terdapat 2 server: server penjual dan server pembeli
  
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

## Penyelesaian

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

## Penyelesaian

## **Soal Nomor 4**

Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 

**Dengan Syarat :** 
  - Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
  - Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
  - Ketika Mengekstrak file .zip juga harus secara bersama-sama
  - Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”
  - Wajib Menggunakan Multithreading
  - Boleh menggunakan system

## Penyelesaian

## **Soal Nomor 5**

