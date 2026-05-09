#include "renderer.hpp"
#include "../core/layout.hpp"
#include "../utils/format_utils.hpp"
#include "draw_utils.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace draw_utils;

Renderer::Renderer(std::string true_str_1000)
    : true_str_1000_(std::move(true_str_1000)) {}

// ── Public entry point ────────────────────────────────────────────────────────

void Renderer::draw(const std::array<AlgoRunner, 4>& runners,
                    const std::vector<int>&           conv_order,
                    bool      running,
                    bool      recording,
                    int       rec_frames,
                    float     interval,
                    Rectangle calc_btn,
                    Rectangle reset_btn,
                    Rectangle record_btn) const {
    draw_title_bar(recording, rec_frames);
    draw_sidebar(runners, running, recording, rec_frames, interval,
                 calc_btn, reset_btn, record_btn);
    draw_chart(runners, conv_order);
    draw_stats_panel(runners);
}

// ── Title bar ─────────────────────────────────────────────────────────────────

void Renderer::draw_title_bar(bool recording, int rec_frames) const {
    DrawRectangle(0, 0, W, 56, GetColor(0x1A1A1AFF));
    DrawLine(0, 56, W, 56, GetColor(0x383838FF));
    DrawText("THE RACE TO PI",
        W/2 - MeasureText("THE RACE TO PI", 26)/2, 8, 26, WHITE);
    DrawText("Beyond Machine Precision  |  1000-Digit Arithmetic  |  Actual CPU Time",
        W/2 - MeasureText("Beyond Machine Precision  |  1000-Digit Arithmetic  |  Actual CPU Time", 14)/2,
        36, 14, GetColor(0x888888FF));

    if (recording) {
        bool blink = (int)(GetTime() * 2.0) % 2 == 0;
        DrawRectangle(W-120, 10, 110, 36, GetColor(0x330000FF));
        DrawRectangleLines(W-120, 10, 110, 36, GetColor(0x882222FF));
        if (blink) DrawCircle(W-102, 28, 8, RED);
        DrawText("REC", W-88, 19, 18, WHITE);
        std::string fc = std::to_string(rec_frames / 60) + "s";
        DrawText(fc.c_str(), W-52, 22, 14, GetColor(0xAA6666FF));
    }
}

// ── Sidebar ───────────────────────────────────────────────────────────────────

