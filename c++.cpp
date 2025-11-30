
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <map>
#include <sstream>
#include <limits>
#include <ctime>
#include <cctype>
#include <cstdlib> // ���� system ����֧��

using namespace std;

// ��������
const string BOOK_FILE = "books.dat";
const string CUSTOMER_FILE = "customers.dat";
const string TRANSACTION_FILE = "transactions.dat";
const string INVENTORY_FILE = "inventory.dat";
const string ADMIN_PASSWORD_FILE = "admin.dat";
const string ADMIN_PASSWORD = "admin123"; // Ĭ�Ϲ���Ա����

// �鼮��
class Book {
private:
    string bookID;
    string title;
    string author;
    string publisher;
    double price;
    int stock;

public:
    Book() : price(0), stock(0) {}
    Book(string id, string t, string a, string p, double pr, int s) 
        : bookID(id), title(t), author(a), publisher(p), price(pr), stock(s) {}

    // Getter����
    string getBookID() const { return bookID; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getPublisher() const { return publisher; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    // Setter����
    void setStock(int s) { stock = s; }
    void setPrice(double p) { price = p; }
    void setTitle(string t) { title = t; }
    void setAuthor(string a) { author = a; }
    void setPublisher(string p) { publisher = p; }

    // ��ʾ�鼮��Ϣ
    void display() const {
        cout << left << setw(15) << bookID 
             << setw(30) << title 
             << setw(20) << author 
             << setw(20) << publisher 
             << setw(10) << fixed << setprecision(2) << price 
             << setw(10) << stock << endl;
    }

    // �ļ�����
    void saveToFile(ofstream& out) const {
        out << bookID << "|" << title << "|" << author << "|" 
            << publisher << "|" << price << "|" << stock << endl;
    }

    void loadFromFile(ifstream& in) {
        string line;
        getline(in, line);
        stringstream ss(line);
        getline(ss, bookID, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, publisher, '|');
        ss >> price; ss.ignore();
        ss >> stock; ss.ignore();
    }
};

// �ͻ�����
class Customer {
protected:
    string customerID;
    string name;
    string address;
    double totalSpent;
    int points;

public:
    Customer() : totalSpent(0), points(0) {}
    Customer(string id, string n, string a) 
        : customerID(id), name(n), address(a), totalSpent(0), points(0) {}
    
    // ���������������������
    virtual ~Customer() {}

    // Getter����
    string getCustomerID() const { return customerID; }
    string getName() const { return name; }
    string getAddress() const { return address; }
    double getTotalSpent() const { return totalSpent; }
    int getPoints() const { return points; }

    // Setter����
    void addPurchase(double amount) { 
        totalSpent += amount; 
        points += static_cast<int>(amount);
    }
    
    void setName(string n) { name = n; }
    void setAddress(string a) { address = a; }

    // �麯��
    virtual double calculateDiscount(double amount) const = 0;
    virtual void display() const = 0;
    virtual string getType() const = 0;

    // �ļ�����
    virtual void saveToFile(ofstream& out) const {
        out << customerID << "|" << name << "|" << address << "|" 
            << totalSpent << "|" << points << "|";
    }

    virtual void loadFromFile(ifstream& in) {
        string line;
        getline(in, line);
        stringstream ss(line);
        getline(ss, customerID, '|');
        getline(ss, name, '|');
        getline(ss, address, '|');
        ss >> totalSpent; ss.ignore();
        ss >> points; ss.ignore();
    }
};

// ��ͨ�ͻ���
class RegularCustomer : public Customer {
public:
    RegularCustomer() : Customer() {}
    RegularCustomer(string id, string n, string a) : Customer(id, n, a) {}

    double calculateDiscount(double amount) const override {
        // ��ͨ�ͻ����ۿ�
        return amount;
    }

    void display() const override {
        cout << "ID: " << customerID << "\tName: " << name 
             << "\tType: Regular\tTotal Spent: $" << fixed << setprecision(2) << totalSpent 
             << "\tPoints: " << points << endl;
    }

    string getType() const override { return "Regular"; }

    void saveToFile(ofstream& out) const override {
        Customer::saveToFile(out);
        out << "R" << endl;
    }
};

// ��Ա�ͻ���
class MemberCustomer : public Customer {
private:
    int level;  // 1-5����Ա

public:
    MemberCustomer() : Customer(), level(1) {}
    MemberCustomer(string id, string n, string a, int l) : Customer(id, n, a), level(l) {}

    int getLevel() const { return level; }
    void setLevel(int l) { level = l; }

    double calculateDiscount(double amount) const override {
        // ���ݻ�Ա��������ۿ�
        switch(level) {
            case 1: return amount * 0.95;
            case 2: return amount * 0.90;
            case 3: return amount * 0.85;
            case 4: return amount * 0.80;
            case 5: return amount * 0.70;
            default: return amount;
        }
    }

    void display() const override {
        cout << "ID: " << customerID << "\tName: " << name 
             << "\tType: Member (Level " << level << ")\tTotal Spent: $" 
             << fixed << setprecision(2) << totalSpent << "\tPoints: " << points << endl;
    }

    string getType() const override { return "Member"; }

    void saveToFile(ofstream& out) const override {
        Customer::saveToFile(out);
        out << "M|" << level << endl;
    }

    void loadFromFile(ifstream& in) override {
        Customer::loadFromFile(in);
        char type;
        in >> type; in.ignore();
        in >> level; in.ignore();
    }
};

// VIP�ͻ���
class VIPCustomer : public Customer {
private:
    double discountRate;  // �Զ����ۿ���

public:
    VIPCustomer() : Customer(), discountRate(0.2) {}  // Ĭ��8��
    VIPCustomer(string id, string n, string a, double dr) : Customer(id, n, a), discountRate(dr) {}

    double getDiscountRate() const { return discountRate; }
    void setDiscountRate(double dr) { discountRate = dr; }

    double calculateDiscount(double amount) const override {
        return amount * (1 - discountRate);
    }

    void display() const override {
        cout << "ID: " << customerID << "\tName: " << name 
             << "\tType: VIP (Discount " << fixed << setprecision(0) << discountRate*100 
             << "%)\tTotal Spent: $" << fixed << setprecision(2) << totalSpent 
             << "\tPoints: " << points << endl;
    }

    string getType() const override { return "VIP"; }

    void saveToFile(ofstream& out) const override {
        Customer::saveToFile(out);
        out << "V|" << discountRate << endl;
    }

    void loadFromFile(ifstream& in) override {
        Customer::loadFromFile(in);
        char type;
        in >> type; in.ignore();
        in >> discountRate; in.ignore();
    }
};

// ���ﳵ��
struct CartItem {
    Book book;
    int quantity;
    
    CartItem(const Book& b, int q) : book(b), quantity(q) {}
};

// ���ﳵ��
class ShoppingCart {
private:
    vector<CartItem> items;

public:
    void addItem(const Book& book, int quantity) {
        for (auto& item : items) {
            if (item.book.getBookID() == book.getBookID()) {
                item.quantity += quantity;
                return;
            }
        }
        items.emplace_back(book, quantity);
    }

    void removeItem(const string& bookID) {
        items.erase(remove_if(items.begin(), items.end(), 
            [&bookID](const CartItem& item) { 
                return item.book.getBookID() == bookID; 
            }), items.end());
    }

    void updateQuantity(const string& bookID, int newQuantity) {
        for (auto& item : items) {
            if (item.book.getBookID() == bookID) {
                item.quantity = newQuantity;
                return;
            }
        }
    }

    void clear() {
        items.clear();
    }

    const vector<CartItem>& getItems() const {
        return items;
    }

    double calculateTotal() const {
        double total = 0;
        for (const auto& item : items) {
            total += item.book.getPrice() * item.quantity;
        }
        return total;
    }

    void display() const {
        cout << "\n===== Shopping Cart =====\n";
        cout << left << setw(15) << "Book ID" 
             << setw(30) << "Title" 
             << setw(10) << "Price" 
             << setw(10) << "Quantity" 
             << setw(10) << "Subtotal" << endl;
        
        for (const auto& item : items) {
            cout << setw(15) << item.book.getBookID() 
                 << setw(30) << item.book.getTitle() 
                 << setw(10) << fixed << setprecision(2) << item.book.getPrice() 
                 << setw(10) << item.quantity 
                 << setw(10) << item.book.getPrice() * item.quantity << endl;
        }
        
        cout << "\nTotal: $" << fixed << setprecision(2) << calculateTotal() << endl;
    }
};

// ��������
class InventoryManager {
private:
    vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    bool removeBook(const string& bookID) {
        auto it = find_if(books.begin(), books.end(), 
            [&bookID](const Book& b) { return b.getBookID() == bookID; });
        
        if (it != books.end()) {
            books.erase(it);
            return true;
        }
        return false;
    }

    Book* findBook(const string& bookID) {
        for (auto& book : books) {
            if (book.getBookID() == bookID) {
                return &book;
            }
        }
        return nullptr;
    }

    void updateStock(const string& bookID, int quantity) {
        for (auto& book : books) {
            if (book.getBookID() == bookID) {
                book.setStock(book.getStock() + quantity);
                return;
            }
        }
    }

    void displayAllBooks() const {
        cout << "\n===== Book Inventory =====\n";
        cout << left << setw(15) << "Book ID" 
             << setw(30) << "Title" 
             << setw(20) << "Author" 
             << setw(20) << "Publisher" 
             << setw(10) << "Price" 
             << setw(10) << "Stock" << endl;
        
        for (const auto& book : books) {
            book.display();
        }
    }

    void loadFromFile() {
        ifstream in(INVENTORY_FILE);
        if (!in) return;

        books.clear();
        while (in.peek() != EOF) {
            Book book;
            book.loadFromFile(in);
            books.push_back(book);
        }
        in.close();
    }

    void saveToFile() const {
        ofstream out(INVENTORY_FILE);
        if (!out) return;

        for (const auto& book : books) {
            book.saveToFile(out);
        }
        out.close();
    }
};

// �ͻ�������
class CustomerManager {
private:
    vector<Customer*> customers;

public:
    ~CustomerManager() {
        for (auto customer : customers) {
            delete customer;
        }
    }

    void addCustomer(Customer* customer) {
        customers.push_back(customer);
    }

    bool removeCustomer(const string& customerID) {
        auto it = find_if(customers.begin(), customers.end(), 
            [&customerID](const Customer* c) { return c->getCustomerID() == customerID; });
        
        if (it != customers.end()) {
            delete *it;
            customers.erase(it);
            return true;
        }
        return false;
    }

    Customer* findCustomer(const string& customerID) {
        for (auto customer : customers) {
            if (customer->getCustomerID() == customerID) {
                return customer;
            }
        }
        return nullptr;
    }

    void displayAllCustomers() const {
        cout << "\n===== Customer List =====\n";
        for (const auto customer : customers) {
            customer->display();
        }
    }

    void loadFromFile() {
        ifstream in(CUSTOMER_FILE);
        if (!in) return;

        customers.clear();
        while (in.peek() != EOF) {
            string line;
            getline(in, line);
            if (line.empty()) continue;
            
            stringstream ss(line);
            
            string id, name, address;
            double totalSpent;
            int points;
            char type;
            
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, address, '|');
            ss >> totalSpent; ss.ignore();
            ss >> points; ss.ignore();
            ss >> type; ss.ignore();
            
            Customer* customer = nullptr;
            if (type == 'R') {
                customer = new RegularCustomer(id, name, address);
            } else if (type == 'M') {
                int level;
                ss >> level; ss.ignore();
                customer = new MemberCustomer(id, name, address, level);
            } else if (type == 'V') {
                double discountRate;
                ss >> discountRate; ss.ignore();
                customer = new VIPCustomer(id, name, address, discountRate);
            }
            
            if (customer) {
                // ���������Ѻͻ���
                static_cast<Customer*>(customer)->addPurchase(totalSpent);
                customers.push_back(customer);
            }
        }
        in.close();
    }

    void saveToFile() const {
        ofstream out(CUSTOMER_FILE);
        if (!out) return;

        for (const auto customer : customers) {
            customer->saveToFile(out);
        }
        out.close();
    }
};

// ���׼�¼��
class Transaction {
private:
    string transactionID;
    string customerID;
    vector<pair<string, int>> items; // bookID and quantity
    double totalAmount;
    string date;

public:
    Transaction(string tid, string cid, const vector<pair<string, int>>& its, double amount, string d)
        : transactionID(tid), customerID(cid), items(its), totalAmount(amount), date(d) {}

