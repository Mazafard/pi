#pragma once
#include "../core/pi_types.hpp"

// Interface for all pi-approximation algorithms.
// Satisfies Open/Closed Principle: add new algorithms without touching existing code.
class IPiAlgorithm {
public:
    virtual ~IPiAlgorithm() = default;
    virtual void     reset()     = 0;
    virtual mp_float step(int n) = 0;
};
