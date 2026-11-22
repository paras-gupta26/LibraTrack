
#include <iostream>
#include <string>
using namespace std;

// Date information
struct Date {
    int day;
    int month;
    int year;
};

// Student information
struct student {
    string name;
    string En_ID;
};

// Book information
struct Book {
    string title;
    int Id;
    Date borrowDate;
    int timePeriod;
};

// Check leap year
bool isLeapYear(int year) {
    return (year % 400 == 0) ||
           (year % 4 == 0 && year % 100 != 0);
}

// Find number of days in a month
int daysInMonth(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30,
                  31, 31, 30, 31, 30, 31};

    if (month < 1 || month > 12)
        return 0;

    if (month == 2 && isLeapYear(year))
        return 29;

    return days[month - 1];
}

// Validate date
bool validDate(Date date) {
    return date.year >= 1 &&
           date.month >= 1 && date.month <= 12 &&
           date.day >= 1 &&
           date.day <= daysInMonth(date.month, date.year);
}

// Calculate final return date
Date calculateReturnDate(Date date, int timePeriod) {
    for (int i = 0; i < timePeriod; i++) {
        date.day++;

        if (date.day > daysInMonth(date.month, date.year)) {
            date.day = 1;
            date.month++;

            if (date.month > 12) {
                date.month = 1;
                date.year++;
            }
        }
    }

    return date;
}

// Display date
void displayDate(Date date) {
    cout << date.day << "/" << date.month
         << "/" << date.year;
}

// Wait for Enter
void waitforEnter() {
    cout << "Press Enter to continue..." << endl;
    cin.get();
}

// Check whether a book is available
bool isBookAvailable(Book savedBooks[],
                     string savedStatus[],
                     int count,
                     int bookID,
                     int ignoreIndex = -1) {

    for (int i = 0; i < count; i++) {
        if (i != ignoreIndex &&
            savedBooks[i].Id == bookID &&
            savedStatus[i] == "Active") {
            return false;
        }
    }

    return true;
}

// Find active loan using Enrollment ID and Book ID
int findActiveLoan(student savedStudents[],
                   Book savedBooks[],
                   string savedStatus[],
                   int count,
                   string enrollmentID,
                   int bookID) {

    for (int i = 0; i < count; i++) {
        if (savedStudents[i].En_ID == enrollmentID &&
            savedBooks[i].Id == bookID &&
            savedStatus[i] == "Active") {
            return i;
        }
    }

    return -1;
}

// Display one entry
void displayEntry(student s, Book b,
                  string action, string status) {

    cout << "\nStudent name: " << s.name << endl;
    cout << "Enrollment ID: " << s.En_ID << endl;
    cout << "Action: " << action << endl;
    cout << "Book title: " << b.title << endl;
    cout << "Book ID: " << b.Id << endl;
    cout << "Loan status: " << status << endl;

    if (status == "Active") {
        cout << "Borrow date: ";
        displayDate(b.borrowDate);
        cout << endl;

        cout << "Time period: "
             << b.timePeriod << " days" << endl;

        cout << "Final return date: ";
        displayDate(
            calculateReturnDate(b.borrowDate, b.timePeriod)
        );
        cout << endl;
    }

    cout << "====================================" << endl;
}

