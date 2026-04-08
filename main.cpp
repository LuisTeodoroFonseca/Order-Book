#include <iostream>
#include <stdexcept>
#include <windows.h>
using namespace std;

#include "Order.hpp"
#include "OrderBook.hpp"
#include "Transaction.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);

    OrderBook orderbook;
    Order order1 {1, 'B', 15.00, 1};
    Order order2 {2, 'S', 12.00, 2};
    Order order3 {3, 'S', 27.00, 3};
    Order order4 {4, 'B', 9.00, 4};
    Order order5 {5, 'B', 30.00, 5};
    Order order6 {6, 'S', 30.00, 6};
    Order order7 {7, 'B', 4.00, 7};
    Order order8 {8, 'S', 45.00, 8};
    Order order9 {9, 'B', 26.00, 9};
    orderbook.submit(order1);
    orderbook.submit(order2);
    orderbook.submit(order3);
    orderbook.submit(order4);
    orderbook.submit(order5);
    orderbook.submit(order6);
    orderbook.submit(order7);
    orderbook.submit(order8);
    orderbook.submit(order9);
    orderbook.cancel(7);
    orderbook.cancel(8);
    orderbook.printBuyOrders();
    orderbook.printSellOrders();
    orderbook.printTransactions();
    int n;
    Order* orders = orderbook.getSellOrders(&n);
    for (int i = 0; i < n; i++) {
        cout << "\nOrder:" << "[" << orders[i].getId() << " | " << orders[i].getPrice() << " | " << orders[i].getTimestamp() << "]" << endl;
    }
    delete[] orders;
    return 0;
}