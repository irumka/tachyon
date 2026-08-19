#pragma once

#include <cstdint>
#include <vector>
#include "order.hpp"
namespace tachyon::orderbook {

struct PriceLevel {
    uint64_t price = 0;
    uint64_t total_volume = 0;
    std::vector<Order> orders;
    
    void add_order(const Order& order) {
        orders.push_back(order);
        total_volume += order.volume;
    }

    bool remove_order(uint64_t order_id) {
        for (auto it = orders.begin(); it != orders.end(); ++it) {
            if (it->id == order_id) {
                total_volume -= it->volume;
                orders.erase(it);
                return true;
            }
        }
        return false;
    }
    bool empty() const { return orders.empty(); }


};

}