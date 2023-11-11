//
// Created by trist on 11/11/2023.
//

#include <iostream>
#include <memory>
#include "Orderbook.h"

void Orderbook::printBestBuys() {
    std::vector<limitp> removed;
    for (int i = 0; i < 3 && getBestBuy() != nullptr; i++) {
        std::cout << buyLimits.top()->price << ", " << buyLimits.top()->qty << std::endl;
        removed.push_back(buyLimits.top());
        buyLimits.pop();
    }
    for (const auto& l: removed) {
        buyLimits.push(l);
    }
}

void Orderbook::printBestSells() {
    std::vector<limitp> removed;
    for (int i = 0; i < 3 && getBestSell() != nullptr; i++) {
        std::cout << sellLimits.top()->price << ", " << sellLimits.top()->qty << std::endl;
        removed.push_back(sellLimits.top());
        sellLimits.pop();
    }
    for (const auto& l: removed) {
        sellLimits.push(l);
    }
}

void Orderbook::handleAdd(const std::shared_ptr<Order> &o) {
    addOrder(o);
    match();
}

Orderbook::Orderbook(std::string& stock)
        : stock{stock}, buyLimits{}, sellLimits{} {};


Limit::orderp Orderbook::getBestBuy() {
    while (!buyLimits.empty() && buyLimits.top()->getBest() == nullptr) {
        buyLimitsMap.erase(buyLimits.top()->price);
        buyLimits.pop();
    };
    if (buyLimits.empty()) return nullptr;
    return buyLimits.top()->getBest();
};

Limit::orderp Orderbook::getBestSell() {
    while (!sellLimits.empty() && sellLimits.top()->getBest() == nullptr) {
        sellLimitsMap.erase(sellLimits.top()->price);
        sellLimits.pop();
    };
    if (sellLimits.empty()) return nullptr;
    return sellLimits.top()->getBest();
}

limitp Orderbook::getLimit(int price, Side side) {
    if (side == Buy) {
        if (buyLimitsMap.count(price) == 0) {
            auto l = std::make_shared<Limit>(price);
            buyLimitsMap[price] = l;
            buyLimits.push(l);
        }
        return buyLimitsMap[price];
    } else {
        if (sellLimitsMap.count(price) == 0) {
            auto l = std::make_shared<Limit>(price);
            sellLimitsMap[price] = l;
            sellLimits.push(l);
        }
        return sellLimitsMap[price];
    }
}
void Orderbook::addOrder(const Limit::orderp& o) {
    orders[o->id] = o;
    getLimit(o->price, o->side)->addOrder(o);
}

void Orderbook::cancelOrder(int id) {
    reduceQty(orders[id], orders[id]->qty);
}

void Orderbook::handleCancel(int orderId) {
    cancelOrder(orderId);
}

void Orderbook::match() {
    while (getBestBuy() != nullptr && getBestSell() != nullptr && getBestBuy()->price >= getBestSell()->price) {
        int minQty = std::min(getBestBuy()->qty, getBestSell()->qty);
        reduceQty(getBestBuy(), minQty);
        reduceQty(getBestSell(), minQty);
    }
}

void Orderbook::handleModify(int orderId, int firm, Side s, int qty, int price) {
    cancelOrder(orderId);
    auto order = std::make_shared<Order>(orderId, firm, price, qty, s);
    addOrder(order);
    match();
}

void Orderbook::print() {
    std::cout << "### " << stock << " ###" << std::endl;
    std::cout << "BUYS: " << std::endl;
    printBestBuys();
    std::cout << "SELLS: " << std::endl;
    printBestSells();
}

void Orderbook::reduceQty(const Limit::orderp& o, int qty) {
    limitp l;
    if (o->side == Buy) {
        l = buyLimitsMap[o->price];
    } else {
        l = sellLimitsMap[o->price];
    }
    l->qty -= qty;
    o->qty -= qty;
    if (o->qty == 0) {
        o->cancelled = true;
        orders.erase(o->id);
    }
}

bool OrderBuy::operator()(const limitp &first, const limitp &right) const {
    return first->price < right->price;
}

bool OrderSell::operator()(const limitp &first, const limitp &right) const {
    return first->price > right->price;
}
