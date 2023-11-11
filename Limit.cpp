//
// Created by trist on 11/11/2023.
//

#include "Limit.h"

Limit::orderp Limit::getBest() {
    while (!orders.empty() && orders.front()->cancelled) {
        orders.pop_front();
    }
    if (orders.empty()) return nullptr;
    return orders.front();
}

Limit::Limit(int price) : price{price}, qty{0} {

}

void Limit::addOrder(const Limit::orderp& order) {
    qty += order->qty;
    orders.push_back(order);
}
