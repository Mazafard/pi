#pragma once
#include "raylib.h"
#include <cstdio>
#include <string>

// Manages an ffmpeg pipe for recording screen frames to an mp4 file.
// Reads pixels from a RenderTexture2D — reliable on all platforms including
// macOS where LoadImageFromScreen returns black from the Metal-backed framebuffer.
class Recorder {
public:
    Recorder()  = default;
    ~Recorder() { stop(); }

    Recorder(const Recorder&)            = delete;
    Recorder& operator=(const Recorder&) = delete;

    bool        start(int width, int height);
    void        stop();
    void        capture_frame(const RenderTexture2D& target);

    bool        is_recording() const { return recording_; }
    int         frame_count()  const { return frame_count_; }
    std::string filename()     const { return filename_; }

private:
    FILE*       pipe_        = nullptr;
    bool        recording_   = false;
    int         frame_count_ = 0;
    std::string filename_;
};
