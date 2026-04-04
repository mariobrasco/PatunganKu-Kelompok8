#include <iostream>
using namespace std;

// ================== STRUCT ==================
struct Peserta {
    string nama;
    int totalBayar;
    int sudahBayar;
};

struct Transaksi {
    int id;
    string namaPengeluaran;
    int nominal;
    string pembayar;

    Peserta peserta[25];
    int jumlahPeserta;

    Transaksi* prev;
    Transaksi* next;
};

// ================== GLOBAL ==================
Transaksi* head = NULL;
Transaksi* tail = NULL;
int currentId = 1;

// ================== INPUT ==================
int inputInt(string msg) {
    int x;
    while(true) {
        cout << msg;
        cin >> x;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "❌ Harus angka!\n";
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
}

// ================== SEARCH ==================
Transaksi* cariByID(int id) {
    Transaksi* cur = head;
    while(cur) {
        if(cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

// ================== CEK SELESAI ==================
bool isSelesai(Transaksi* t) {
    for(int i=0;i<t->jumlahPeserta;i++) {
        if(t->peserta[i].sudahBayar < t->peserta[i].totalBayar)
            return false;
    }
    return true;
}

// ================== TAMBAH ==================
void tambahTransaksi() {
    Transaksi* baru = new Transaksi;

    cout << "\n=== Tambah Transaksi ===\n";

    cout << "Nama Pengeluaran: ";
    getline(cin, baru->namaPengeluaran);

    cout << "Nama Pembayar: ";
    getline(cin, baru->pembayar);

    baru->jumlahPeserta = inputInt("Jumlah Peserta (max 25 / 0 batal): ");
    if(baru->jumlahPeserta == 0 || baru->jumlahPeserta > 25) { delete baru; return; }

    int metode = inputInt("Cara Bayar\n1. Bagi rata\n2. Manual (Split Bill)\nPilih: ");

    int total = 0;

    if(metode == 1) {
        baru->nominal = inputInt("Nominal total: ");
        int per = baru->nominal / baru->jumlahPeserta;

        for(int i=0;i<baru->jumlahPeserta;i++) {
            cout << "   Nama " << i+1 << ": ";
            getline(cin, baru->peserta[i].nama);

            baru->peserta[i].totalBayar = per;

            if(baru->peserta[i].nama == baru->pembayar)
                baru->peserta[i].sudahBayar = per;
            else
                baru->peserta[i].sudahBayar = 0;
        }
    } 
    else {
        for(int i=0;i<baru->jumlahPeserta;i++) {
            cout << "\nPeserta " << i+1 << endl;

            cout << "Nama: ";
            getline(cin, baru->peserta[i].nama);

            int jml = inputInt("Jumlah item: ");
            int t = 0;

            for(int j=0;j<jml;j++) {
                string namaItem;
                cout << "   Nama Item " << j+1 << ": ";
                getline(cin, namaItem);

                int harga = inputInt("   Harga Item " + to_string(j+1) + ": ");
                t += harga;
            }

            baru->peserta[i].totalBayar = t;

            if(baru->peserta[i].nama == baru->pembayar)
                baru->peserta[i].sudahBayar = t;
            else
                baru->peserta[i].sudahBayar = 0;

            cout << "Total: " << t << endl;
            total += t;
        }

        baru->nominal = total;
    }

    // FIX ID
    baru->id = currentId++;

    baru->next = NULL;
    baru->prev = NULL;

    if(head == NULL) head = tail = baru;
    else {
        tail->next = baru;
        baru->prev = tail;
        tail = baru;
    }

    cout << "[OK] Berhasil!\n";
}

// ================== TAMPIL ==================
void tampilkanSemua() {
    cout << "\n=== SEMUA TRANSAKSI ===\n";

    if(head == NULL) {
        cout << "Belum ada data transaksi.\n";
        return;
    }

    Transaksi* cur = head;

    while(cur) {
        cout << "\nID: " << cur->id << " | " << cur->namaPengeluaran << endl;

        for(int i=0;i<cur->jumlahPeserta;i++) {
            int sisa = cur->peserta[i].totalBayar - cur->peserta[i].sudahBayar;

            cout << cur->peserta[i].nama
                 << " | Total: " << cur->peserta[i].totalBayar
                 << " | Bayar: " << cur->peserta[i].sudahBayar
                 << " | Sisa: " << sisa << endl;
        }

        cur = cur->next;
    }
}

// ================== BELUM LUNAS ==================
void tampilkanBelumLunas() {
    cout << "\n=== BELUM LUNAS ===\n";

    if(head == NULL) {
        cout << "Belum ada data transaksi.\n";
        return;
    }

    Transaksi* cur = head;

    while(cur) {
        for(int i=0;i<cur->jumlahPeserta;i++) {
            if(cur->peserta[i].sudahBayar < cur->peserta[i].totalBayar) {
                cout << "ID: " << cur->id
                     << " | " << cur->namaPengeluaran
                     << " | " << cur->peserta[i].nama
                     << " | Sisa: "
                     << cur->peserta[i].totalBayar - cur->peserta[i].sudahBayar
                     << endl;
            }
        }
        cur = cur->next;
    }
}

// ================== BAYAR ==================
void bayarTransaksi() {
    if(head == NULL) {
        cout << "Belum ada data transaksi.\n";
        return;
    }

    int id = inputInt("ID (0 batal): ");
    if(id == 0) return;

    Transaksi* t = cariByID(id);
    if(!t) {
        cout << "Tidak ditemukan!\n";
        return;
    }

    cout << "\n=== Peserta Belum Lunas ===\n";

    int index[25], cnt=0;

    for(int i=0;i<t->jumlahPeserta;i++) {
        if(t->peserta[i].sudahBayar < t->peserta[i].totalBayar) {
            int sisa = t->peserta[i].totalBayar - t->peserta[i].sudahBayar;

            cout << cnt+1 << ". " << t->peserta[i].nama
                 << " | Sisa: " << sisa << endl;

            index[cnt++] = i;
        }
    }

    if(cnt == 0) {
        cout << "Semua sudah lunas!\n";
        return;
    }

    int pilih;

    while(true) {
        pilih = inputInt("Pilih (0 batal): ");

        if(pilih == 0) return;

        if(pilih < 1 || pilih > cnt) {
            cout << "❌ Pilihan tidak valid!\n";
        } else break;
    }

    int i = index[pilih-1];
    Peserta &p = t->peserta[i];

    int sisa = p.totalBayar - p.sudahBayar;

    while(true) {
        int bayar = inputInt("Bayar (max " + to_string(sisa) + "): ");

        if(bayar > sisa) {
            cout << "❌ Tidak boleh lebih!\n";
        } else {
            p.sudahBayar += bayar;
            break;
        }
    }

    cout << "✅ Sukses!\n";
}

// ================== HAPUS ==================
void hapusTransaksiSelesai() {
    if(head == NULL) {
        cout << "Belum ada data transaksi.\n";
        return;
    }

    Transaksi* cur = head;
    bool ada = false;

    cout << "\n=== TRANSAKSI SELESAI ===\n";

    while(cur) {
        if(isSelesai(cur)) {
            cout << "ID: " << cur->id
                 << " | " << cur->namaPengeluaran << endl;
            ada = true;
        }
        cur = cur->next;
    }

    if(!ada) {
        cout << "Belum ada transaksi yang selesai.\n";
        return;
    }

    int id = inputInt("ID (0 batal): ");
    if(id == 0) return;

    Transaksi* t = cariByID(id);
    if(!t || !isSelesai(t)) {
        cout << "Tidak valid!\n";
        return;
    }

    if(t == head) {
        head = head->next;
        if(head) head->prev = NULL;
    } 
    else if(t == tail) {
        tail = tail->prev;
        tail->next = NULL;
    } 
    else {
        t->prev->next = t->next;
        t->next->prev = t->prev;
    }

    delete t;
    cout << "Berhasil dihapus!\n";
}

// ================== MAIN ==================
int main() {
    int p;

    do {
        cout << "\n=== MENU ===\n";
        cout << "1. Tambah Transaksi\n";
        cout << "2. Lihat Semua Transaksi\n";
        cout << "3. Belum Lunas\n";
        cout << "4. Bayar\n";
        cout << "5. Hapus Selesai\n";
        cout << "6. Keluar\n";

        p = inputInt("Pilih: ");

        switch(p) {
            case 1: tambahTransaksi(); break;
            case 2: tampilkanSemua(); break;
            case 3: tampilkanBelumLunas(); break;
            case 4: bayarTransaksi(); break;
            case 5: hapusTransaksiSelesai(); break;
        }

    } while(p != 6);

    return 0;
}
