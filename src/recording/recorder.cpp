#include "recorder.hpp"
#include "raylib.h"
#include <ctime>
#include <cstring>
#include <cstdio>

bool Recorder::start(int render_width, int render_height) {
    time_t now = std::time(nullptr);
    char buf[128];
    std::strftime(buf, sizeof(buf), "pi_race_%Y%m%d_%H%M%S.mp4", std::localtime(&now));
    filename_ = buf;

    char cmd[512];
    std::snprintf(cmd, sizeof(cmd),
        "ffmpeg -y -f rawvideo -pixel_format rgb24 "
        "-video_size %dx%d -framerate 60 -i pipe:0 "
        "-vcodec libx264 -preset fast -crf 18 -pix_fmt yuv420p "
        "\"%s\" 2>/dev/null",
        render_width, render_height, filename_.c_str());

    pipe_        = popen(cmd, "w");
    recording_   = (pipe_ != nullptr);
    frame_count_ = 0;
    return recording_;
}

void Recorder::stop() {
    if (pipe_) { pclose(pipe_); pipe_ = nullptr; }
    recording_ = false;
}

void Recorder::capture_frame(const RenderTexture2D& target) {
    if (!recording_ || !pipe_) return;
    // LoadImageFromTexture reads from GPU memory we own — always correct pixels.
    // Textures are stored bottom-up in OpenGL, so flip before sending to ffmpeg.
    Image frame = LoadImageFromTexture(target.texture);
    ImageFlipVertical(&frame);
    ImageFormat(&frame, PIXELFORMAT_UNCOMPRESSED_R8G8B8);
    fwrite(frame.data, 1, (size_t)frame.width * frame.height * 3, pipe_);
    UnloadImage(frame);
    frame_count_++;
}
