/*
.------..------..------.
|T.--. ||U.--. ||J.--. |
| :/\: || (\/) || :(): |
| (__) || :\/: || ()() |
| '--'T|| '--'U|| '--'J|
`------'`------'`------'
                                                                       
*/

#include <iostream>
#include <vector>
#include <string>
#include <map> 
#include <algorithm> 
#include <iomanip> 
#include <limits> 

using namespace std;

// Function prototype
string toUpperCase(const string& str);
void convertTime(int totalMinutes, int* hour, int* minutes);


map<int, string> hari;

void initializeHari() {
    hari.insert(make_pair(0, "Senin"));
    hari.insert(make_pair(1, "Selasa"));
    hari.insert(make_pair(2, "Rabu"));
    hari.insert(make_pair(3, "Kamis"));
    hari.insert(make_pair(4, "Jumat"));
    hari.insert(make_pair(5, "Sabtu"));
    hari.insert(make_pair(6, "Minggu"));
}

// Matkul class to store subject details
class Matkul {
public:
    string nama;
    int startTime;
    int endTime;
    int day;

    Matkul(string nama, int mulai, int akhir, int day) {
        this->nama = nama;
        startTime = mulai;
        endTime = akhir;
        this->day = day;
    }
};

// Node class for the linked list
struct Node {
    Matkul data;
    Node* next;
    Node(Matkul m) : data(m), next(NULL) {}  
};

// Class to manage available subjects
class MatkulTersedia {
private:
    vector<Matkul> availableMatkul; 
public:
    void addAvailableMatkul(string nama, int start, int end, int day) {
        availableMatkul.push_back(Matkul(nama, start, end, day)); 
    }

    bool findMatkul(const string& nama, Matkul& foundMatkul) {
        for (size_t i = 0; i < availableMatkul.size(); i++) {  
            Matkul& m = availableMatkul[i];
            if (toUpperCase(m.nama) == toUpperCase(nama)) {
                foundMatkul = m;
                return true;
            }
        }
        return false;
    }

    void printAvailableMatkul() {
        cout << "Daftar matkul yang tersedia:\n";
        for (size_t i = 0; i < availableMatkul.size(); i++) { 
            Matkul& m = availableMatkul[i];
            int startHour, startMinute, endHour, endMinute;
            convertTime(m.startTime, &startHour, &startMinute);
            convertTime(m.endTime, &endHour, &endMinute);

            cout << "\t" << toUpperCase(m.nama) << " " << hari[m.day] << ",\t"
                 << setw(2) << setfill('0') << startHour << "."
                 << setw(2) << setfill('0') << startMinute
                 << " s/d " << setw(2) << setfill('0') << endHour << "."
                 << setw(2) << setfill('0') << endMinute << endl;
        }
    }
};