    void saveToFile(ofstream& out) const {
        out << transactionID << "|" << customerID << "|" << totalAmount << "|" << date << "|";
        for (const auto& item : items) {
            out << item.first << "," << item.second << ";";
        }
        out << endl;
    }

    void display() const {
        cout << "Transaction ID: " << transactionID << "\tCustomer ID: " << customerID 
             << "\tDate: " << date << "\nTotal Amount: $" << fixed << setprecision(2) << totalAmount << endl;
        cout << "Items Purchased:\n";
        for (const auto& item : items) {
            cout << "Book ID: " << item.first << "\tQuantity: " << item.second << endl;
        }
    }
};

// ���׹�����
class TransactionManager {
private:
    vector<Transaction> transactions;

public:
    void addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);
    }

    void displayAllTransactions() const {
        cout << "\n===== Transaction History =====\n";
        for (const auto& transaction : transactions) {
            transaction.display();
            cout << "---------------------------\n";
        }
    }

    void loadFromFile() {
        ifstream in(TRANSACTION_FILE);
        if (!in) return;

        transactions.clear();
        while (in.peek() != EOF) {
            string line;
            getline(in, line);
            if (line.empty()) continue;
            
            stringstream ss(line);
            
            string tid, cid, date, itemsStr;
            double amount;
            
            getline(ss, tid, '|');
            getline(ss, cid, '|');
            ss >> amount; ss.ignore();
            getline(ss, date, '|');
            getline(ss, itemsStr);
            
            vector<pair<string, int>> items;
            stringstream itemsSS(itemsStr);
            string item;
            while (getline(itemsSS, item, ';')) {
                if (item.empty()) continue;
                size_t pos = item.find(',');
                string bookID = item.substr(0, pos);
                int quantity = stoi(item.substr(pos + 1));
                items.emplace_back(bookID, quantity);
            }
            
            transactions.emplace_back(tid, cid, items, amount, date);
        }
        in.close();
    }

    void saveToFile() const {
        ofstream out(TRANSACTION_FILE);
        if (!out) return;

        for (const auto& transaction : transactions) {
            transaction.saveToFile(out);
        }
        out.close();
    }
};

