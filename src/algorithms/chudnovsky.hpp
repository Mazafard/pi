#pragma once
#include "algorithm.hpp"

// Chudnovsky series: ~14 correct digits per step (fastest known series).
// Term ratio: T_k/T_{k-1} = -(6k)(6k-1)..(6k-5)*(13591409+545140134k)
//                          / ((3k)(3k-1)(3k-2) * k^3 * 640320^3 * (13591409+545140134(k-1)))
class Chudnovsky final : public IPiAlgorithm {
    mp_float sum_, term_;

    static const mp_float& c640_3() {
        static const mp_float v = bmp::pow(mp_float(640320), mp_float(3));
        return v;
    }
    static const mp_float& c640_15() {
        static const mp_float v = bmp::pow(mp_float(640320), mp_float(3) / 2);
        return v;
    }
public:
    void reset() override { sum_ = 0; term_ = 0; }

    mp_float step(int k) override {
        if (k == 0) {
            term_ = mp_float(13591409) / c640_15();
        } else {
            mp_float numer = -(mp_float(6*k)   * mp_float(6*k-1) * mp_float(6*k-2)
                             * mp_float(6*k-3) * mp_float(6*k-4) * mp_float(6*k-5))
                           * (mp_float(545140134) * k + 13591409);
            mp_float denom = mp_float(3*k) * mp_float(3*k-1) * mp_float(3*k-2)
                           * bmp::pow(mp_float(k), mp_float(3)) * c640_3()
                           * (mp_float(545140134) * (k - 1) + 13591409);
            term_ *= numer / denom;
        }
        sum_ += term_;
        return mp_float(1) / (12 * sum_);
    }
};
