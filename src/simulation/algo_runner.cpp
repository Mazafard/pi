#include "algo_runner.hpp"
#include "../utils/format_utils.hpp"
#include <cmath>
#include <chrono>

void AlgoRunner::reset() {
    history.clear();
    iters     = 0;
    val       = mp_float(0);
    converged = false;
    cumul_cpu = 0.0;
    conv_time = 0.0;
    cached_val_str.clear();
    cached_ndp = 0;
    algo->reset();
}

bool AlgoRunner::advance(const mp_float& conv_eps,
                         std::vector<int>& conv_order, int self_idx) {
    if (!active || converged || iters >= max_iters)
        return false;

    // Anchor so all curves share the same t=0 origin.
    if (history.empty())
        history.push_back({0.0, std::log10(PI_D)});

    auto t0 = hclock::now();
    val = algo->step(iters);
    double dt = std::chrono::duration<double>(hclock::now() - t0).count();
    cumul_cpu += dt;

    double err    = (double)bmp::abs(val - TRUE_PI);
    int    ndp    = fmt_utils::correctDecimalPlaces(val);
    double logErr = (err > 0.0) ? std::log10(err) : -(double)ndp;

    history.push_back({cumul_cpu, logErr});
    iters++;
    cached_val_str = fmt_utils::fmt(val, 1000);
    cached_ndp     = ndp;

    if (bmp::abs(val - TRUE_PI) < conv_eps) {
        converged = true;
        conv_time = cumul_cpu;
        conv_order.push_back(self_idx);
    }

    return true;
}
