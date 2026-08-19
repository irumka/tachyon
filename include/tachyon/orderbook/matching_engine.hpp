#pragma once

#include <vector>
#include <algorithm>
#include "limit_order_book.hpp"

namespace tachyon::orderbook {

struct Trade {
    uint64_t incoming_order_id;
    uint64_t matched_order_id;
    uint64_t price;
    uint32_t volume;
    uint64_t timestamp;
};

class MatchingEngine {
    LimitOrderBook& book_;

public:
    explicit MatchingEngine(LimitOrderBook& book) : book_(book) {}

    std::vector<Trade> process_order(Order order) {
        std::vector<Trade> trades;

        while (order.volume > 0) {
            auto* level = (order.side == Side::Buy) ? book_.best_ask_level() : book_.best_bid_level();
            if (!level || level->orders.empty()) break;

            if (order.type == OrderType::Limit) {
                if (order.side == Side::Buy && level->price > order.price) break;
                if (order.side == Side::Sell && level->price < order.price) break;
            }

            auto& head = level->orders.front();
            const uint32_t qty = std::min(order.volume, head.volume);

            trades.push_back({order.id, head.id, level->price, qty, order.timestamp});

            order.volume -= qty;

            if (qty == head.volume) {
                book_.cancel_order(head.id);
            } else {
                head.volume -= qty;
                level->total_volume -= qty;
            }
        }

        if (order.type == OrderType::Limit && order.volume > 0) {
            book_.add_order(order);
        }

        return trades;
    }
};

}