void Renderer::draw_sidebar(const std::array<AlgoRunner, 4>& runners,
                             bool running, bool recording, int rec_frames, float interval,
                             Rectangle calc_btn, Rectangle reset_btn,
                             Rectangle record_btn) const {
    DrawRectangle(0, 56, SB, H-56, GetColor(0x161616FF));
    DrawLine(SB, 56, SB, H, GetColor(0x2E2E2EFF));

    // Algorithm list (58px slots)
    DrawText("ALGORITHMS", 16, 66, 11, GetColor(0x666666FF));
    for (int i = 0; i < 4; i++) {
        int  y  = 82 + i * 58;
        bool on = runners[i].active;

        DrawText(runners[i].name, 16, y, 17, on ? WHITE : GetColor(0x555555FF));
        DrawText(runners[i].desc, 16, y + 20, 11,
                 on ? GetColor(0x666666FF) : GetColor(0x333333FF));

        if (runners[i].iters > 0 && on) {
            std::string eb;
            if (runners[i].converged) {
                eb = fmt_utils::fmtTime(runners[i].conv_time);
            } else {
                double err = (double)bmp::abs(runners[i].val - TRUE_PI);
                if (err > 0.0) {
                    std::ostringstream ss;
                    ss << std::scientific << std::setprecision(1) << err;
                    eb = ss.str();
                } else {
                    eb = "~0";
                }
            }
            DrawText(eb.c_str(), SB - 60 - MeasureText(eb.c_str(), 12), y + 2, 12,
                     runners[i].converged ? GetColor(0x22CC66FF) : GetColor(0x888888FF));
        }

        // Toggle switch
        Color bg = on ? GetColor(0x1A44FFFF) : GetColor(0x3A3A3AFF);
        DrawRectangleRounded({(float)(SB-60), (float)(y+34), 40, 22}, 0.55f, 6, bg);
        float kx = on ? (float)(SB-60+19) : (float)(SB-60+1);
        DrawCircle((int)(kx+10), y+45, 9.0f, WHITE);
        DrawRectangle(SB-60-16, y+38, 12, 12, on ? runners[i].color : GetColor(0x444444FF));
    }

    DrawLine(0, 314, SB, 314, GetColor(0x2A2A2AFF));

    // True Pi preview
    DrawText("TRUE PI  (1000 decimal places)", 16, 322, 13, GetColor(0x666666FF));
    {
        std::string tp = fmt_utils::fmt(TRUE_PI, 52);
        DrawText(tp.substr(0, 27).c_str(), 16, 341, 14, GetColor(0x33DD77FF));
        DrawText(tp.substr(27, 26).c_str(), 16, 359, 14, GetColor(0x33DD77FF));
        DrawText("...  (1000 digits total)", 16, 377, 12, GetColor(0x446644FF));
    }

    DrawLine(0, 396, SB, 396, GetColor(0x2A2A2AFF));

    // Iteration counter
    DrawText("ITERATIONS", 16, 406, 11, GetColor(0x666666FF));
    int max_iter = 0;
    for (const auto& r : runners)
        if (r.active && r.iters > max_iter) max_iter = r.iters;
    DrawText(std::to_string(max_iter).c_str(), 16, 422, 30, WHITE);

    DrawLine(0, 462, SB, 462, GetColor(0x2A2A2AFF));

    // Buttons
    {
        Color calc_c = Hovered(calc_btn)  ? GetColor(0x3B5BFFFF) : GetColor(0x2244EEFF);
        Color rst_c  = Hovered(reset_btn) ? GetColor(0x3A3A3AFF) : GetColor(0x272727FF);
        DrawRectangleRounded(calc_btn,  0.25f, 6, calc_c);
        DrawText(running ? "  Pause" : "  Calculate",
                 (int)calc_btn.x + 8, (int)calc_btn.y + 12, 16, WHITE);
        DrawRectangleRounded(reset_btn, 0.25f, 6, rst_c);
        DrawText("  Reset", (int)reset_btn.x + 8, (int)reset_btn.y + 12, 16, LIGHTGRAY);

        Color rec_bg = recording          ? GetColor(0x991111FF)
                     : Hovered(record_btn)? GetColor(0x553333FF)
                                          : GetColor(0x2A1515FF);
        DrawRectangleRounded(record_btn, 0.25f, 6, rec_bg);
        bool blink = recording && ((int)(GetTime() * 2.0) % 2 == 0);
        int  dot_x = (int)record_btn.x + 18;
        int  dot_y = (int)record_btn.y + 20;
        DrawCircle(dot_x, dot_y, 7.0f,
                   recording ? (blink ? RED : GetColor(0x882222FF))
                             : GetColor(0xAA3333FF));
        DrawText(recording ? "  Recording..." : "  Record",
                 (int)record_btn.x + 28, (int)record_btn.y + 12, 16,
                 recording ? WHITE : GetColor(0xDD8888FF));
    }

    // Controls hint
    DrawLine(0, H-125, SB, H-125, GetColor(0x2A2A2AFF));
    int spd = (int)(100.0f * 0.03f / interval);
    const char* hints[] = { "SPACE  Start / Pause", "R  Reset",
                             "1 2 3 4  Toggle algo", "UP/DOWN  Speed" };
    for (int i = 0; i < 4; i++)
        DrawText(hints[i], 16, H-117 + i*22, 13, GetColor(0x555555FF));
    DrawText(("Speed: " + std::to_string(spd) + "%").c_str(),
             16, H-25, 13, GetColor(0x777777FF));
}

// ── Chart ─────────────────────────────────────────────────────────────────────

