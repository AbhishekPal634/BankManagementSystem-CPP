#include <bits/stdc++.h>
using namespace std;

struct BankAccount {
    string id;
    string password;
    double balance;
};

class BankManagementSystem {
private:
    vector<BankAccount> accounts;
    string filename;

public:
    BankManagementSystem(const string &file) : filename(file) {
        loadAccounts();
    }

    void loadAccounts() {
        ifstream fileStream(filename);

        if (!fileStream.is_open()) {
            cerr << "Error: Could not open the file.\n";
            return;
        }

        string line;
        while (getline(fileStream, line)) {
            stringstream ss(line);
            string id, password, balanceStr;

            if (getline(ss, id, ',') && getline(ss, password, ',')) {
                try {
                    if (getline(ss, balanceStr, ',')) {
                        BankAccount account;
                        account.id = id;
                        account.password = password;
                        account.balance = stod(balanceStr);
                        accounts.push_back(account);
                    } else {
                        cerr << "Error: Invalid balance format for account " << id << "\n";
                    }
                } catch (const invalid_argument &e) {
                    cerr << "Error: Invalid balance value for account " << id << "\n";
                }
            } else {
                cerr << "Error: Invalid account information format\n";
            }
        }

        fileStream.close();
    }

    void saveAccounts() {
        ofstream fileStream(filename);

        if (!fileStream.is_open()) {
            cerr << "Error: Could not open the file.\n";
            return;
        }

        for (const auto &account : accounts) {
            fileStream << account.id << "," << account.password << "," << account.balance << "\n";
        }

        fileStream.close();
    }

    BankAccount *findAccount(const string &id) {
        for (auto &account : accounts) {
            if (account.id == id) {
                return &account;
            }
        }
        return nullptr;
    }

    void displayAccountInfo(const BankAccount &account) {
        cout << "Account ID: " << account.id << "\n";
        cout << "Balance: Rs." << account.balance << "\n";
    }

    bool isAccountExist(const string &id) {
        for (const auto &account : accounts) {
            if (account.id == id) {
                return true;
            }
        }
        return false;
    }

    void deposit(BankAccount &account, double amount) {
        account.balance += amount;
        saveAccounts();
        cout << "Deposit completed. Updated balance: Rs." << account.balance << "\n";
    }

    void withdraw(BankAccount &account, double amount) {
        if (amount > account.balance) {
            cout << "Insufficient funds. Withdrawal failed.\n";
        } else {
            account.balance -= amount;
            saveAccounts();
            cout << "Withdrawal completed. Updated balance: Rs." << account.balance << "\n";
        }
    }

    void createAccount(const string &id, const string &password, double balance) {
        if (isAccountExist(id)) {
            cout << "Account with this ID already exists. Cannot create account.\n";
            return;
        }

        BankAccount account;
        account.id = id;
        account.password = password;
        account.balance = balance;
        accounts.push_back(account);
        saveAccounts();
        cout << "Account created successfully!\n";
    }

    void checkBalance(const BankAccount &account) {
        cout << "Account Balance for Account ID " << account.id << ": Rs." << account.balance << "\n";
    }
};

int main() {
    BankManagementSystem bankSystem("account_info.csv");

    int choice;
    cout << "Choose an option:\n";
    cout << "1. Login\n";
    cout << "2. Create a new account\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        string userId, password;
        cout << "Enter account ID: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, userId);

        BankAccount *userAccount = bankSystem.findAccount(userId);

        if (userAccount == nullptr) {
            cout << "Account not found.\n";
            return 1;
        }

        cout << "Enter password: ";
        cin >> password;

        if (password != userAccount->password) {
            cout << "Incorrect password.\n";
            return 1;
        }

        cout << "Login successful!\n";
        bankSystem.displayAccountInfo(*userAccount);

        int operationChoice;
        cout << "Choose an operation:\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Check Balance\n";
        cout << "Enter your choice: ";
        cin >> operationChoice;

        double amount;
        if (operationChoice == 1) {
            cout << "Enter deposit amount: Rs.";
            cin >> amount;
            bankSystem.deposit(*userAccount, amount);
        } else if (operationChoice == 2) {
            cout << "Enter withdrawal amount: Rs.";
            cin >> amount;
            bankSystem.withdraw(*userAccount, amount);
        } else if (operationChoice == 3) {
            bankSystem.checkBalance(*userAccount);
        } else {
            cout << "Invalid choice. Exiting...\n";
        }
    } else if (choice == 2) {
        string newUserId, newPassword;
        double initialBalance;
        cout << "Enter a new account ID: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, newUserId);

        if (bankSystem.isAccountExist(newUserId)) {
            cout << "Account with this ID already exists. Cannot create account.\n";
            return 1;
        }

        cout << "Enter a password: ";
        getline(cin, newPassword);
        cout << "Enter an initial balance: Rs.";
        cin >> initialBalance;

        bankSystem.createAccount(newUserId, newPassword, initialBalance);
    } else {
        cout << "Invalid choice. Exiting...\n";
    }

    return 0;
}