// ��ϵͳ��
class BookStoreSystem {
private:
    InventoryManager inventory;
    CustomerManager customers;
    TransactionManager transactions;
    ShoppingCart currentCart;
    Customer* currentCustomer;

    string generateTransactionID() const {
        static int counter = 0;
        return "T" + to_string(time(nullptr)) + to_string(++counter);
    }

    string getCurrentDate() const {
        time_t now = time(nullptr);
        tm* ltm = localtime(&now);
        return to_string(1900 + ltm->tm_year) + "-" 
             + to_string(1 + ltm->tm_mon) + "-" 
             + to_string(ltm->tm_mday);
    }

    bool verifyAdminPassword() {
        ifstream in(ADMIN_PASSWORD_FILE);
        string savedPassword = ADMIN_PASSWORD;
        
        if (in) {
            getline(in, savedPassword);
            in.close();
        }
        
        string input;
        cout << "Enter Admin Password: ";
        getline(cin, input);
        
        return input == savedPassword;
    }

    void saveAdminPassword(const string& password) {
        ofstream out(ADMIN_PASSWORD_FILE);
        if (out) {
            out << password;
            out.close();
        }
    }

public:
    BookStoreSystem() : currentCustomer(nullptr) {
        loadData();
    }

    ~BookStoreSystem() {
        saveData();
    }

