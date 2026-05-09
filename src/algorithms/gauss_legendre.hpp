#pragma once
#include "algorithm.hpp"

// Gauss-Legendre: quadratic convergence — doubles correct digits each step.
class GaussLegendre final : public IPiAlgorithm {
    mp_float a_, b_, t_, p_;
public:
    void reset() override {
        a_ = 1;
        b_ = mp_float(1) / bmp::sqrt(mp_float(2));
        t_ = mp_float(1) / 4;
        p_ = 1;
    }

    mp_float step(int /*n*/) override {
        mp_float a1 = (a_ + b_) / 2;
        b_  = bmp::sqrt(a_ * b_);
        t_ -= p_ * (a_ - a1) * (a_ - a1);
        a_  = a1;
        p_ *= 2;
        return (a_ + b_) * (a_ + b_) / (4 * t_);
    }
};