int main() {

    int choice;
    int count = 0;
    bool running = true;

    string action;

    // Original arrays
    student savedStudents[100];
    Book savedBooks[100];
    Book savedNewBooks[100];
    string savedActions[100];

    // Additional array for loan status
    string savedStatus[100];

    cout << "\n             Welcome To LibraTrack" << endl;
    cout << "    Every Book. Every Borrower. All in One Place"
         << endl;

    waitforEnter();

    while (running) {

        cout << "\n========== MAIN MENU ==========" << endl;
        cout << "1. BORROW BOOK" << endl;
        cout << "2. EXCHANGE BOOK" << endl;
        cout << "3. RETURN BOOK" << endl;
        cout << "4. CHECK BOOK AVAILABILITY" << endl;
        cout << "5. SEARCH BY ENROLLMENT ID" << endl;
        cout << "6. VIEW ALL SAVED ENTRIES" << endl;
        cout << "7. EXIT" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        // =====================================
        // 1. BORROW BOOK
        // =====================================

        if (choice == 1) {

            if (count >= 100) {
                cout << "Storage full. Maximum 100 entries allowed."
                     << endl;
                continue;
            }

            student s1;
            Book B1 = {};

            cout << "\nEnter your name: ";
            getline(cin >> ws, s1.name);

            cout << "Enrollment ID: ";
            getline(cin, s1.En_ID);

            cout << "Enter book title: ";
            getline(cin, B1.title);

            cout << "Enter Book ID: ";

            if (!(cin >> B1.Id) || B1.Id <= 0) {
                cout << "Invalid Book ID." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            // Check availability
            if (!isBookAvailable(savedBooks, savedStatus,
                                 count, B1.Id)) {

                cout << "Sorry! This book is already borrowed."
                     << endl;
                continue;
            }

            cout << "Enter borrow day (DD): ";
            cin >> B1.borrowDate.day;

            cout << "Enter borrow month (MM): ";
            cin >> B1.borrowDate.month;

            cout << "Enter borrow year (YYYY): ";
            cin >> B1.borrowDate.year;

            cout << "Enter borrowing period in days: ";
            cin >> B1.timePeriod;

            if (!cin || !validDate(B1.borrowDate) ||
                B1.timePeriod <= 0) {

                cout << "Invalid date or borrowing period."
                     << endl;

                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            // Save transaction
            savedStudents[count] = s1;
            savedBooks[count] = B1;
            savedNewBooks[count] = {};
            savedActions[count] = "Borrow";
            savedStatus[count] = "Active";

            count++;

            cout << "\n========== BORROW SUMMARY =========="
                 << endl;

            displayEntry(s1, B1, "Borrow", "Active");

            cout << "Book successfully borrowed!" << endl;
        }

        // =====================================
        // 2. EXCHANGE BOOK
        // =====================================

        else if (choice == 2) {

            student s1;
            Book oldBook = {};
            Book newBook = {};

            cout << "\nEnter your name: ";
            getline(cin >> ws, s1.name);

            cout << "Enrollment ID: ";
            getline(cin, s1.En_ID);

            cout << "Enter old Book ID: ";

            if (!(cin >> oldBook.Id) || oldBook.Id <= 0) {
                cout << "Invalid Book ID." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            // Find the student's active loan
            int existingIndex = findActiveLoan(
                savedStudents, savedBooks, savedStatus,
                count, s1.En_ID, oldBook.Id
            );

            if (existingIndex == -1) {
                cout << "No active loan found for this student."
                     << endl;
                continue;
            }

            cout << "Enter new book title: ";
            getline(cin >> ws, newBook.title);

            cout << "Enter new Book ID: ";

            if (!(cin >> newBook.Id) || newBook.Id <= 0) {
                cout << "Invalid Book ID." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            // Check new book availability
            if (!isBookAvailable(savedBooks, savedStatus,
                                 count, newBook.Id,
                                 existingIndex)) {

                cout << "The new book is already borrowed."
                     << endl;
                continue;
            }

            cout << "Enter exchange day (DD): ";
            cin >> newBook.borrowDate.day;

            cout << "Enter exchange month (MM): ";
            cin >> newBook.borrowDate.month;

            cout << "Enter exchange year (YYYY): ";
            cin >> newBook.borrowDate.year;

            cout << "Enter borrowing period in days: ";
            cin >> newBook.timePeriod;

            if (!cin || !validDate(newBook.borrowDate) ||
                newBook.timePeriod <= 0) {

                cout << "Invalid date or borrowing period."
                     << endl;

                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            // Store old book and update existing entry
            savedNewBooks[existingIndex] =
                savedBooks[existingIndex];

            savedBooks[existingIndex] = newBook;
            savedActions[existingIndex] = "Exchange";
            savedStudents[existingIndex] = s1;

            cout << "\nBook exchanged successfully!" << endl;
            cout << "New book: " << newBook.title << endl;
            cout << "New Book ID: " << newBook.Id << endl;

            cout << "Final return date: ";
            displayDate(
                calculateReturnDate(
                    newBook.borrowDate,
                    newBook.timePeriod
                )
            );
            cout << endl;
        }

        // =====================================
        // 3. RETURN BOOK
        // =====================================

        else if (choice == 3) {

            string enrollmentID;
            int bookID;

            cout << "\nEnter Enrollment ID: ";
            getline(cin >> ws, enrollmentID);

            cout << "Enter Book ID to return: ";

            if (!(cin >> bookID) || bookID <= 0) {
                cout << "Invalid Book ID." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            int existingIndex = findActiveLoan(
                savedStudents, savedBooks, savedStatus,
                count, enrollmentID, bookID
            );

            if (existingIndex == -1) {
                cout << "No active borrowed book found."
                     << endl;
            }

            else {
                // Update loan status
                savedStatus[existingIndex] = "Returned";

                cout << "\nBook returned successfully!" << endl;
                cout << "Book title: "
                     << savedBooks[existingIndex].title << endl;
                cout << "Book ID: " << bookID << endl;
                cout << "Status updated to Returned." << endl;
            }
        }

        // =====================================
        // 4. CHECK BOOK AVAILABILITY
        // =====================================

        else if (choice == 4) {

            int bookID;

            cout << "\nEnter Book ID: ";

            if (!(cin >> bookID) || bookID <= 0) {
                cout << "Invalid Book ID." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            if (isBookAvailable(savedBooks, savedStatus,
                                count, bookID)) {

                cout << "Book is AVAILABLE." << endl;
            }

            else {
                cout << "Book is currently NOT AVAILABLE."
                     << endl;
            }
        }

        // =====================================
        // 5. SEARCH BY ENROLLMENT ID
        // =====================================

        else if (choice == 5) {

            string searchID;
            bool found = false;

            cout << "\nEnter Enrollment ID to search: ";
            getline(cin >> ws, searchID);

            for (int i = 0; i < count; i++) {

                if (savedStudents[i].En_ID == searchID) {

                    found = true;

                    cout << "\n========== MATCH FOUND =========="
                         << endl;

                    displayEntry(
                        savedStudents[i],
                        savedBooks[i],
                        savedActions[i],
                        savedStatus[i]
                    );
                }
            }

            if (!found) {
                cout << "No records found for this Enrollment ID."
                     << endl;
            }
        }

        // =====================================
        // 6. VIEW ALL SAVED ENTRIES
        // =====================================

        else if (choice == 6) {

            if (count == 0) {
                cout << "No saved entries found." << endl;
            }

            else {
                for (int i = 0; i < count; i++) {

                    cout << "\n========== ENTRY "
                         << i + 1 << " ==========" << endl;

                    displayEntry(
                        savedStudents[i],
                        savedBooks[i],
                        savedActions[i],
                        savedStatus[i]
                    );
                }
            }
        }

        // =====================================
        // 7. EXIT
        // =====================================

        else if (choice == 7) {
            running = false;
        }

        else {
            cout << "Invalid choice. Select from 1 to 7."
                 << endl;
        }
    }

    cout << "\nThank you for using LibraTrack!" << endl;
    return 0;
}