    void loadData() {
        inventory.loadFromFile();
        customers.loadFromFile();
        transactions.loadFromFile();
    }

    void saveData() {
        inventory.saveToFile();
        customers.saveToFile();
        transactions.saveToFile();
    }

    void displayMainMenu() {
        cout << "\n===== Online Book Store System =====\n";
        cout << "1. Book Management\n";
        cout << "2. Shopping Checkout\n";
        cout << "3. Exit\n";
        cout << "===================================\n";
        cout << "Enter your choice: ";
    }

    void run() {
        int choice;
        do {
            displayMainMenu();
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: bookManagementMenu(); break;
                case 2: shoppingCheckout(); break;
                case 3: cout << "Exiting system...\n"; break;
                default: cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 3);
    }

    void bookManagementMenu() {
        if (!verifyAdminPassword()) {
            cout << "Incorrect password. No permission to modify books.\n";
            return;
        }

        int choice;
        do {
            cout << "\n===== Book Management =====\n";
            cout << "1. Add Book\n";
            cout << "2. Remove Book\n";
            cout << "3. Update Book Stock\n";
            cout << "4. View All Books\n";
            cout << "5. Customer Management\n";
            cout << "6. Change Admin Password\n";
            cout << "7. Back to Main Menu\n";
            cout << "==========================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: addBook(); break;
                case 2: removeBook(); break;
                case 3: updateBookStock(); break;
                case 4: inventory.displayAllBooks(); break;
                case 5: customerManagementMenu(); break;
                case 6: changeAdminPassword(); break;
                case 7: break;
                default: cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 7);
    }

