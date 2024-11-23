#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include "include/imgui/imgui.h"
#include "include/imgui/backends/imgui_impl_opengl3.h"
#include "include/imgui/backends/imgui_impl_glfw.h"
#ifdef __EMSCRIPTEN__
#include "include/imgui/emscripten_mainloop_stub.h"
#endif

GLFWwindow* window;
int numberToCheck = 0;
bool flag;
bool resultMalloc = false;
bool reasonMalloc = false;
bool notFirst = false;
char* reason;
char* result = "Results show up here";

void mainLoop() {
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Prime Number Checker", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowSize(ImVec2((float)400, (float)125));
    ImGui::SetWindowPos(ImVec2(0, 0));
    ImGui::InputInt("Number to check", &numberToCheck);
    if(ImGui::Button("Check")) {
        if(notFirst) {
            if(reasonMalloc) {
                free(reason);
                reasonMalloc = false;
            }
            reason = "";
            if(resultMalloc) {
                free(result);
                resultMalloc = false;
            }
            result = "";
        }

        flag = false;

        for(int i=2; i<numberToCheck; i++) {
            if(numberToCheck % i == 0) {
                reasonMalloc = true;
                flag = true;
                std::string reasonCPP = std::to_string(numberToCheck/i) + " * " + std::to_string(i) + " = " + std::to_string(numberToCheck);
                reason = (char*)malloc(reasonCPP.length());
                std::strcpy(reason, reasonCPP.c_str());
                break;
            }
        }

        if(flag) {
            resultMalloc = true;
            std::string reasonCPP;
            reasonCPP.assign(reason, reason+strlen(reason));
            std::string resultCPP = "Number is composite because " + reasonCPP;
            result = (char*)malloc(resultCPP.length());
            std::strcpy(result, resultCPP.c_str());
        } else {
            resultMalloc = false;
            result = "Number is prime";
        }

        notFirst = true;
    }
    ImGui::Text(result);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main() {
#ifdef __linux__
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
    if(!glfwInit()) return 1;

#ifdef IMGUI_IMPL_OPENGL_ES2
  const char *glslVersion = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif __APPLE__
  const char *glslVersion = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  const char *glslVersion = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    window = glfwCreateWindow(400, 125, "Prime Number Checker", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glClearColor((GLclampf)34/255, (GLclampf)35/255, (GLclampf)40/255, 1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("Open_Sans/OpenSans-Variable.ttf", 20);
    io.Fonts->Build();
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glslVersion);

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while(!glfwWindowShouldClose(window))
#endif
    {
        mainLoop();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    if(reasonMalloc) free(reason);
    if(resultMalloc) free(result);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    
    return 0;
}

