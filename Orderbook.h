//
// Created by trist on 11/11/2023.
//

#ifndef SIMPLE_MATCHING_ENGINE_ORDERBOOK_H
#define SIMPLE_MATCHING_ENGINE_ORDERBOOK_H


#include <queue>
#include <unordered_map>
#include "Order.h"
#include "Limit.h"

using limitp = std::shared_ptr<Limit>;

struct OrderBuy {
    bool operator()(const limitp& first, const limitp& right) const;
};

struct OrderSell {
    bool operator()(const limitp& first, const limitp& right) const;
};

class Orderbook {
    using BuyOrdersPriorityQueue = std::priority_queue<limitp, std::vector<limitp>, OrderBuy>;
    using SellOrdersPriorityQueue = std::priority_queue<limitp, std::vector<limitp>, OrderSell>;
public:
    std::string stock;

    explicit Orderbook(std::string& stock);

    void handleAdd(const std::shared_ptr<Order>& o);
    void handleCancel(int orderId);
    void handleModify(int orderId, int firm, Side s, int qty, int price);

    void print();
private:
    BuyOrdersPriorityQueue buyLimits;
    SellOrdersPriorityQueue sellLimits;

    std::unordered_map<int, limitp> buyLimitsMap;
    std::unordered_map<int, limitp> sellLimitsMap;

    std::unordered_map<int, Limit::orderp> orders;

    Limit::orderp getBestBuy();
    Limit::orderp getBestSell();

    limitp getLimit(int price, Side side);

    void addOrder(const Limit::orderp& o);
    void cancelOrder(int id);

    void match();

    void reduceQty(const Limit::orderp& o, int qty);

    void printBestBuys();
    void printBestSells();
};


#endif //SIMPLE_MATCHING_EBuiNGINE_ORDERBOOK_H