    void changeAdminPassword() {
        string newPassword;
        cout << "Enter new admin password: ";
        getline(cin, newPassword);
        
        saveAdminPassword(newPassword);
        cout << "Password changed successfully!\n";
    }

    void addBook() {
        string id, title, author, publisher;
        double price;
        int stock;

        cout << "Enter Book ID: ";
        getline(cin, id);
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Publisher: ";
        getline(cin, publisher);
        cout << "Enter Price: ";
        cin >> price;
        cout << "Enter Stock: ";
        cin >> stock;
        cin.ignore();

        inventory.addBook(Book(id, title, author, publisher, price, stock));
        inventory.saveToFile();
        cout << "Book added successfully!\n";
    }

    void removeBook() {
        string id;
        cout << "Enter Book ID to remove: ";
        getline(cin, id);

        if (inventory.removeBook(id)) {
            inventory.saveToFile();
            cout << "Book removed successfully!\n";
        } else {
            cout << "Book not found!\n";
        }
    }

    void updateBookStock() {
        string id;
        int quantity;
        cout << "Enter Book ID: ";
        getline(cin, id);
        cout << "Enter quantity to add (negative to reduce): ";
        cin >> quantity;
        cin.ignore();

        inventory.updateStock(id, quantity);
        inventory.saveToFile();
        cout << "Stock updated successfully!\n";
    }

    void customerManagementMenu() {
        int choice;
        do {
            cout << "\n===== Customer Management =====\n";
            cout << "1. Add Customer\n";
            cout << "2. Remove Customer\n";
            cout << "3. Update Customer\n";
            cout << "4. View All Customers\n";
            cout << "5. Back to Book Management\n";
            cout << "==============================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: addCustomer(); break;
                case 2: removeCustomer(); break;
                case 3: updateCustomer(); break;
                case 4: customers.displayAllCustomers(); break;
                case 5: break;
                default: cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 5);
    }

