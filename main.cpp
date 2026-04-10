#include <iostream>
#include <unordered_map>
#include <queue>
#include <stack>
#include <string>
#include <limits>

using namespace std;

// Book structure for Doubly Linked List
struct Book {
    int id;
    string title;
    string author;
    bool available;

    Book* next;
    Book* prev;

    queue<string> reservationQueue;

    Book(int i, string t, string a) {
        id = i;
        title = t;
        author = a;
        available = true;
        next = nullptr;
        prev = nullptr;
    }
};

struct Action {
    string type;
    int bookId;
};

// ========== Library Class ==========
class Library {
private:
    Book* head;
    Book* tail;
    unordered_map<int, Book*> bookMap;
    stack<Action> history;

public:
    Library() {
        head = nullptr;
        tail = nullptr;
    }


    void addBook(int id, string title, string author) {

        if (bookMap.find(id) != bookMap.end()) {
            cout << "Book ID already exists!" << endl;
            return;
        }

        Book* newBook = new Book(id, title, author);

        if (head == nullptr) {
            head = tail = newBook;
        } else {
            tail->next = newBook;
            newBook->prev = tail;
            tail = newBook;
        }

        bookMap[id] = newBook;
        cout << "Book added successfully!" << endl;
    }

    void borrowBook(int id, string user) {

        if (bookMap.find(id) == bookMap.end()) {
            cout << "Book not found." << endl;
            return;
        }

        Book* book = bookMap[id];

        if (book->available) {
            book->available = false;
            history.push({"borrow", id});
            cout << user << " borrowed the book." << endl;
        }
        else {
            book->reservationQueue.push(user);
            cout << "Book already borrowed. Added to reservation queue." << endl;
        }
    }


    void returnBook(int id) {

        if (bookMap.find(id) == bookMap.end()) {
            cout << "Book not found." << endl;
            return;
        }

        Book* book = bookMap[id];

        if (!book->reservationQueue.empty()) {
            string nextUser = book->reservationQueue.front();
            book->reservationQueue.pop();

            book->available = false;

            cout << "Book assigned to next user: " << nextUser << endl;
        }
        else {
            book->available = true;
            cout << "Book returned and now available." << endl;
        }

        history.push({"return", id});
    }


    void searchBook(int id) {

        if (bookMap.find(id) == bookMap.end()) {
            cout << "Book not found." << endl;
            return;
        }

        Book* book = bookMap[id];

        cout << "----- Book Details -----" << endl;
        cout << "ID: " << book->id << endl;
        cout << "Title: " << book->title << endl;
        cout << "Author: " << book->author << endl;
        cout << "Status: "
             << (book->available ? "Available" : "Borrowed")
             << endl;
    }


    void displayAllBooks() {

        Book* temp = head;

        if (!temp) {
            cout << "No books in library." << endl;
            return;
        }

        cout << "\n===== All Books =====" << endl;

        while (temp != nullptr) {
            cout << "ID: " << temp->id
                 << " | Title: " << temp->title
                 << " | Status: "
                 << (temp->available ? "Available" : "Borrowed")
                 << endl;

            temp = temp->next;
        }
    }


    void deleteBook(int id) {

        if (bookMap.find(id) == bookMap.end()) {
            cout << "Book not found." << endl;
            return;
        }

        Book* book = bookMap[id];

        if (book->prev)
            book->prev->next = book->next;
        else
            head = book->next;

        if (book->next)
            book->next->prev = book->prev;
        else
            tail = book->prev;

        bookMap.erase(id);
        delete book;

        cout << "Book deleted successfully!" << endl;
    }


    void undoLastAction() {

        if (history.empty()) {
            cout << "No actions to undo." << endl;
            return;
        }

        Action last = history.top();
        history.pop();

        if (bookMap.find(last.bookId) == bookMap.end()) {
            cout << "Book not found." << endl;
            return;
        }

        Book* book = bookMap[last.bookId];

        if (last.type == "borrow") {
            book->available = true;
            cout << "Undo: Borrow cancelled." << endl;
        }
        else if (last.type == "return") {
            book->available = false;
            cout << "Undo: Return cancelled." << endl;
        }
    }
};

// ========== MAIN ==========
int main() {

    Library lib;
    int choice;

    while (true) {

        cout << "\n===== Library Menu =====" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Borrow Book" << endl;
        cout << "3. Return Book" << endl;
        cout << "4. Search Book" << endl;
        cout << "5. Display All Books" << endl;
        cout << "6. Undo Last Action" << endl;
        cout << "7. Delete Book" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";

        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {

            int id;
            string title, author;

            cout << "Enter Book ID: ";
            cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter Book Title: ";
            getline(cin, title);

            cout << "Enter Author Name: ";
            getline(cin, author);

            lib.addBook(id, title, author);
        }

        else if (choice == 2) {

            int id;
            string user;

            cout << "Enter Book ID: ";
            cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter your name: ";
            getline(cin, user);

            lib.borrowBook(id, user);
        }

        else if (choice == 3) {

            int id;
            cout << "Enter Book ID: ";
            cin >> id;

            lib.returnBook(id);
        }

        else if (choice == 4) {

            int id;
            cout << "Enter Book ID to search: ";
            cin >> id;

            lib.searchBook(id);
        }

        else if (choice == 5) {
            lib.displayAllBooks();
        }

        else if (choice == 6) {
            lib.undoLastAction();
        }

        else if (choice == 7) {

            int id;
            cout << "Enter Book ID to delete: ";
            cin >> id;

            lib.deleteBook(id);
        }

        else if (choice == 8) {
            cout << "Exiting program..." << endl;
            break;
        }

        else {
            cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}
