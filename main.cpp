#include <iostream>
using namespace std;

struct Transaksi {
    int id;
    string namaPengeluaran;
    double nominal;
    string pembayar;
    string peserta[10];
    int jmlPeserta;
    bool isLunas;
    Transaksi* next;
    Transaksi* prev;
};

Transaksi* head = nullptr;
Transaksi* tail = nullptr;
int currentId = 1; 

void tambahTransaksi() {
    Transaksi* nodeBaru = new Transaksi();

    nodeBaru->id = currentId++;

    cout << "\n=== INPUT TRANSAKSI BARU ===\n";
    
    // input data transaksi
    cout << "Nama Pengeluaran : ";
    getline(cin, nodeBaru->namaPengeluaran);

    cout << "Nominal (Rp)     : ";
    cin >> nodeBaru->nominal;

    cout << "Nama Pembayar    : ";
    cin.ignore();
    getline(cin, nodeBaru->pembayar);

    cout << "Jumlah Peserta (Maks 10): ";
    cin >> nodeBaru->jmlPeserta;

    // inputan data peserta
    cin.ignore();
    for (int i = 0; i < nodeBaru->jmlPeserta; i++) {
        cout << "  - Nama Peserta " << (i + 1) << " : ";
        getline(cin, nodeBaru->peserta[i]);
    }

    // set nilai default untuk status pembayaran
    nodeBaru->isLunas = false;
    nodeBaru->next = nullptr;
    nodeBaru->prev = nullptr;

    // simpan ke double linked list
    if (head == nullptr) {
        // kondisi jika data masih kosong
        head = nodeBaru;
        tail = nodeBaru;
    } else {
        // kondisi jika sudah ada data, node baru disambung di bagian ekor (tail)
        tail->next = nodeBaru;
        nodeBaru->prev = tail;
        tail = nodeBaru; // Update pointer tail ke node terujung
    }

    cout << "\n[SUCCESS] Data '" << nodeBaru->namaPengeluaran << "' berhasil disimpan ke dalam list!\n";
}

int main() {
    tambahTransaksi();
    
    return 0;
}
