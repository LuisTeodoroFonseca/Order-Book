#include <iostream>
#include <stdexcept>
using namespace std;

#include "Order.hpp"
#include "Transaction.hpp"
#include "OrderBook.hpp"

OrderBook::OrderBook() {
    BuyHead = nullptr;
    SellHead = nullptr;
    TransHead = nullptr;
    TransTail = nullptr;
    sizeBuy = 0;
    sizeSell = 0;
    sizeTrans = 0;  
}

OrderBook::~OrderBook() {
    Node* currentBuy = BuyHead;
    Node* currentSell = SellHead;
    TransactionNode* currentTrans = TransHead;

    while(currentTrans != nullptr) { //percore a lista para apagar um por um
        TransactionNode* temp = currentTrans;
        currentTrans = currentTrans->next;
        delete temp;
    }

    while(currentBuy != nullptr) { //percore a lista para apagar um por um
        Node* temp = currentBuy;
        currentBuy = currentBuy->next;
        delete temp;
    }

    while(currentSell != nullptr) { //percore a lista para apagar um por um
        Node* temp = currentSell;
        currentSell = currentSell->next;
        delete temp;
    }
}

bool OrderBook::submit(Order order) {
    if (order.getType() == 'B') { // faz primeiro para ordem de compra
        if (SellHead != nullptr && SellHead->order.getPrice() <= order.getPrice()) { //como SellHead é o elemento com menor preço de venda, só precisa comparar com o primeiro elemento da lista (a lista já está ordenada pelo menor preço)
            Node* temp = SellHead;
            SellHead = SellHead->next; //transforma o SellHead no próximo elemento da lista pois o atual vai participar de uma transação
            Transaction t(order.getId(), temp->order.getId(), temp->order.getPrice()); //cria transação
            TransactionNode* current = new TransactionNode;
            current->transaction = t;
            current->next = nullptr;
            if(TransHead == nullptr) { //adiciona a transação se não tiver nenhum ainda
                TransHead = current;
                TransTail = current;
            }
            else {
                TransTail->next = current; //adiciona a transação pela tail (teria que passar pela lista toda se fosse usar o head, por isso optei pelo tail)
                TransTail = current;
            }
            sizeTrans++; //aumenta o número de transações
            sizeSell--; //diminui o número de sell orders
            delete temp;
            return true;
        }
        else if (BuyHead == nullptr || order.getPrice() > BuyHead->order.getPrice() || (order.getPrice() == BuyHead->order.getPrice() && order.getTimestamp() < BuyHead->order.getTimestamp())) {
            //adiciona a order passada como parâmetro no lugar do BuyHead atual se BuyHead for nulo, ou se o preço da compra for maior que o do BuyHead ou se o preço for igual e tiver o menor timestamp;
            Node* newnode = new Node; 
            newnode->order = order;
            newnode->next = nullptr;
            newnode->next = BuyHead;
            BuyHead = newnode;
            sizeBuy++; //aumenta o número de buy orders
            return false;
        }
        else {
            Node* newnode = new Node;
            newnode->order = order;
            newnode->next = nullptr;
            Node* temp = BuyHead;
            while(temp->next != nullptr) { //encontra o lugar certo para a order na lista encadeada (ela está ordenada primeiramente pelo maior preço e segundamente pelo menor timestamp)
                float nextprice = temp->next->order.getPrice();
                int nexttimestamp = temp->next->order.getTimestamp();
                if (newnode->order.getPrice() > nextprice) 
                    break;

                if (newnode->order.getPrice() == nextprice && newnode->order.getTimestamp() < nexttimestamp)
                    break;

                temp = temp->next;
            }
            newnode->next = temp->next; //adiciona a order no sistema
            temp->next = newnode;
            sizeBuy++; //aumenta o número de buy orders no sistema
            return false;
        }
    }
    else { //agora analisa a ordem de venda
        if (BuyHead != nullptr && BuyHead->order.getPrice() >= order.getPrice()) { //como BuyHead é o elemento com maior preço de compra, só precisa comparar com o primeiro elemento da lista (a lista já está ordenada pelo maior preço)
            Node* temp = BuyHead;
            BuyHead = BuyHead->next; //transforma o BuyHead no próximo elemento da lista pois o atual vai participar de uma transação
            Transaction t(temp->order.getId(), order.getId(), temp->order.getPrice()); //cria transação
            TransactionNode* current = new TransactionNode;
            current->transaction = t;
            current->next = nullptr;
            if(TransHead == nullptr) { //adiciona transação se não tiver nenhuma ainda
                TransHead = current;
                TransTail = current;
            }
            else {
                TransTail->next = current; //adiciona a transação pela tail (teria que passar pela lista toda se fosse usar o head, por isso optei pelo tail)
                TransTail = current;
            }
            sizeTrans++; //aumenta número de transações
            sizeBuy--; //aumenta número de buy orders
            delete temp;
            return true;
        }
        else if (SellHead == nullptr || order.getPrice() < SellHead->order.getPrice() || (order.getPrice() == SellHead->order.getPrice() && order.getTimestamp() < SellHead->order.getTimestamp())) {
            //adiciona a order passada como parâmetro no lugar do SellHead atual se SellHead for nulo, ou se o preço da venda for menor que o do SellHead ou se o preço for igual e tiver o menor timestamp;
            Node* newnode = new Node;
            newnode->order = order;
            newnode->next = nullptr;
            newnode->next = SellHead;
            SellHead = newnode;
            sizeSell++; //aumenta o número de sell orders
            return false;
        }
        else {
            Node* newnode = new Node;
            newnode->order = order;
            newnode->next = nullptr;
            Node* temp = SellHead;
            while(temp->next != nullptr) {  //encontra o lugar certo para a order na lista encadeada (ela está ordenada primeiramente pelo menor preço e segundamente pelo menor timestamp)
                float nextprice = temp->next->order.getPrice();
                int nexttimestamp = temp->next->order.getTimestamp();

                if (newnode->order.getPrice() < nextprice) 
                    break;

                if (newnode->order.getPrice() == nextprice && newnode->order.getTimestamp() < nexttimestamp)
                    break;

                temp = temp->next;
            }
            newnode->next = temp->next; //adiciona a order no sistema no lugar certo
            temp->next = newnode;
            sizeSell++; //aumenta o número de sell orders
            return false;
        }
    }
}