    void addCustomer() {
        string id, name, address;
        char type;
        cout << "Enter Customer ID: ";
        getline(cin, id);
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Address: ";
        getline(cin, address);
        cout << "Enter Customer Type (R-Regular, M-Member, V-VIP): ";
        cin >> type;
        cin.ignore();

        Customer* customer = nullptr;
        if (toupper(type) == 'R') {
            customer = new RegularCustomer(id, name, address);
        } else if (toupper(type) == 'M') {
            int level;
            cout << "Enter Member Level (1-5): ";
            cin >> level;
            cin.ignore();
            customer = new MemberCustomer(id, name, address, level);
        } else if (toupper(type) == 'V') {
            double discountRate;
            cout << "Enter Discount Rate (e.g., 0.2 for 20% discount): ";
            cin >> discountRate;
            cin.ignore();
            customer = new VIPCustomer(id, name, address, discountRate);
        } else {
            cout << "Invalid customer type!\n";
            return;
        }

        customers.addCustomer(customer);
        customers.saveToFile();
        cout << "Customer added successfully!\n";
    }

    void removeCustomer() {
        string id;
        cout << "Enter Customer ID to remove: ";
        getline(cin, id);

        if (customers.removeCustomer(id)) {
            customers.saveToFile();
            cout << "Customer removed successfully!\n";
        } else {
            cout << "Customer not found!\n";
        }
    }

    void updateCustomer() {
        string id;
        cout << "Enter Customer ID to update: ";
        getline(cin, id);
        
        Customer* customer = customers.findCustomer(id);
        if (!customer) {
            cout << "Customer not found!\n";
            return;
        }
        
        string name, address;
        cout << "Enter new name (or press Enter to keep current): ";
        getline(cin, name);
        if (!name.empty()) {
            customer->setName(name);
        }
        
        cout << "Enter new address (or press Enter to keep current): ";
        getline(cin, address);
        if (!address.empty()) {
            customer->setAddress(address);
        }
        
        // ���»�Ա�ȼ����ۿ���
        if (customer->getType() == "Member") {
            MemberCustomer* mc = dynamic_cast<MemberCustomer*>(customer);
            if (mc) {
                int level;
                cout << "Enter new member level (1-5, or 0 to skip): ";
                cin >> level;
                cin.ignore();
                if (level >= 1 && level <= 5) {
                    mc->setLevel(level);
                }
            }
        } else if (customer->getType() == "VIP") {
            VIPCustomer* vc = dynamic_cast<VIPCustomer*>(customer);
            if (vc) {
                double discountRate;
                cout << "Enter new discount rate (e.g., 0.3 for 30%, or -1 to skip): ";
                cin >> discountRate;
                cin.ignore();
                if (discountRate >= 0 && discountRate <= 1) {
                    vc->setDiscountRate(discountRate);
                }
            }
        }
        
        customers.saveToFile();
        cout << "Customer updated successfully!\n";
    }

