# Build steps

1. Clone the repository
2. Install dependencies (git submodule update --init --recursive)
3. Build the project (mkdir build && cd build && cmake -DHIVE_TRACK_PROFILE=FULL .. && cmake --build .)
4. Run the project (cd bind && ./Testbed)
