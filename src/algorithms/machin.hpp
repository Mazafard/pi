#pragma once
#include "algorithm.hpp"

// Machin arctan formula: pi/4 = 4*arctan(1/5) - arctan(1/239).
// Linear convergence — ~1.4 correct digits per step.
class Machin final : public IPiAlgorithm {
    mp_float sum1_, sum2_, x1p_, x1sq_, x2p_, x2sq_;
public:
    void reset() override {
        sum1_ = sum2_ = 0;
        mp_float x1 = mp_float(1) / 5;
        mp_float x2 = mp_float(1) / 239;
        x1p_ = x1; x1sq_ = x1 * x1;
        x2p_ = x2; x2sq_ = x2 * x2;
    }

    mp_float step(int n) override {
        mp_float d = 2 * n + 1;
        if (n % 2 == 0) { sum1_ += x1p_ / d; sum2_ += x2p_ / d; }
        else            { sum1_ -= x1p_ / d; sum2_ -= x2p_ / d; }
        x1p_ *= x1sq_;
        x2p_ *= x2sq_;
        return 16 * sum1_ - 4 * sum2_;
    }
};
