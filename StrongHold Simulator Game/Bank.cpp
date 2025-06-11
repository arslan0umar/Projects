#include "Stronghold.h"
#include <iostream>
using namespace std;

Bank::Bank() : loan(0) {}

void Bank::menu(ResourceManager& rm, int playerId) {
    int choice;
    cout << "\n===== Bank Menu =====\n";
    cout << "Current loan: " << loan << " gold\n";
    cout << "1. Take Loan\n";
    cout << "2. Repay Loan\n";
    cout << "3. Back to Main Menu\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
    case 1: {
        int amt;
        cout << "Enter loan amount: ";
        cin >> amt;
        if (amt > 0) {
            rm.gather("Gold", amt, playerId);
            loan += amt;
            int interest = amt / 10; // 10% interest
            loan += interest;
            cout << "Loan taken: " << amt << " gold with " << interest << " gold interest.\n";
            cout << "Total debt: " << loan << " gold\n";
        }
        else {
            cout << "Invalid amount. Must be positive.\n";
        }
        break;
    }
    case 2: {
        int repay;
        cout << "Enter repay amount (max " << loan << "): ";
        cin >> repay;
        if (repay <= 0) {
            cout << "Invalid amount. Must be positive.\n";
            break;
        }
        if (repay > loan) {
            cout << "Amount exceeds your loan. Maximum payment possible: " << loan << endl;
            repay = loan;
        }
        if (rm.hasResource("Gold", repay)) {
            rm.consume("Gold", repay, playerId);
            loan -= repay;
            cout << "Paid " << repay << " gold. Remaining debt: " << loan << " gold\n";
        }
        else {
            cout << "Not enough gold to repay. Check your treasury.\n";
        }
        break;
    }
    case 3:
        cout << "Returning to main menu.\n";
        break;
    default:
        cout << "Invalid option.\n";
    }
}

void Bank::display() const {
    cout << "Bank Status:\n";
    cout << "  Current loan: " << loan << " gold\n";
    if (loan > 0) {
        cout << "  Interest accruing at 10% rate.\n";
    }
    else {
        cout << "  No current debt.\n";
    }
}

void Bank::setLoan(int newLoan) {
    loan = newLoan;
    if (loan < 0) loan = 0;
}