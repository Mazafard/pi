#pragma once
#include "../simulation/algo_runner.hpp"
#include "raylib.h"
#include <array>
#include <string>
#include <vector>

// Stateless drawing layer. Reads app state and issues raylib draw calls.
// Single Responsibility: rendering only — no input, no simulation logic.
class Renderer {
public:
    explicit Renderer(std::string true_str_1000);

    void draw(const std::array<AlgoRunner, 4>& runners,
              const std::vector<int>&           conv_order,
              bool      running,
              bool      recording,
              int       rec_frames,
              float     interval,
              Rectangle calc_btn,
              Rectangle reset_btn,
              Rectangle record_btn) const;

private:
    std::string true_str_1000_;

    void draw_title_bar(bool recording, int rec_frames) const;

    void draw_sidebar(const std::array<AlgoRunner, 4>& runners,
                      bool running, bool recording, int rec_frames, float interval,
                      Rectangle calc_btn, Rectangle reset_btn, Rectangle record_btn) const;

    void draw_chart(const std::array<AlgoRunner, 4>& runners,
                    const std::vector<int>&           conv_order) const;

    void draw_stats_panel(const std::array<AlgoRunner, 4>& runners) const;
};
