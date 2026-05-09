#pragma once
#include "../core/pi_types.hpp"
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace fmt_utils {

inline std::string fmt(const mp_float& v, int prec) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(prec) << v;
    return ss.str();
}

inline std::string fmtSci(const mp_float& v, int prec = 3) {
    std::ostringstream ss;
    ss << std::scientific << std::setprecision(prec) << v;
    return ss.str();
}

inline std::string fmtTime(double t) {
    std::ostringstream ss;
    if      (t < 0.001) ss << std::fixed << std::setprecision(0) << t * 1e6 << "us";
    else if (t < 1.0)   ss << std::fixed << std::setprecision(1) << t * 1e3 << "ms";
    else                ss << std::fixed << std::setprecision(2) << t        << "s";
    return ss.str();
}

inline int correctDecimalPlaces(const mp_float& val) {
    std::string vs = fmt(val, 1010);
    std::string ts = fmt(TRUE_PI, 1010);
    int count = 0;
    for (int i = 2; i < (int)std::min(vs.size(), ts.size()); i++) {
        if (vs[i] == ts[i]) count++;
        else break;
    }
    return count;
}

} // namespace fmt_utils
