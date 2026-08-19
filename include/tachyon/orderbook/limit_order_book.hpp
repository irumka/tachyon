#pragma once

#include <functional>
#include <map>
#include <unordered_map>
#include <optional>
#include <cstdint>

#include "order.hpp"
#include "price_level.hpp"

namespace tachyon::orderbook {

struct OrderLocation {
    uint64_t price{0};
    Side side{Side::Buy};
};

class LimitOrderBook {
private:
    std::map<uint64_t, PriceLevel, std::greater<uint64_t>> bids_;
    std::map<uint64_t, PriceLevel> asks_;
    std::unordered_map<uint64_t, OrderLocation> order_index_;

public:
    LimitOrderBook() = default;

    bool add_order(const Order& order) {
        if (order_index_.find(order.id) != order_index_.end()) {
            return false;
        }

        if (order.side == Side::Buy) {
            auto& level = bids_[order.price];
            level.price = order.price;
            level.add_order(order);
        } else {
            auto& level = asks_[order.price];
            level.price = order.price;
            level.add_order(order);
        }

        order_index_[order.id] = {order.price, order.side};
        return true;
    }

    bool cancel_order(uint64_t order_id) {
        auto it = order_index_.find(order_id);
        if (it == order_index_.end()) {
            return false;
        }

        const auto loc = it->second;

        if (loc.side == Side::Buy) {
            auto level_it = bids_.find(loc.price);
            if (level_it != bids_.end()) {
                level_it->second.remove_order(order_id);
                if (level_it->second.orders.empty()) {
                    bids_.erase(level_it);
                }
            }
        } else {
            auto level_it = asks_.find(loc.price);
            if (level_it != asks_.end()) {
                level_it->second.remove_order(order_id);
                if (level_it->second.orders.empty()) {
                    asks_.erase(level_it);
                }
            }
        }

        order_index_.erase(it);
        return true;
    }

    std::optional<uint64_t> best_bid() const {
        if (bids_.empty()) return std::nullopt;
        return bids_.begin()->first;
    }

    std::optional<uint64_t> best_ask() const {
        if (asks_.empty()) return std::nullopt;
        return asks_.begin()->first;
    }

    PriceLevel* best_bid_level() {
        if (bids_.empty()) return nullptr;
        return &bids_.begin()->second;
    }

    PriceLevel* best_ask_level() {
        if (asks_.empty()) return nullptr;
        return &asks_.begin()->second;
    }

    PriceLevel* get_price_level(Side side, uint64_t price) {
        if (side == Side::Buy) {
            auto it = bids_.find(price);
            return (it != bids_.end()) ? &it->second : nullptr;
        } else {
            auto it = asks_.find(price);
            return (it != asks_.end()) ? &it->second : nullptr;
        }
    }
};

}