void Renderer::draw_chart(const std::array<AlgoRunner, 4>& runners,
                           const std::vector<int>&           conv_order) const {
    DrawRectangle(CX, CY, CW, CH, GetColor(0x131313FF));

    // Y-axis grid: error magnitude (left) and correct-digit count (right)
    for (int le = 0; le >= -1000; le -= 100) {
        float gy = logEtoY((double)le, (float)CY, (float)CH);
        DrawLine(CX, (int)gy, CX+CW, (int)gy,
                 le == 0 ? GetColor(0x3A3A3AFF) : GetColor(0x232323FF));
        DrawLine(CX-6,  (int)gy, CX,     (int)gy, GetColor(0x555555FF));
        DrawLine(CX+CW, (int)gy, CX+CW+6,(int)gy, GetColor(0x555555FF));

        std::ostringstream ss;
        if (le == 0) ss << "10^0";
        else         ss << "10^" << le;
        DrawText(ss.str().c_str(), CX-76, (int)gy-8, 14, GetColor(0xAAAAAAFF));

        if (le < 0) {
            std::string dl = std::to_string(-le) + " digits";
            DrawText(dl.c_str(), CX+CW+10, (int)gy-8, 13, GetColor(0x888888FF));
        }
    }
    // Minor grid every 50 log units
    for (int le = -50; le >= -950; le -= 100) {
        float gy = logEtoY((double)le, (float)CY, (float)CH);
        if (gy >= CY && gy <= CY+CH)
            DrawLine(CX, (int)gy, CX+CW, (int)gy, GetColor(0x1C1C1CFF));
    }

    // X-axis: CPU time
    const char* x_labels[] = {"1us","10us","100us","1ms","10ms","100ms","1s","10s","100s"};
    for (int lt = (int)LX_MIN; lt <= (int)LX_MAX; lt++) {
        float gx = (float)(CX + (lt - LX_MIN) / (LX_MAX - LX_MIN) * CW);
        DrawLine((int)gx, CY, (int)gx, CY+CH, GetColor(0x232323FF));
        DrawLine((int)gx, CY+CH, (int)gx, CY+CH+6, GetColor(0x555555FF));
        int idx = lt - (int)LX_MIN;
        if (idx >= 0 && idx < 9)
            DrawText(x_labels[idx],
                (int)gx - MeasureText(x_labels[idx], 14)/2, CY+CH+12, 14,
                GetColor(0xAAAAAAFF));
    }

    DrawRectangleLines(CX, CY, CW, CH, GetColor(0x3C3C3CFF));

    // Axis titles
    {
        Font f = GetFontDefault();
        DrawTextPro(f, "Error magnitude",
            {(float)(CX-80), (float)(CY + CH/2 + 60)},
            {0,0}, -90.0f, 15.0f, 1.0f, GetColor(0x777777FF));
        DrawTextPro(f, "Correct digits",
            {(float)(CX+CW+52), (float)(CY + CH/2 + 55)},
            {0,0}, 90.0f, 15.0f, 1.0f, GetColor(0x777777FF));
    }
    DrawText("CPU Time (Seconds)",
        CX + CW/2 - MeasureText("CPU Time (Seconds)", 15)/2,
        CY+CH+34, 15, GetColor(0x888888FF));

    // Chart title + legend
    DrawText("Convergence to True Pi  —  Log-Log Scale", CX, CY-38, 20, WHITE);
    {
        int lx = CX + CW - 8;
        for (int i = 3; i >= 0; i--) {
            Color lc = runners[i].active ? runners[i].color : GetColor(0x444444FF);
            int   tw = MeasureText(runners[i].name, 15);
            lx -= (tw + 28);
            DrawRectangle(lx, CY-17, 18, 4, lc);
            DrawText(runners[i].name, lx+22, CY-22, 15, lc);
        }
    }

    // Double-precision reference line
    float dp_y = logEtoY(-16.0, (float)CY, (float)CH);
    DrawDashedH(CX, (int)dp_y, CX+CW, 8, 5, GetColor(0x555555FF));
    DrawText("<-- double precision floor (10^-16 / ~16 digits)",
             CX+8, (int)dp_y-18, 13, GetColor(0x666666FF));

    // Algorithm curves
    float y_top = (float)CY,    y_bot = (float)(CY+CH);
    float x_lft = (float)CX,    x_rgt = (float)(CX+CW);
    for (int i = 0; i < 4; i++) {
        if (!runners[i].active || (int)runners[i].history.size() < 2) continue;
        for (int j = 0; j < (int)runners[i].history.size()-1; j++) {
            const auto& p0 = runners[i].history[j];
            const auto& p1 = runners[i].history[j+1];
            float x1 = timeToX(p0.t, (float)CX, (float)CW);
            float x2 = timeToX(p1.t, (float)CX, (float)CW);
            float y1 = logErrToY(p0.logErr, (float)CY, (float)CH);
            float y2 = logErrToY(p1.logErr, (float)CY, (float)CH);
            x1 = std::max(x_lft, std::min(x_rgt, x1));
            x2 = std::max(x_lft, std::min(x_rgt, x2));
            y1 = std::max(y_top, std::min(y_bot, y1));
            y2 = std::max(y_top, std::min(y_bot, y2));
            DrawLineEx({x1,y1}, {x2,y2}, 2.5f, runners[i].color);
        }
        const auto& last = runners[i].history.back();
        float lx = timeToX(last.t, (float)CX, (float)CW);
        float ly = logErrToY(last.logErr, (float)CY, (float)CH);
        lx = std::max(x_lft, std::min(x_rgt, lx));
        ly = std::max(y_top, std::min(y_bot, ly));
        DrawCircle((int)lx, (int)ly, 5.0f, runners[i].color);

        if (runners[i].converged) {
            DrawText("CONV", (int)lx+8, (int)ly-20, 13, runners[i].color);
            DrawText(fmt_utils::fmtTime(runners[i].conv_time).c_str(),
                     (int)lx+8, (int)ly-6, 12, runners[i].color);
        } else if (runners[i].cached_ndp > 0) {
            std::string dlab = std::to_string(runners[i].cached_ndp) + " digits";
            DrawText(dlab.c_str(), (int)lx+8, (int)ly-8, 12, runners[i].color);
        }
    }

    // Convergence leaderboard
    {
        int ly = CY + CH + 10;
        DrawText("RESULTS:", CX, ly+4, 13, GetColor(0x555555FF));
        static const char* medals[] = {"1st", "2nd", "3rd", "4th"};
        int rx = CX + 70;
        if (conv_order.empty()) {
            DrawText("-- running --", rx, ly+4, 13, GetColor(0x444444FF));
        }
        for (int r = 0; r < (int)conv_order.size(); r++) {
            int idx = conv_order[r];
            std::string label = std::string(medals[r]) + " "
                              + runners[idx].name + " ("
                              + fmt_utils::fmtTime(runners[idx].conv_time) + ")";
            DrawText(label.c_str(), rx, ly+4, 13, runners[idx].color);
            rx += MeasureText(label.c_str(), 13) + 24;
        }
    }
}

