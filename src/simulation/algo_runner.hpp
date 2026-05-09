#pragma once
#include "../core/pi_types.hpp"
#include "../algorithms/algorithm.hpp"
#include "raylib.h"
#include <memory>
#include <string>
#include <vector>

struct DataPt {
    double t;
    double logErr;
};

// Owns an IPiAlgorithm plus all runtime convergence state for one algorithm slot.
struct AlgoRunner {
    const char* name      = "";
    const char* desc      = "";
    Color       color     = WHITE;
    int         max_iters = 0;

    bool        active    = true;
    bool        converged = false;
    int         iters     = 0;
    mp_float    val;
    double      cumul_cpu = 0.0;
    double      conv_time = 0.0;
    std::string cached_val_str;
    int         cached_ndp = 0;

    std::vector<DataPt>           history;
    std::unique_ptr<IPiAlgorithm> algo;

    void reset();

    // Times and executes one step. Records convergence into conv_order if reached.
    // Returns false if already done (converged / max_iters / inactive).
    bool advance(const mp_float& conv_eps, std::vector<int>& conv_order, int self_idx);
};
