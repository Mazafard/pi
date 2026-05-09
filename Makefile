CXX = g++

BOOST_INC     = /opt/homebrew/include
RAYLIB_PREFIX = /opt/homebrew/opt/raylib

CXXFLAGS = -std=c++17 -O3 -I$(BOOST_INC) -I$(RAYLIB_PREFIX)/include
LDFLAGS  = -L$(RAYLIB_PREFIX)/lib -lraylib \
           -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# ── visualize sources ─────────────────────────────────────────────────────────
VIZ_SRCS = visualize.cpp \
           src/simulation/algo_runner.cpp \
           src/recording/recorder.cpp \
           src/rendering/renderer.cpp \
           src/app/app.cpp

VIZ_OBJS = $(VIZ_SRCS:.cpp=.o)

# ── targets ───────────────────────────────────────────────────────────────────
.PHONY: all run run-viz clean

all: pi_calc visualize

pi_calc: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o pi_calc

visualize: $(VIZ_OBJS)
	$(CXX) $(CXXFLAGS) $(VIZ_OBJS) -o visualize $(LDFLAGS)

run: pi_calc
	./pi_calc

run-viz: visualize
	./visualize

# ── generic compile rule ──────────────────────────────────────────────────────
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ── header dependencies (keeps incremental builds correct) ───────────────────
src/simulation/algo_runner.o: src/simulation/algo_runner.cpp \
    src/simulation/algo_runner.hpp src/core/pi_types.hpp \
    src/utils/format_utils.hpp src/algorithms/algorithm.hpp

src/recording/recorder.o: src/recording/recorder.cpp \
    src/recording/recorder.hpp

src/rendering/renderer.o: src/rendering/renderer.cpp \
    src/rendering/renderer.hpp src/core/layout.hpp \
    src/utils/format_utils.hpp src/rendering/draw_utils.hpp \
    src/simulation/algo_runner.hpp

src/app/app.o: src/app/app.cpp \
    src/app/app.hpp src/utils/format_utils.hpp src/rendering/draw_utils.hpp \
    src/core/layout.hpp \
    src/algorithms/gauss_legendre.hpp src/algorithms/machin.hpp \
    src/algorithms/ramanujan.hpp src/algorithms/chudnovsky.hpp

visualize.o: visualize.cpp src/app/app.hpp

clean:
	rm -f pi_calc visualize $(VIZ_OBJS)