// Function to add a subject to the linked list
void addMatkul(Node*& head, MatkulTersedia& jadwal, string nama) {
    Matkul newMatkul("", 0, 0, 0);

    if (!jadwal.findMatkul(nama, newMatkul)) {
        cout << "Matkul tidak ditemukan\n";
        return;
    }

    Node* temp = head;
    while (temp) {
        if (toUpperCase(temp->data.nama) == toUpperCase(nama)) {
            cout << "Matkul sudah diambil\n";
            return;
        }
        temp = temp->next;
    }

    Node* newNode = new Node(newMatkul);

    if (!head) {
        head = newNode;
    } else {
        temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Function to drop a subject from the linked list
void dropMatkul(Node*& head, string nama) {
    if (!head) {
        cout << "Tidak ada mata kuliah yang diambil\n";
        return;
    }

    Node* temp = head;
    Node* prev = NULL; 

    while (temp) {
        if (toUpperCase(temp->data.nama) == toUpperCase(nama)) {
            if (!prev) {
                head = temp->next;
            } else {
                prev->next = temp->next;  
            }
            delete temp;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    cout << "Matkul tidak ditemukan\n";
}

// Function to print taken subjects
void printTakenMatkul(Node* head) {
    if (!head) {
        cout << "Tidak ada mata kuliah yang diambil.\n";
        return;
    }

    Node* temp = head;
    while (temp) {
        int startHour, startMinute, endHour, endMinute;
        convertTime(temp->data.startTime, &startHour, &startMinute);
        convertTime(temp->data.endTime, &endHour, &endMinute);

        cout << toUpperCase(temp->data.nama) << " " << toUpperCase(hari[temp->data.day]) << ", "
             << setw(2) << setfill('0') << startHour << "."
             << setw(2) << setfill('0') << startMinute
             << " s/d " << setw(2) << setfill('0') << endHour << "."
             << setw(2) << setfill('0') << endMinute << "\n";
        temp = temp->next;
    }
}

// Function to check for schedule conflicts
void cekRingkasan(Node* head) {
    if (!head) {
        cout << "Tidak ada mata kuliah yang diambil.\n";
        return;
    }

    Node* temp1 = head;
    bool bentrok = false;

    while (temp1) {
        Node* temp2 = temp1->next;
        while (temp2) {
            if (temp1->data.day == temp2->data.day &&
                (temp1->data.startTime < temp2->data.endTime && temp1->data.endTime > temp2->data.startTime)) {
                cout << "Bentrokan antara " << temp1->data.nama << " dan " << temp2->data.nama << "\n";
                bentrok = true;
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }

    if (!bentrok) {
        cout << "Tidak ada mata kuliah yang bermasalah\n";
    }
}

// Convert a string to uppercase
string toUpperCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// Convert total minutes to hours and minutes
void convertTime(int totalMinutes, int* hour, int* minutes) {
    int minutesInDay = 24 * 60;
    int remainingMinutes = totalMinutes % minutesInDay;
    *hour = remainingMinutes / 60;
    *minutes = remainingMinutes % 60;
}

// Print the main menu
void printMenu() {
    cout << "=========== SUSUN JADWAL ===========\n";
    cout << "1 Add matkul\n";
    cout << "2 Drop matkul\n";
    cout << "3 Cek ringkasan\n";
    cout << "4 Lihat mata kuliah yang diambil\n";
    cout << "5 Lihat mata kuliah yang tersedia\n";
    cout << "6 Keluar\n";
}

int main() {
	
	initializeHari();

	
    MatkulTersedia jadwal;

    jadwal.addAvailableMatkul("Struktur Data", 480, 580, 0);   
    jadwal.addAvailableMatkul("Linear Matriks", 480, 580, 2);  
    jadwal.addAvailableMatkul("Alpro", 480, 580, 1);           
    jadwal.addAvailableMatkul("Manbis", 540, 640, 0);          
    jadwal.addAvailableMatkul("Matdis 1 A", 540, 640, 2);      
    jadwal.addAvailableMatkul("Matdis 1 B", 540, 640, 0);      
    jadwal.addAvailableMatkul("Kalkulus 1 C", 600, 720, 2); 

    Node* takenMatkul = NULL;
    int choice;
    string inputMatkul;

    while (true) {
        printMenu();
        cout << "Pilihan: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                jadwal.printAvailableMatkul();
                cout << "Masukkan nama matkul yang ingin diambil: ";
                getline(cin, inputMatkul);
                addMatkul(takenMatkul, jadwal, inputMatkul);
                break;

            case 2:
                cout << "Masukkan nama matkul yang ingin di-drop: ";
                getline(cin, inputMatkul);
                dropMatkul(takenMatkul, inputMatkul);
                break;

            case 3:
                cekRingkasan(takenMatkul);
                break;

            case 4:
                printTakenMatkul(takenMatkul);
                break;

            case 5:
                jadwal.printAvailableMatkul();
                break;

            case 6:
                cout << "Terima kasih!\n";
                return 0;

            default:
                cout << "Input tidak valid.\n";
                break;
        }
        cout << endl;
    }

    return 0;
}

