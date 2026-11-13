
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <limits>
using namespace std;

// Structure to store item details
struct Item {
    int id;
    string studentName;
    string itemName;
    string location;
    string type;
    bool matched;
};

// Array to store records
Item items[200];
int totalItems = 0;
int nextId = 1;

// Convert text to lowercase for matching
string lowerCase(string text) {
    transform(text.begin(), text.end(), text.begin(),
              [](unsigned char c) { return tolower(c); });
    return text;
}

// Save records in a text file
void saveRecords() {
    ofstream file("records.txt");

    for (int i = 0; i < totalItems; i++) {
        file << items[i].id << "|"
             << items[i].studentName << "|"
             << items[i].itemName << "|"
             << items[i].location << "|"
             << items[i].type << "|"
             << items[i].matched << "\n";
    }

    file.close();
}

// Load records from file
void loadRecords() {
    ifstream file("records.txt");
    string line;

    totalItems = 0;
    nextId = 1;

    while (getline(file, line) && totalItems < 200) {
        stringstream ss(line);
        string id, student, name, location, type, matched;

        getline(ss, id, '|');
        getline(ss, student, '|');
        getline(ss, name, '|');
        getline(ss, location, '|');
        getline(ss, type, '|');
        getline(ss, matched, '|');

        try {
            Item item;
            item.id = stoi(id);
            item.studentName = student;
            item.itemName = name;
            item.location = location;
            item.type = type;
            item.matched = (matched == "1");

            items[totalItems++] = item;

            if (item.id >= nextId)
                nextId = item.id + 1;
        }
        catch (...) {
            // Skip invalid records
        }
    }

    file.close();
}

// Student login
void studentLogin() {
    string name;
    cout << "\nEnter Student Name: ";
    getline(cin, name);

    cout << "\nWelcome, " << name << "!\n";
}

// Report lost or found item
void reportItem(string type) {
    if (totalItems >= 200) {
        cout << "\nRecord limit reached!\n";
        return;
    }

    Item item;
    item.id = nextId++;
    item.type = type;
    item.matched = false;

    cout << "\nEnter Student Name: ";
    getline(cin, item.studentName);

    cout << "Enter Item Name: ";
    getline(cin, item.itemName);

    cout << "Enter Location: ";
    getline(cin, item.location);

    items[totalItems++] = item;

    saveRecords();

    cout << "\n" << type << " item reported successfully!";
    cout << "\nYour Record ID is: " << item.id << "\n";
}

// Search an item
void searchItem() {
    string name;
    bool found = false;

    cout << "\nEnter Item Name to Search: ";
    getline(cin, name);

    for (int i = 0; i < totalItems; i++) {
        if (lowerCase(items[i].itemName).find(lowerCase(name))
            != string::npos) {

            cout << "\nID: " << items[i].id;
            cout << "\nItem: " << items[i].itemName;
            cout << "\nLocation: " << items[i].location;
            cout << "\nType: " << items[i].type;
            cout << "\nMatch Status: "
                 << (items[i].matched ? "Matched" : "Not Matched");
            cout << "\n----------------------\n";

            found = true;
        }
    }

    if (!found)
        cout << "\nNo matching item found!\n";
}

// Automatically match lost and found items
void autoMatch() {
    bool found = false;

    for (int i = 0; i < totalItems; i++) {
        for (int j = i + 1; j < totalItems; j++) {

            if (items[i].type != items[j].type &&
                !items[i].matched && !items[j].matched &&
                lowerCase(items[i].itemName) ==
                lowerCase(items[j].itemName) &&
                lowerCase(items[i].location) ==
                lowerCase(items[j].location)) {

                items[i].matched = true;
                items[j].matched = true;

                cout << "\nMATCH FOUND!";
                cout << "\nItem: " << items[i].itemName;
                cout << "\nLocation: " << items[i].location;
                cout << "\nLost by: " << items[i].studentName;
                cout << "\nFound by: " << items[j].studentName;
                cout << "\n----------------------\n";

                found = true;
            }
        }
    }

    if (!found)
        cout << "\nNo new matches found!\n";

    saveRecords();
}

