#pragma once
#include "../simulation/algo_runner.hpp"
#include "../recording/recorder.hpp"
#include "../rendering/renderer.hpp"
#include "../core/layout.hpp"
#include "raylib.h"
#include <array>
#include <string>
#include <vector>

// Owns all application state and drives the main loop.
// Delegates rendering to Renderer and recording to Recorder.
class App {
public:
    App();
    ~App();

    void run();

private:
    void handle_input();
    void advance_step();
    void reset_all();

    std::array<AlgoRunner, 4> runners_;
    std::vector<int>          conv_order_;

    bool  running_  = false;
    float acc_      = 0.f;
    float interval_ = 0.12f;

    mp_float    conv_eps_;
    std::string true_str_1000_;

    Rectangle calc_btn_   = {16, 468, SB - 32.0f, 40};
    Rectangle reset_btn_  = {16, 518, SB - 32.0f, 40};
    Rectangle record_btn_ = {16, 570, SB - 32.0f, 40};

    RenderTexture2D render_target_{};

    Recorder recorder_;
    Renderer renderer_;
};
