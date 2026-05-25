#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <fstream>

using namespace std;

// ================= ROOM BASE CLASS =================

class Room {
protected:
    int roomNumber;
    double price;
    bool isAvailable;

public:
    Room(int num, double p, bool available = true) {
        roomNumber = num;
        price = p;
        isAvailable = available;
    }

    virtual void displayDetails() = 0;
    virtual string getRoomType() = 0;

    bool getAvailability() {
        return isAvailable;
    }

    void setAvailability(bool status) {
        isAvailable = status;
    }

    int getRoomNumber() {
        return roomNumber;
    }

    virtual ~Room() {}
};

// ================= DERIVED ROOM CLASSES =================

class VIPRoom : public Room {
public:
    VIPRoom(int num, double p) : Room(num, p) {}

    void displayDetails() override {

        cout << "VIP Room - Room No: "
             << roomNumber
             << " Price: $" << price
             << " Status: "
             << (isAvailable ? "Available" : "Booked")
             << endl;
    }

    string getRoomType() override {
        return "VIP";
    }
};

class MediumRoom : public Room {
public:
    MediumRoom(int num, double p) : Room(num, p) {}

    void displayDetails() override {

        cout << "Medium Room - Room No: "
             << roomNumber
             << " Price: $" << price
             << " Status: "
             << (isAvailable ? "Available" : "Booked")
             << endl;
    }

    string getRoomType() override {
        return "Medium";
    }
};

class GeneralRoom : public Room {
public:
    GeneralRoom(int num, double p) : Room(num, p) {}

    void displayDetails() override {

        cout << "General Room - Room No: "
             << roomNumber
             << " Price: $" << price
             << " Status: "
             << (isAvailable ? "Available" : "Booked")
             << endl;
    }

    string getRoomType() override {
        return "General";
    }
};

// ================= GUEST CLASS =================

class Guest {
private:
    string name;
    string email;

public:
    Guest() {}

    Guest(string n, string e) {
        name = n;
        email = e;
    }

    string getName() {
        return name;
    }

    string getEmail() {
        return email;
    }
};

// ================= RESERVATION CLASS =================

class Reservation {
private:
    int reservationID;
    Guest guest;
    int roomNumber;
    string checkInDate;
    string checkOutDate;

public:
    Reservation(int id,
                Guest g,
                int room,
                string checkIn,
                string checkOut)
    {
        reservationID = id;
        guest = g;
        roomNumber = room;
        checkInDate = checkIn;
        checkOutDate = checkOut;
    }

    int getReservationID() {
        return reservationID;
    }

    int getRoomNumber() {
        return roomNumber;
    }

    void displayReservation() {

        cout << "\nReservation ID: "
             << reservationID << endl;

        cout << "Guest Name: "
             << guest.getName() << endl;

        cout << "Guest Email: "
             << guest.getEmail() << endl;

        cout << "Room Number: "
             << roomNumber << endl;

        cout << "Check In Date: "
             << checkInDate << endl;

        cout << "Check Out Date: "
             << checkOutDate << endl;
    }

    string fileFormat() {

        return to_string(reservationID) + "," +
               guest.getName() + "," +
               guest.getEmail() + "," +
               to_string(roomNumber) + "," +
               checkInDate + "," +
               checkOutDate;
    }
};

// ================= CUSTOM EXCEPTIONS =================

class RoomNotAvailableException : public exception {
public:
    const char* what() const noexcept override {
        return "Room is not available!";
    }
};

class InvalidInputException : public exception {
public:
    const char* what() const noexcept override {
        return "Invalid input entered!";
    }
};

// ================= HOTEL MANAGER CLASS =================

class HotelManager {
private:
    vector<Room*> rooms;
    vector<Reservation> reservations;

public:

    // ================= LOAD ROOMS =================

    void loadRooms() {

        rooms.push_back(new GeneralRoom(101, 40));
        rooms.push_back(new GeneralRoom(102, 45));

        rooms.push_back(new MediumRoom(201, 70));
        rooms.push_back(new MediumRoom(202, 80));

        rooms.push_back(new VIPRoom(301, 150));
    }

    // ================= LOAD RESERVATIONS =================

    void loadReservations() {

        ifstream file("reservations.txt");

        if (!file) {
            return;
        }

        string line;

        while (getline(file, line)) {

            string data[6];
            string temp = "";
            int index = 0;

            for (char ch : line) {

                if (ch == ',') {
                    data[index++] = temp;
                    temp = "";
                }
                else {
                    temp += ch;
                }
            }

            data[index] = temp;

            int reservationID = stoi(data[0]);
            string name = data[1];
            string email = data[2];
            int roomNumber = stoi(data[3]);
            string checkIn = data[4];
            string checkOut = data[5];

            Guest guest(name, email);

            Reservation reservation(
                reservationID,
                guest,
                roomNumber,
                checkIn,
                checkOut
            );

            reservations.push_back(reservation);

            // Mark room unavailable
            for (Room* room : rooms) {

                if (room->getRoomNumber() == roomNumber) {
                    room->setAvailability(false);
                }
            }
        }

        file.close();
    }

