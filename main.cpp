#include <GLFW/glfw3.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int width = 1280, height = 128;
GLFWwindow* window;

unsigned long long numberToCheck;
std::string resultText = "Press check for result";

void mainLoop() {
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::InputScalar("Number to check", ImGuiDataType_U64, &numberToCheck);
	if(ImGui::Button("Check"))
		if(numberToCheck >= 2)
			for(unsigned long long i = 2; i <= numberToCheck / 2; i++) {
				std::string numberAsString = std::to_string(numberToCheck);

				if(numberToCheck % i == 0) {
					resultText = numberAsString + " is composite because " + std::to_string(numberToCheck / i) + " * " + std::to_string(i) + " = " + numberAsString;
					break;
				}

				resultText = numberAsString + " is prime";
			}
		else resultText = "Numbers cannot be less than 2";
	ImGui::Text(resultText.c_str());
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main() {
	if(!glfwInit()) return -1;

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, "Prime Number Checker", nullptr, nullptr);
	if(!window) {
		glfwTerminate();
		return -1;
	}

	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, (vidMode->width - width) / 2, (vidMode->height - height) / 2);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	io.Fonts->AddFontFromFileTTF("OpenSans.ttf", 24);

	io.IniFilename = nullptr;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
	ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
	ImGui_ImplOpenGL3_Init("#version 300 es");

	glClearColor(float(18)/255, float(19)/255, float(15)/255, 1);

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(mainLoop, 0, 1);
#else
	while(!glfwWindowShouldClose(window)) mainLoop();
#endif

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

