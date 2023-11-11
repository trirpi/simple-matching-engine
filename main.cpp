//
// Created by trist on 11/11/2023.
//

#include <fstream>
#include <memory>
#include <vector>
#include <sstream>
#include <iostream>
#include "Orderbook.h"
#include "Order.h"

std::unique_ptr<std::vector<std::string>> parseOrder(std::string& line) {
    auto result = std::make_unique<std::vector<std::string>>();
    std::istringstream iss(line);
    std::string s;
    while (getline( iss, s, ' ' ) ) {
        result->push_back(s);
    }
    return std::move(result);
}

int main() {
    using obp = std::shared_ptr<Orderbook>;
    std::unordered_map<std::string, obp> obs;

    std::ifstream infile("C:/Users/trist/CLionProjects/simple-matching-engine/example_input.txt");

    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty() || line[0] == '#') continue;

        auto inp_vector = parseOrder(line);
        int firm = std::stoi(inp_vector->at(1));
        std::string stock = inp_vector->at(2);
        if (!obs.contains(stock)) {
            obs[stock] = std::make_shared<Orderbook>(stock);
        }
        auto ob = obs[stock];
        int id = std::stoi(inp_vector->at(3));


        if (inp_vector->at(0) == "ADD") {
            Side side = (inp_vector->at(4) == "BUY") ? Buy : Sell;
            int qty = std::stoi(inp_vector->at(5));
            int price = std::stoi(inp_vector->at(6));

            auto order = std::make_shared<Order>(id, firm, price, qty, side);
            ob->handleAdd(order);
        } else if (inp_vector->at(0) == "CAN") {
            ob->handleCancel(id);
        } else if (inp_vector->at(0) == "MOD") {
            Side side = (inp_vector->at(4) == "BUY") ? Buy : Sell;
            int qty = std::stoi(inp_vector->at(5));
            int price = std::stoi(inp_vector->at(6));

            ob->handleModify(id, firm, side, qty, price);
        } else {
            std::cout << "input error" << std::endl;
        }
    }
    for (const auto& pair : obs) {
        pair.second->print();
    }
    return 0;
}
