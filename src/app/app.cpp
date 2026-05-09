#include "app.hpp"
#include "../utils/format_utils.hpp"
#include "../rendering/draw_utils.hpp"
#include "../algorithms/gauss_legendre.hpp"
#include "../algorithms/machin.hpp"
#include "../algorithms/ramanujan.hpp"
#include "../algorithms/chudnovsky.hpp"
#include <algorithm>

using namespace draw_utils;

// ── Construction / destruction ────────────────────────────────────────────────

App::App()
    : conv_eps_("1e-999"),
      true_str_1000_(fmt_utils::fmt(TRUE_PI, 1000)),
      renderer_(fmt_utils::fmt(TRUE_PI, 1000)) {
    InitWindow(W, H, "The Race to Pi");
    SetTargetFPS(60);
    render_target_ = LoadRenderTexture(W, H);

    // Configure each runner slot
    struct Meta { const char* name; const char* desc; Color color; int max_iters; };
    static const Meta metas[4] = {
        {"Gauss-Legendre", "Quadratic — doubles digits each step",  SKYBLUE, 20 },
        {"Machin arctan",  "Linear series — ~1.4 digits per step",  ORANGE,  750},
        {"Ramanujan",      "~8 digits per step (now incremental)",  RED,     135},
        {"Chudnovsky",     "~14 digits per step (fastest series)",  VIOLET,  80 },
    };

    runners_[0].algo = std::make_unique<GaussLegendre>();
    runners_[1].algo = std::make_unique<Machin>();
    runners_[2].algo = std::make_unique<Ramanujan>();
    runners_[3].algo = std::make_unique<Chudnovsky>();

    for (int i = 0; i < 4; i++) {
        runners_[i].name      = metas[i].name;
        runners_[i].desc      = metas[i].desc;
        runners_[i].color     = metas[i].color;
        runners_[i].max_iters = metas[i].max_iters;
    }

    reset_all();
}

App::~App() {
    recorder_.stop();
    UnloadRenderTexture(render_target_);
    CloseWindow();
}

// ── Main loop ─────────────────────────────────────────────────────────────────

void App::run() {
    while (!WindowShouldClose()) {
        handle_input();

        if (running_) {
            acc_ += GetFrameTime();
            if (acc_ >= interval_) {
                acc_ = 0;
                advance_step();
            }
        }

        // Draw into offscreen texture so we can reliably read pixels back.
        BeginTextureMode(render_target_);
        ClearBackground(GetColor(0x0E0E0EFF));
        renderer_.draw(runners_, conv_order_, running_,
                       recorder_.is_recording(), recorder_.frame_count(),
                       interval_, calc_btn_, reset_btn_, record_btn_);
        EndTextureMode();

        // Capture before presenting — texture pixel reads always succeed.
        recorder_.capture_frame(render_target_);

        // Blit to screen. Negative src height flips Y (OpenGL textures are bottom-up).
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(render_target_.texture,
            {0.f, 0.f, (float)W, -(float)H},
            {0.f, 0.f, (float)GetScreenWidth(), (float)GetScreenHeight()},
            {0.f, 0.f}, 0.f, WHITE);
        EndDrawing();
    }
}

// ── Input ─────────────────────────────────────────────────────────────────────

void App::handle_input() {
    if (IsKeyPressed(KEY_SPACE) || Clicked(calc_btn_))  running_ = !running_;
    if (IsKeyPressed(KEY_R)     || Clicked(reset_btn_)) reset_all();

    if (Clicked(record_btn_)) {
        if (!recorder_.is_recording())
            recorder_.start(W, H);
        else
            recorder_.stop();
    }

    if (IsKeyPressed(KEY_ONE))   runners_[0].active ^= 1;
    if (IsKeyPressed(KEY_TWO))   runners_[1].active ^= 1;
    if (IsKeyPressed(KEY_THREE)) runners_[2].active ^= 1;
    if (IsKeyPressed(KEY_FOUR))  runners_[3].active ^= 1;

    if (IsKeyPressed(KEY_UP))   interval_ = std::max(0.03f, interval_ - 0.03f);
    if (IsKeyPressed(KEY_DOWN)) interval_ = std::min(1.00f, interval_ + 0.05f);

    // Toggle switches rendered in the sidebar
    for (int i = 0; i < 4; i++) {
        Rectangle tr = {(float)(SB-60), (float)(82 + i*58 + 34), 40, 22};
        if (Clicked(tr)) runners_[i].active ^= 1;
    }
}

// ── Simulation step ───────────────────────────────────────────────────────────

void App::advance_step() {
    for (int i = 0; i < 4; i++)
        runners_[i].advance(conv_eps_, conv_order_, i);
}

// ── Reset ─────────────────────────────────────────────────────────────────────

void App::reset_all() {
    running_ = false;
    conv_order_.clear();
    for (auto& r : runners_)
        r.reset();
}
