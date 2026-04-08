#ifndef ORDER_HPP
#define ORDER_HPP

class Order {

private:
    int id;
    char type; // 'B' ou 'S'
    float price;
    int timestamp;

public:

    Order();
    Order(int id, char type, float price, int timestamp);
    ~Order();

    int getId();
    char getType();
    float getPrice();
    int getTimestamp();
};

#endif