// Display all records in sorted order
void viewRecords() {
    if (totalItems == 0) {
        cout << "\nNo records available!\n";
        return;
    }

    Item sortedItems[200];

    for (int i = 0; i < totalItems; i++)
        sortedItems[i] = items[i];

    sort(sortedItems, sortedItems + totalItems,
         [](const Item &a, const Item &b) {
             return lowerCase(a.itemName) < lowerCase(b.itemName);
         });

    cout << "\n===== ALL RECORDS =====\n";

    for (int i = 0; i < totalItems; i++) {
        cout << "\nID: " << sortedItems[i].id;
        cout << "\nStudent: " << sortedItems[i].studentName;
        cout << "\nItem: " << sortedItems[i].itemName;
        cout << "\nLocation: " << sortedItems[i].location;
        cout << "\nType: " << sortedItems[i].type;
        cout << "\nMatched: "
             << (sortedItems[i].matched ? "Yes" : "No");
        cout << "\n----------------------\n";
    }
}

// Delete a record using ID
void deleteRecord() {
    int id;
    cout << "\nEnter Record ID to Delete: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < totalItems; i++) {
        if (items[i].id == id) {

            for (int j = i; j < totalItems - 1; j++)
                items[j] = items[j + 1];

            totalItems--;
            saveRecords();

            cout << "\nRecord deleted successfully!\n";
            return;
        }
    }

    cout << "\nRecord ID not found!\n";
}

// Display item statistics
void itemStatistics() {
    int lost = 0, found = 0, matched = 0;

    for (int i = 0; i < totalItems; i++) {
        if (items[i].type == "Lost")
            lost++;

        if (items[i].type == "Found")
            found++;

        if (items[i].matched)
            matched++;
    }

    cout << "\n===== ITEM STATISTICS =====\n";
    cout << "Total Records: " << totalItems;
    cout << "\nLost Items: " << lost;
    cout << "\nFound Items: " << found;
    cout << "\nMatched Records: " << matched << "\n";
}

// Export report to another file
void exportReport() {
    ofstream file("report.txt");

    if (!file) {
        cout << "\nUnable to create report!\n";
        return;
    }

    file << "SMART CAMPUS LOST & FOUND REPORT\n";
    file << "================================\n";

    for (int i = 0; i < totalItems; i++) {
        file << "\nID: " << items[i].id;
        file << "\nStudent: " << items[i].studentName;
        file << "\nItem: " << items[i].itemName;
        file << "\nLocation: " << items[i].location;
        file << "\nType: " << items[i].type;
        file << "\nMatched: "
             << (items[i].matched ? "Yes" : "No");
        file << "\n----------------------\n";
    }

    file.close();
    cout << "\nReport exported successfully to report.txt!\n";
}

// Main function
int main() {
    loadRecords();

    int choice;

    cout << "====================================\n";
    cout << " SMART CAMPUS LOST & FOUND SYSTEM\n";
    cout << "====================================\n";

    studentLogin();

    do {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Student Login\n";
        cout << "2. Report Lost Item\n";
        cout << "3. Report Found Item\n";
        cout << "4. Search Item\n";
        cout << "5. Auto Match Lost & Found\n";
        cout << "6. View All Records\n";
        cout << "7. Delete Record\n";
        cout << "8. Item Statistics\n";
        cout << "9. Export Report\n";
        cout << "10. Exit\n";

        cout << "\nEnter Your Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input! Enter a number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                studentLogin();
                break;

            case 2:
                reportItem("Lost");
                break;

            case 3:
                reportItem("Found");
                break;

            case 4:
                searchItem();
                break;

            case 5:
                autoMatch();
                break;

            case 6:
                viewRecords();
                break;

            case 7:
                deleteRecord();
                break;

            case 8:
                itemStatistics();
                break;

            case 9:
                exportReport();
                break;

            case 10:
                cout << "\nThank you for using the system!\n";
                break;

            default:
                cout << "\nInvalid choice! Try again.\n";
        }

    } while (choice != 10);

    return 0;
}
