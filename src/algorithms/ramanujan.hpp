#pragma once
#include "algorithm.hpp"

// Ramanujan series: ~8 correct digits per step.
// Term ratio: T_k/T_{k-1} = (4k)(4k-1)(4k-2)(4k-3)*(1103+26390k)
//                         / (k^4 * 396^4 * (1103+26390(k-1)))
class Ramanujan final : public IPiAlgorithm {
    mp_float sum_, term_;

    static const mp_float& c396_4() {
        static const mp_float v = bmp::pow(mp_float(396), mp_float(4));
        return v;
    }
public:
    void reset() override { sum_ = 0; term_ = 0; }

    mp_float step(int k) override {
        if (k == 0) {
            term_ = mp_float(1103);
        } else {
            mp_float numer = mp_float(4*k) * mp_float(4*k-1) * mp_float(4*k-2) * mp_float(4*k-3)
                           * (mp_float(26390) * k + 1103);
            mp_float denom = bmp::pow(mp_float(k), mp_float(4)) * c396_4()
                           * (mp_float(26390) * (k - 1) + 1103);
            term_ *= numer / denom;
        }
        sum_ += term_;
        return mp_float(1) / (mp_float(2) * bmp::sqrt(mp_float(2)) / 9801 * sum_);
    }
};