// ── Bottom stats panel ────────────────────────────────────────────────────────

void Renderer::draw_stats_panel(const std::array<AlgoRunner, 4>& runners) const {
    constexpr int CHARS_PER_LINE = 38;
    constexpr int DIGIT_FONT     = 10;
    constexpr int DIGIT_LH       = 14;

    const int panel_y = CY + CH + 52;
    const int col_w   = CW / 4;

    DrawLine(CX, panel_y-2, CX+CW, panel_y-2, GetColor(0x2E2E2EFF));

    for (int i = 0; i < 4; i++) {
        int   bx = CX + col_w * i;
        Color c  = runners[i].active ? runners[i].color : GetColor(0x444444FF);

        DrawText(runners[i].name, bx, panel_y, 16, c);
        DrawText((std::to_string(runners[i].iters) + " steps").c_str(),
                 bx, panel_y + 20, 13, GetColor(0x555555FF));

        if (runners[i].iters > 0 && !runners[i].cached_val_str.empty()) {
            int ndp = runners[i].cached_ndp;
            const std::string& val_str = runners[i].cached_val_str;

            // Digit display: white = correct, orange = wrong onward
            int total_chars = (int)std::min(val_str.size(), true_str_1000_.size());
            int line_y = panel_y + 40;
            for (int start = 0; start < total_chars; start += CHARS_PER_LINE) {
                int len = std::min(CHARS_PER_LINE, total_chars - start);
                DrawPrecisionDigits(val_str.substr(start, len),
                                    true_str_1000_.substr(start, len),
                                    bx, line_y, DIGIT_FONT);
                line_y += DIGIT_LH;
            }

            // Progress bar
            int bar_w  = col_w - 18;
            int filled = std::min(bar_w, bar_w * ndp / 1000);
            Color bar_c = ndp >= 999 ? GetColor(0x22EE66FF)
                        : ndp > 500  ? GetColor(0x88CC44FF)
                        : ndp > 150  ? GetColor(0xDDCC22FF)
                                     : GetColor(0xDD5511FF);
            DrawRectangle(bx, line_y+4, bar_w, 8, GetColor(0x1A1A1AFF));
            DrawRectangle(bx, line_y+4, filled, 8, bar_c);
            std::string plab = std::to_string(ndp) + " / 1000";
            DrawText(plab.c_str(),
                     bx + bar_w/2 - MeasureText(plab.c_str(), 11)/2,
                     line_y+15, 11, GetColor(0x888888FF));

            // Status
            if (runners[i].converged) {
                int sw = MeasureText("= TRUE PI  (1000 digits)", 14) + 14;
                DrawRectangle(bx, line_y+32, sw, 24, GetColor(0x122212FF));
                DrawText("= TRUE PI  (1000 digits)", bx+7, line_y+35, 14,
                         GetColor(0x33EE66FF));
                DrawText(("Converged in " + fmt_utils::fmtTime(runners[i].conv_time)).c_str(),
                         bx, line_y+62, 12, GetColor(0x44AA66FF));
            } else {
                std::string cstr = std::to_string(ndp) + " / 1000 correct digits";
                DrawText(cstr.c_str(), bx, line_y+32, 13, GetColor(0xAAAAAAFF));
                double err = (double)bmp::abs(runners[i].val - TRUE_PI);
                if (err > 0.0)
                    DrawText(("err: " + fmt_utils::fmtSci(runners[i].val - TRUE_PI, 3)).c_str(),
                             bx, line_y+50, 12, c);
                else
                    DrawText(("err < 10^-" + std::to_string(ndp)).c_str(),
                             bx, line_y+50, 12, c);
            }
        } else {
            DrawText("--", bx, panel_y+40, 20, GetColor(0x3A3A3AFF));
            DrawText("not started", bx, panel_y+66, 13, GetColor(0x444444FF));
        }

        if (i < 3)
            DrawLine(bx + col_w - 10, panel_y-2,
                     bx + col_w - 10, H-20, GetColor(0x252525FF));
    }
}
