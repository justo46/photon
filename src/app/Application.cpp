#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "Application.h"
#include "Rendering.h"

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

struct App {
	App(int w, int h) : window_width(w), window_height(h) {}

	SDL_Window*	window = nullptr;
	SDL_GLContext gl_context = nullptr;
	const char*	glsl_version = "#version 130";
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	int	window_width;
	int	window_height;
	std::vector<uint8_t> framebuffer;
};

// --------------------------------------------------------------------------------------
// GL profile + GLSL selection (desktop, macOS, GLES2/3)
// --------------------------------------------------------------------------------------
static void setup_gl_attributes_and_version(App& a) {
#if defined(__APPLE__)
	a.glsl_version = "#version 150"; // GL 3.2 Core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	a.glsl_version = "#version 130"; // GL 3.0+
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
}

// --------------------------------------------------------------------------------------
// Create window + GL context
// --------------------------------------------------------------------------------------
static bool create_window_and_context(App& a, const char* title, int base_w = 1280, int base_h = 720) {
	float dpi_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	int   win_w = int(base_w * dpi_scale);
	int   win_h = int(base_h * dpi_scale);

	const SDL_WindowFlags flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;

	a.window = SDL_CreateWindow(title, win_w, win_h, flags);
	if (!a.window) { 
		printf("SDL_CreateWindow: %s\n", SDL_GetError()); 
		return false; 
	}

	a.gl_context = SDL_GL_CreateContext(a.window);
	if (!a.gl_context) { 
		printf("SDL_GL_CreateContext: %s\n", SDL_GetError()); 
		return false; 
	}

	SDL_GL_MakeCurrent(a.window, a.gl_context);
	SDL_GL_SetSwapInterval(1); // vsync
	SDL_SetWindowPosition(a.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(a.window);

	return true;
}

// --------------------------------------------------------------------------------------
// ImGui init (style, DPI, docking/viewports, backends)
// --------------------------------------------------------------------------------------
static void init_imgui(App& a) {
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
	float dpi_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    style.ScaleAllSizes(dpi_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = dpi_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
    io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(a.window, a.gl_context);
    ImGui_ImplOpenGL3_Init(a.glsl_version);
}

// --------------------------------------------------------------------------------------
// ImGui shutdown + SDL/GL cleanup
// --------------------------------------------------------------------------------------
static void quit_all(App& a) {
	// Cleanup
	// [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppQuit() function]
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DestroyContext(a.gl_context);
	SDL_DestroyWindow(a.window);
	SDL_Quit();

}

static void draw_ui(App& a) {
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin("Raytraced Output");
	if (gTex) {
		ImGui::Image((ImTextureID)(intptr_t)gTex, ImVec2((float)a.window_width, (float)a.window_height));
		// If your buffer is bottom-left origin, flip UVs:
		// ImGui::Image((ImTextureID)(intptr_t)gTex, ImVec2((float)gW,(float)gH), ImVec2(0,1), ImVec2(1,0));
	}
	ImGui::End();

	ImGui::Begin("Properties");
	ImGui::Text("Resolution: %d x %d | %.1f FPS", a.window_width, a.window_height, io.Framerate);
	ImGui::End();

}

// --------------------------------------------------------------------------------------
// Main loop (events → imgui frame → render → present)
// --------------------------------------------------------------------------------------
static void run_loop(App& a)
{
	bool done = false;
	ImGuiIO& io = ImGui::GetIO();

	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(a.window))
				done = true;
		}

		if (SDL_GetWindowFlags(a.window) & SDL_WINDOW_MINIMIZED)
		{
			SDL_Delay(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		// Draw your UI
		draw_ui(a);

		// Rendering
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(a.clear_color.x * a.clear_color.w, 
					 a.clear_color.y * a.clear_color.w, 
					 a.clear_color.z * a.clear_color.w, 
					 a.clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		SDL_GL_SwapWindow(a.window);
	}
}

bool runApplication() {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
	{
		printf("SDL_Init: %s\n", SDL_GetError());
		return false;
	}
	
	App app(1280, 720);
	setup_gl_attributes_and_version(app);
	if (!create_window_and_context(app, "photon"))
		return false;

	init_imgui(app);

	InitRaytracedImageOnce(app.window_width, app.window_height);

	run_loop(app);
	quit_all(app);
	return true;
}