    void shoppingCheckout() {
        string customerID;
        cout << "Enter Customer ID: ";
        getline(cin, customerID);

        // ���ҿͻ�
        Customer* customer = customers.findCustomer(customerID);
        if (customer == nullptr) {
            // �����¿ͻ�����ͨ�û���
            string name, address;
            cout << "Welcome new customer! Please enter your name: ";
            getline(cin, name);
            cout << "Enter your address: ";
            getline(cin, address);

            customer = new RegularCustomer(customerID, name, address);
            customers.addCustomer(customer);
            customers.saveToFile();
            cout << "Registered as regular customer.\n";
        }

        currentCustomer = customer;
        cout << "\n=========================================\n";
        cout << "   Welcome, " << currentCustomer->getName() << "!\n";
        cout << "=========================================\n";

        // ��ʾ�����鼮
        inventory.displayAllBooks();

        ShoppingCart cart;
        char choice;
        do {
            cout << "\n===== Shopping Menu =====\n";
            cout << "1. Add Book to Cart\n";
            cout << "2. View Cart\n";
            cout << "3. Checkout\n";
            cout << "4. Cancel\n";
            cout << "========================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case '1': {
                    string bookID;
                    int quantity;
                    cout << "Enter Book ID: ";
                    getline(cin, bookID);
                    cout << "Enter Quantity: ";
                    cin >> quantity;
                    cin.ignore();

                    Book* book = inventory.findBook(bookID);
                    if (book) {
                        if (book->getStock() >= quantity) {
                            cart.addItem(*book, quantity);
                            cout << "Added to cart.\n";
                        } else {
                            cout << "Insufficient stock. Available: " << book->getStock() << endl;
                        }
                    } else {
                        cout << "Book not found.\n";
                    }
                    break;
                }
                case '2':
                    cart.display();
                    break;
                case '3': {
                    // ����
                    if (cart.getItems().empty()) {
                        cout << "Your cart is empty. Nothing to checkout.\n";
                        break;
                    }
                    
                    cout << "\n===== Order Summary =====\n";
                    cart.display();
                    double total = cart.calculateTotal();
                    double discountedTotal = currentCustomer->calculateDiscount(total);
                    
                    cout << "Total: $" << total << endl;
                    cout << "Discount applied: $" << total - discountedTotal << endl;
                    cout << "Amount due: $" << discountedTotal << endl;
                    
                    cout << "\nConfirm checkout? (Y/N): ";
                    char confirm;
                    cin >> confirm;
                    cin.ignore();

                    if (toupper(confirm) == 'Y') {
                        // ���¿��
                        for (const auto& item : cart.getItems()) {
                            inventory.updateStock(item.book.getBookID(), -item.quantity);
                        }
                        inventory.saveToFile();
                        
                        // ���¿ͻ���Ϣ
                        currentCustomer->addPurchase(discountedTotal);
                        customers.saveToFile();
                        
                        // �������׼�¼
                        vector<pair<string, int>> purchasedItems;
                        for (const auto& item : cart.getItems()) {
                            purchasedItems.emplace_back(item.book.getBookID(), item.quantity);
                        }
                        
                        string tid = generateTransactionID();
                        string date = getCurrentDate();
                        transactions.addTransaction(Transaction(tid, currentCustomer->getCustomerID(), 
                                                     purchasedItems, discountedTotal, date));
                        transactions.saveToFile();
                        
                        cout << "\nCheckout completed successfully!\n";
                        cout << "Transaction ID: " << tid << endl;
                        cout << "=========================================\n";
                        cout << "   Thank you for shopping with us!\n";
                        cout << "   Have a nice day!\n";
                        cout << "=========================================\n";
                        return;
                    } else {
                        cout << "Checkout cancelled.\n";
                    }
                    break;
                }
                case '4':
                    cout << "Shopping cancelled.\n";
                    return;
                default:
                    cout << "Invalid choice.\n";
            }
        } while (choice != '3' && choice != '4');
    }
};

// ��ʼ��ϵͳ����
void initializeSystem() {
    // ����Ĭ�Ϲ���Ա�����ļ�
    ofstream adminFile(ADMIN_PASSWORD_FILE);
    if (adminFile) {
        adminFile << ADMIN_PASSWORD;
        adminFile.close();
    }
    
    // ����ʾ���鼮�ļ�
    ofstream bookFile("inventory.dat");
    if (bookFile) {
        bookFile << "B001|C++ Programming|John Smith|Tech Press|45.99|100\n";
        bookFile << "B002|Data Structures|Mary Johnson|CS Publishing|39.95|80\n";
        bookFile << "B003|Algorithms|Robert Brown|Algorithm Books|55.50|60\n";
        bookFile << "B004|Web Development|Sarah Davis|Web Publishers|29.99|120\n";
        bookFile.close();
    }
    
    // ����ʾ���ͻ��ļ�
    ofstream customerFile(CUSTOMER_FILE);
    if (customerFile) {
        customerFile << "C001|Alice Johnson|123 Main St|0|0|R\n";
        customerFile << "C002|Bob Williams|456 Oak Ave|0|0|M|3\n";
        customerFile << "C003|Carol Davis|789 Pine Rd|0|0|V|0.25\n";
        customerFile.close();
    }
}

// ���Windowsƽ̨��ڵ�����
#ifdef _WIN32
int WinMain() {
#else
int main() {
#endif
    // ��ʼ��ϵͳ���ݣ������һ�����У�
    initializeSystem();
    
    BookStoreSystem system;
    system.run();
    return 0;
}