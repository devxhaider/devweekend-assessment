#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <climits>
#include <cctype>

using namespace std;

struct Bookmark {
    int id;
    string title;
    string url;
    string description;
};

vector<Bookmark> bookmarks;
int nextId = 1;

// -- Helpers ------------------------------------------------------------------

void clearInput() {
    cin.ignore(INT_MAX, '\n');
}

void printLine(char c, int len) {
    for (int i = 0; i < len; i++) cout << c;
    cout << "\n";
}

void printHeader(const string& title) {
    printLine('=', 60);
    cout << "  " << title << "\n";
    printLine('=', 60);
}

void pause() {
    cout << "\nPress Enter to continue...";
    clearInput();
    cin.get();

}

string toLower(const string& s) {
    string result = s;
    for (int i = 0; i < (int)result.size(); i++)
        result[i] = (char)tolower((unsigned char)result[i]);
    return result;
}

string truncate(const string& s, int maxLen) {
    if ((int)s.length() > maxLen)
        return s.substr(0, maxLen - 3) + "...";
    return s;
}

// -- Core Operations ----------------------------------------------------------

void addBookmark() {
    printHeader("ADD BOOKMARK");

    Bookmark b;
    b.id = nextId++;

    cout << "Title       : ";
    clearInput();
    getline(cin, b.title);
    if (b.title.empty()) {
        cout << "Title cannot be empty.\n";
        nextId--;
        pause();
        return;
    }

    cout << "URL         : ";
    getline(cin, b.url);
    if (b.url.empty()) {
        cout << "URL cannot be empty.\n";
        nextId--;
        pause();
        return;
    }

    cout << "Description : ";
    getline(cin, b.description);

    bookmarks.push_back(b);
    cout << "\n+ Bookmark #" << b.id << " added successfully.\n";
    pause();
}

void listBookmarks() {
    printHeader("ALL BOOKMARKS");

    if (bookmarks.empty()) {
        cout << "  No bookmarks yet. Add one!\n";
        pause();
        return;
    }

    cout << left
         << setw(5)  << "ID"
         << setw(25) << "Title"
         << setw(35) << "URL"
         << "Description" << "\n";
    printLine('-', 60);

    for (int i = 0; i < (int)bookmarks.size(); i++) {
        const Bookmark& b = bookmarks[i];
        cout << left
             << setw(5)  << b.id
             << setw(25) << truncate(b.title, 24)
             << setw(35) << truncate(b.url, 34)
             << truncate(b.description, 28) << "\n";
    }

    cout << "\nTotal: " << bookmarks.size() << " bookmark(s)\n";
    pause();
}

void viewBookmark() {
    printHeader("VIEW BOOKMARK");

    if (bookmarks.empty()) {
        cout << "No bookmarks to view.\n";
        pause();
        return;
    }

    int id;
    cout << "Enter bookmark ID: ";
    cin >> id;

    for (int i = 0; i < (int)bookmarks.size(); i++) {
        const Bookmark& b = bookmarks[i];
        if (b.id == id) {
            printLine('-', 60);
            cout << "  ID          : " << b.id << "\n";
            cout << "  Title       : " << b.title << "\n";
            cout << "  URL         : " << b.url << "\n";
            cout << "  Description : " << (b.description.empty() ? "(none)" : b.description) << "\n";
            printLine('-', 60);
            pause();
            return;
        }
    }

    cout << "Bookmark #" << id << " not found.\n";
    pause();
}

void updateBookmark() {
    printHeader("UPDATE BOOKMARK");

    if (bookmarks.empty()) {
        cout << "No bookmarks to update.\n";
        pause();
        return;
    }

    int id;
    cout << "Enter bookmark ID to update: ";
    cin >> id;

    for (int i = 0; i < (int)bookmarks.size(); i++) {
        Bookmark& b = bookmarks[i];
        if (b.id == id) {
            clearInput();
            cout << "\nLeave a field blank to keep the current value.\n\n";

            string input;

            cout << "Title [" << b.title << "]: ";
            getline(cin, input);
            if (!input.empty()) b.title = input;

            cout << "URL [" << b.url << "]: ";
            getline(cin, input);
            if (!input.empty()) b.url = input;

            cout << "Description [" << (b.description.empty() ? "(none)" : b.description) << "]: ";
            getline(cin, input);
            if (!input.empty()) b.description = input;

            cout << "\n+ Bookmark #" << id << " updated successfully.\n";
            pause();
            return;
        }
    }

    cout << "Bookmark #" << id << " not found.\n";
    pause();
}

void deleteBookmark() {
    printHeader("DELETE BOOKMARK");

    if (bookmarks.empty()) {
        cout << "No bookmarks to delete.\n";
        pause();
        return;
    }

    int id;
    cout << "Enter bookmark ID to delete: ";
    cin >> id;

    for (vector<Bookmark>::iterator it = bookmarks.begin(); it != bookmarks.end(); ++it) {
        if (it->id == id) {
            char confirm;
            cout << "Delete \"" << it->title << "\"? (y/n): ";
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                bookmarks.erase(it);
                cout << "\n+ Bookmark #" << id << " deleted.\n";
            } else {
                cout << "Cancelled.\n";
            }
            pause();
            return;
        }
    }

    cout << "Bookmark #" << id << " not found.\n";
    pause();
}

void searchBookmarks() {
    printHeader("SEARCH BOOKMARKS");

    if (bookmarks.empty()) {
        cout << "No bookmarks to search.\n";
        pause();
        return;
    }

    string query;
    cout << "Search (title/url/description): ";
    clearInput();
    getline(cin, query);

    string lq = toLower(query);
    int found = 0;
    cout << "\n";
    printLine('-', 60);

    for (int i = 0; i < (int)bookmarks.size(); i++) {
        const Bookmark& b = bookmarks[i];
        string lt = toLower(b.title);
        string lu = toLower(b.url);
        string ld = toLower(b.description);

        if (lt.find(lq) != string::npos ||
            lu.find(lq) != string::npos ||
            ld.find(lq) != string::npos) {
            cout << "  [#" << b.id << "] " << b.title << "\n";
            cout << "        " << b.url << "\n";
            if (!b.description.empty())
                cout << "        " << b.description << "\n";
            printLine('-', 60);
            found++;
        }
    }

    if (found == 0)
        cout << "No bookmarks matched \"" << query << "\".\n";
    else
        cout << found << " result(s) found.\n";

    pause();
}

// -- Menu ---------------------------------------------------------------------

void showMenu() {
    cout << "\n";
    printLine('=', 60);
    cout << "   BOOKMARK MANAGER\n";
    printLine('=', 60);
    cout << "  1. Add Bookmark\n";
    cout << "  2. List All Bookmarks\n";
    cout << "  3. View Bookmark\n";
    cout << "  4. Update Bookmark\n";
    cout << "  5. Delete Bookmark\n";
    cout << "  6. Search Bookmarks\n";
    printLine('-', 60);
    cout << "  0. Exit\n";
    printLine('=', 60);
    cout << "  Choice: ";
}

int main() {
    int choice;

    cout << "\nWelcome to Bookmark Manager!\n";

    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: addBookmark();     break;
            case 2: listBookmarks();   break;
            case 3: viewBookmark();    break;
            case 4: updateBookmark();  break;
            case 5: deleteBookmark();  break;
            case 6: searchBookmarks(); break;
            case 0:
                cout << "\nGoodbye!\n\n";
                return 0;
            default:
                cout << "Invalid option. Try again.\n";
        }
    }
}