bool OrderBook::cancel(int id) {

    if(BuyHead == nullptr && SellHead == nullptr) { //caso as duas listas estiverem vazias
        return false; 
    }
    else if(BuyHead->order.getId() == id) { //caso o id seja igual ao id de BuyHead
        Node* temp = BuyHead;
        BuyHead = BuyHead->next;
        sizeBuy--;
        delete temp;
        return true;
    }
    else if(SellHead->order.getId() == id) { //caso o id seja igual ao id de SellHead
        Node* temp = SellHead;
        SellHead = SellHead->next;
        sizeSell--;
        delete temp;
        return true;
    }
    else {
        if(BuyHead != nullptr) {
            Node* currentBuy = BuyHead;
            while(currentBuy->next != nullptr) { //vai percorrer toda a lista de buy orders para procurar se tem algum id igual
                if(currentBuy->next->order.getId() == id) {
                    if(currentBuy->next->next != nullptr) { //caso a buy order com o id igual seja a última na lista encadeada
                        Node* temp = currentBuy->next;
                        currentBuy->next = currentBuy->next->next;
                        sizeBuy--; //diminui o tamanho das buy orders
                        delete temp;
                        return true;
                    }
                    else { //caso a buy order com o id igual esteja no meio da lista encadeada
                        Node* temp = currentBuy->next;
                        currentBuy->next = nullptr;
                        sizeBuy--; //diminui o tamanho das buy orders
                        delete temp;
                        return true;
                    }
                }
                currentBuy = currentBuy->next; //passa para a próxima buy order da lista
            }
        }
        if(SellHead != nullptr) {
            Node* currentSell = SellHead;
            while(currentSell->next != nullptr) { //vai percorrer toda a lista de sell orders para procurar se tem algum id igual
                if(currentSell->next->order.getId() == id) {
                    if(currentSell->next->next != nullptr) { //caso a sell order com o id igual seja a última na lista encadeada
                        Node* temp = currentSell->next;
                        currentSell->next = currentSell->next->next;
                        sizeSell--; //diminui o tamanho das sell orders
                        delete temp;
                        return true;
                    }
                    else {
                        Node* temp = currentSell->next;
                        currentSell->next = nullptr;
                        sizeSell--; //diminui o tamanho das sell orders
                        delete temp;
                        return true;
                    }
                }
                currentSell = currentSell->next; //passa para a próxima sell order da lista
            }
        }
    }
    return false;
}