    // ================= SHOW AVAILABLE ROOMS =================

    void showAvailableRooms() {

        cout << "\n===== AVAILABLE ROOMS =====\n";

        bool found = false;

        for (Room* room : rooms) {

            if (room->getAvailability()) {

                room->displayDetails();
                found = true;
            }
        }

        if (!found) {
            cout << "No rooms available.\n";
        }
    }

    // ================= BOOK ROOM =================

    void bookRoom() {

        int roomNumber;
        string name;
        string email;
        string checkIn;
        string checkOut;

        cout << "\nEnter Room Number: ";

        if (!(cin >> roomNumber)) {

            cin.clear();
            cin.ignore(1000, '\n');

            throw InvalidInputException();
        }

        cin.ignore();

        cout << "Enter Guest Name: ";
        getline(cin, name);

        cout << "Enter Guest Email: ";
        getline(cin, email);

        cout << "Enter Check In Date: ";
        getline(cin, checkIn);

        cout << "Enter Check Out Date: ";
        getline(cin, checkOut);

        try {

            for (Room* room : rooms) {

                if (room->getRoomNumber() == roomNumber) {

                    if (!room->getAvailability()) {
                        throw RoomNotAvailableException();
                    }

                    room->setAvailability(false);

                    Guest guest(name, email);

                    int reservationID = reservations.size() + 1001;

                    Reservation newReservation(
                        reservationID,
                        guest,
                        roomNumber,
                        checkIn,
                        checkOut
                    );

                    reservations.push_back(newReservation);

                    saveData();

                    cout << "\nRoom booked successfully!\n";

                    cout << "Reservation ID: "
                         << reservationID << endl;

                    return;
                }
            }

            cout << "\nRoom not found!\n";
        }

        catch (RoomNotAvailableException& e) {

            cout << "\n"
                 << e.what()
                 << endl;
        }
    }

    // ================= CANCEL RESERVATION =================

    void cancelReservation() {

        int reservationID;

        cout << "\nEnter Reservation ID to cancel: ";

        if (!(cin >> reservationID)) {

            cin.clear();
            cin.ignore(1000, '\n');

            throw InvalidInputException();
        }

        for (int i = 0; i < reservations.size(); i++) {

            if (reservations[i].getReservationID() == reservationID) {

                int roomNum = reservations[i].getRoomNumber();

                // Make room available again
                for (Room* room : rooms) {

                    if (room->getRoomNumber() == roomNum) {

                        room->setAvailability(true);
                        break;
                    }
                }

                reservations.erase(reservations.begin() + i);

                saveData();

                cout << "\nReservation cancelled successfully!\n";

                return;
            }
        }

        cout << "\nReservation not found!\n";
    }

    // ================= VIEW RESERVATIONS =================

    void viewReservations() {

        cout << "\n===== RESERVATIONS =====\n";

        if (reservations.empty()) {

            cout << "No reservations found!\n";
        }
        else {

            for (Reservation& r : reservations) {
                r.displayReservation();
            }
        }
    }

    // ================= SAVE DATA =================

    void saveData() {

        ofstream file("reservations.txt");

        for (Reservation& r : reservations) {

            file << r.fileFormat()
                 << endl;
        }

        file.close();
    }

    // ================= DESTRUCTOR =================

    ~HotelManager() {

        for (Room* room : rooms) {
            delete room;
        }
    }
};

// ================= MAIN FUNCTION =================

int main() {

    HotelManager hotel;

    hotel.loadRooms();

    hotel.loadReservations();

    int choice;

    do {

        cout << "\n===== HOTEL RESERVATION SYSTEM =====\n";

        cout << "1. View Available Rooms\n";
        cout << "2. Book Room\n";
        cout << "3. Cancel Reservation\n";
        cout << "4. View Reservations\n";
        cout << "5. Exit\n";

        cout << "Enter your choice: ";

        if (!(cin >> choice)) {

            cin.clear();
            cin.ignore(1000, '\n');

            cout << "\nInvalid input! Enter numbers only.\n";

            continue;
        }

        try {

            switch(choice) {

            case 1:
                hotel.showAvailableRooms();
                break;

            case 2:
                hotel.bookRoom();
                break;

            case 3:
                hotel.cancelReservation();
                break;

            case 4:
                hotel.viewReservations();
                break;

            case 5:
                cout << "\nExiting Program...\n";
                break;

            default:
                cout << "\nInvalid choice!\n";
            }
        }

        catch (InvalidInputException& e) {

            cout << "\n"
                 << e.what()
                 << endl;
        }

    } while(choice != 5);

    return 0;
}