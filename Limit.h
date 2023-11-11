//
// Created by trist on 11/11/2023.
//

#ifndef SIMPLE_MATCHING_ENGINE_LIMIT_H
#define SIMPLE_MATCHING_ENGINE_LIMIT_H


#include <memory>
#include <list>
#include "Order.h"


class Limit {
public:
    explicit Limit(int price);

    using orderp = std::shared_ptr<Order>;

    int price;
    int qty;
    orderp getBest();
    void addOrder(const orderp& order);

private:
    std::list<orderp> orders;
};


#endif //SIMPLE_MATCHING_ENGINE_LIMIT_H