Order* OrderBook::getBuyOrders(int* n) {
    if (sizeBuy == 0) { //caso a lista encadeada esteja vazia
        *n = 0;
        return nullptr;
    }
    *n = sizeBuy;
    Order* orders = new Order[sizeBuy];
    Node* temp = BuyHead;
    for (int i = 0; i < sizeBuy; i++) { //passa cada elemento da lista encadeada para o array dinâmico
        orders[i] = temp->order;
        temp = temp->next;
    }

    return orders;
}

Order* OrderBook::getSellOrders(int* n) {
    if (sizeSell == 0) { //caso a lista encadeada esteja vazia
        *n = 0;
        return nullptr;
    }
    *n = sizeSell;
    Order* orders = new Order[sizeSell];
    Node* temp = SellHead;
    for (int i = 0; i < sizeSell; i++) { //passa cada elemento da lista encadeada para o array dinâmico
        orders[i] = temp->order;
        temp = temp->next;
    }

    return orders;
}

Transaction* OrderBook::getTransactions(int* n) {
    if (sizeTrans == 0) { //caso a lista encadeada esteja vazia
        *n = 0;
        return nullptr;
    }
    *n = sizeTrans;
    Transaction* transactions = new Transaction[sizeTrans];
    TransactionNode* temp = TransHead;
    for (int i = 0; i < sizeTrans; i++) { //passa cada elemento da lista encadeada para o array dinâmico
        transactions[i] = temp->transaction;
        temp = temp->next;
    }

    return transactions;
}

void OrderBook::printBuyOrders() {
    int n;
    Order* orders = getBuyOrders(&n); //transforma a lista encadeada em um array dinâmico já ordenado
    cout << "Buy Orders:" << endl;
    if(orders != nullptr) {
        for (int i = 0; i < sizeBuy; i++) { //printa elemento a elemento do array dinâmico
            cout << "[" << orders[i].getId() << " | " << orders[i].getPrice() << " | " << orders[i].getTimestamp() << "]" << endl;
        }
        cout << endl; //apenas para dar \n depois de printar todas as buy orders
        delete[] orders;
    }
    else {
        cout << "(empty)" << endl;
    }
}

void OrderBook::printSellOrders() {
    int n;
    Order* orders = getSellOrders(&n); //transforma a lista encadeada em um array dinâmico já ordenado
    cout << "Sell Orders:" << endl;
    if(orders != nullptr) {
        for (int i = 0; i < sizeSell; i++) { //printa elemento a elemento do array dinâmico
            cout << "[" << orders[i].getId() << " | " << orders[i].getPrice() << " | " << orders[i].getTimestamp() << "]" << endl;
        }
        cout << endl; //apenas para dar \n depois de printar todas as sell orders
        delete[] orders;
    }
    else {
        cout << "(empty)" << endl;
    }
}

void OrderBook::printTransactions() {
    int n;
    Transaction* transactions = getTransactions(&n); //transforma a lista encadeada em um array dinâmico já ordenado
    cout << "Transactions:" << endl;
    if(transactions != nullptr) {
        for (int i = 0; i < sizeTrans; i++) { //printa elemento a elemento do array dinâmico
            cout << "[" << transactions[i].getBuyOrderId() << " , " << transactions[i].getSellOrderId() << " , " << transactions[i].getExecutionPrice() << "]" << endl;
        }
        cout << endl; //apenas para dar \n depois de printar todas as transações
        delete[] transactions;
    }
    else {
        cout << "(empty)" << endl;
    }
}