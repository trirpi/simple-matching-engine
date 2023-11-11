//
// Created by trist on 11/11/2023.
//

#ifndef SIMPLE_MATCHING_ENGINE_ORDER_H
#define SIMPLE_MATCHING_ENGINE_ORDER_H

#include "Side.h"

struct Order {
    int id;
    int firm;
    int price;
    int qty;
    Side side;
    bool cancelled;

    Order(int id, int firm, int price, int qty, Side side) :
            id{id},
            firm{firm},
            price{price},
            qty{qty},
            side{side},
            cancelled(false) {
    };
};



#endif //SIMPLE_MATCHING_ENGINE_ORDER_H
