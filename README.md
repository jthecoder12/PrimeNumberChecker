# Prime Number Checker
A prime number checker written in C++.
[Web Build](https://jthecoder12.github.io/PrimeNumberChecker)
To compile:
For desktop: <code>g++ main.cpp include/imgui/\*.cpp include/imgui/backends/\*.cpp -Iinclude -lglfw -lGL</code>
You must have <code>libglfw-dev</code> and <code>libopengl-dev</code> installed.
For web (requires emscripten): <code>em++ main.cpp include/imgui/\*.cpp include/imgui/backends/\*.cpp -Iinclude -s USE_GLFW=3 -s FULL_ES3 -o index.html</code>

**Credits:**
[ImGui](https://github.com/ocornut/imgui)
[ImGui OpenGL3 GLFW example](https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp)
[Emscripten main loop stub (emscripten_mainloop_stub.h)](https://github.com/ocornut/imgui/blob/master/examples/libs/emscripten/emscripten_mainloop_stub.h)
