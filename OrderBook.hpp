#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include "Order.hpp"
#include "Transaction.hpp"

struct Node { //struct para armazenar as buy orders e as sell orders
    Order order;
    Node* next;
};

struct TransactionNode { //struct para armazenas as transações
    Transaction transaction;
    TransactionNode* next;
};


class OrderBook {

private:
    Node* BuyHead;
    Node* SellHead;
    TransactionNode* TransHead;
    TransactionNode* TransTail;
    int sizeBuy;
    int sizeSell;
    int sizeTrans;
    // Estruturas internas escolhidas pelos alunos
    // para armazenar ordens de compra, venda e transações

public:

    OrderBook();
    ~OrderBook();

    bool submit(Order order);
    bool cancel(int id);

    Order* getBuyOrders(int* n);
    Order* getSellOrders(int* n);
    Transaction* getTransactions(int* n);

    void printBuyOrders();
    void printSellOrders();
    void printTransactions();

    // Outros métodos auxiliares, se necessário
};

#endif