#pragma once

#include <cstdint>

namespace tachyon::orderbook {

enum class Side : uint8_t {
    Buy,
    Sell
};

enum class OrderType : uint8_t {
    Market,
    Limit
};

struct Order {
    uint64_t id;
    uint64_t timestamp;
    uint64_t price; 
    uint32_t volume;
    Side side;
    OrderType type;
};

    

}