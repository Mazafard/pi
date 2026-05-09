#pragma once
#include "../core/layout.hpp"
#include "raylib.h"
#include <string>
#include <algorithm>
#include <cmath>

namespace draw_utils {

inline void DrawDashedH(int x1, int y, int x2, int dash, int gap, Color c) {
    for (int x = x1; x < x2; x += dash + gap)
        DrawLine(x, y, std::min(x + dash, x2), y, c);
}

inline bool Clicked(Rectangle r) {
    return CheckCollisionPointRec(GetMousePosition(), r)
        && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

inline bool Hovered(Rectangle r) {
    return CheckCollisionPointRec(GetMousePosition(), r);
}

inline float logEtoY(double logE, float cy, float ch) {
    logE = std::max(LY_TOP, std::min(LY_BOT, logE));
    return cy + (float)((logE - LY_TOP) / (LY_BOT - LY_TOP)) * ch;
}

inline float logErrToY(double logErr, float cy, float ch) {
    return logEtoY(logErr, cy, ch);
}

inline float timeToX(double t, float cx, float cw) {
    double logT = (t > 1e-7) ? std::log10(t) : LX_MIN;
    logT = std::max(LX_MIN, std::min(LX_MAX, logT));
    return cx + (float)((logT - LX_MIN) / (LX_MAX - LX_MIN)) * cw;
}

inline void DrawPrecisionDigits(const std::string& val, const std::string& ref,
                                int x, int y, int fontSize) {
    int firstDiff = (int)std::min(val.size(), ref.size());
    for (int i = 0; i < firstDiff; i++) {
        if (val[i] != ref[i]) { firstDiff = i; break; }
    }
    if (firstDiff > 0)
        DrawText(val.substr(0, firstDiff).c_str(), x, y, fontSize, WHITE);
    if (firstDiff < (int)val.size()) {
        int px = MeasureText(val.substr(0, firstDiff).c_str(), fontSize);
        DrawText(val.substr(firstDiff).c_str(), x + px, y, fontSize, GetColor(0xFF6633FF));
    }
}

} // namespace draw_utils
