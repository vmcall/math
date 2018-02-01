#include "stdafx.h"

LPDIRECT3DDEVICE9 gui::detail::g_device = nullptr;
LPDIRECT3D9 gui::detail::g_direct3d = nullptr;
D3DPRESENT_PARAMETERS gui::detail::g_params = {};
WNDCLASSEX gui::detail::g_window = {};

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI gui::detail::window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplDX9_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (gui::detail::g_device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			gui::detail::g_params.BackBufferWidth = LOWORD(lParam);
			gui::detail::g_params.BackBufferHeight = HIWORD(lParam);

			if (gui::detail::g_device->Reset(&gui::detail::g_params) == D3DERR_INVALIDCALL)
				IM_ASSERT(0);

			ImGui_ImplDX9_CreateDeviceObjects();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void gui::initialise(const std::wstring& interface_name)
{
	if (!gui::detail::create_window(interface_name))
	{
		std::cout << "Failed to create window" << std::endl;
		return;
	}

#pragma region STYLE
	auto& style = ImGui::GetStyle();

	style.ChildWindowRounding = 0.f;
	style.GrabRounding = 0.f;
	style.WindowRounding = 0.f;
	style.ScrollbarRounding = 0.f;
	style.FrameRounding = 0.f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);

	auto& io = ImGui::GetIO();

	io.Fonts->AddFontDefault();
#pragma endregion

	gui::detail::message_loop();

	ImGui_ImplDX9_Shutdown();

	if (gui::detail::g_device)
		gui::detail::g_device->Release();

	if (gui::detail::g_direct3d)
		gui::detail::g_direct3d->Release();

	UnregisterClass(interface_name.c_str(), gui::detail::g_window.hInstance);
}

void gui::detail::draw_points(pt_list_t& point_list, float& x_input, float& y_input, str_list_t& text_point_list, vector_getter_t vector_getter)
{
	// INPUT POINT
	if (ImGui::Button("Add Point"))
	{
		point_list.emplace_back(vector2d_t(static_cast<double>(x_input), static_cast<double>(y_input)));
		text_point_list.emplace_back(fmt::format("{0:.1f},{1:.1f}", x_input, y_input));
	}

	// X/Y INPUTS
	ImGui::PushItemWidth(100);
	ImGui::InputFloat("X", &x_input, 0.5f, 1.f, 1);
	ImGui::SameLine();
	ImGui::InputFloat("Y", &y_input, 0.5f, 1.f, 1);
	ImGui::PopItemWidth();

	// LIST POINTS
	ImGui::PushItemWidth(150);
	if (point_list.size() > 0)
	{
		static int current_item = -1;
		ImGui::ListBox("Points", &current_item, vector_getter, static_cast<void*>(&text_point_list), text_point_list.size());
	}
}

void gui::detail::draw_side_lengths(polyd_t &polygon, vector_getter_t vector_getter)
{
	str_list_t vec;
	auto sides = polygon.sides();
	std::for_each(sides.begin(), sides.end(), [&vec](auto side) { vec.emplace_back(fmt::format("{0:.1f}", side.length())); });

	static int current_item = -1;
	ImGui::ListBox("Sides", &current_item, vector_getter, static_cast<void*>(&vec), vec.size());
}

void gui::detail::draw_mid_points(polyd_t &polygon, vector_getter_t vector_getter)
{
	str_list_t vec;

	auto midpoints = polygon.midpoints();
	std::for_each(midpoints.begin(), midpoints.end(), [&vec](auto midpoint) { vec.emplace_back(fmt::format("{0:.1f},{1:.1f}", midpoint.x(), midpoint.y())); });

	static int current_item = -1;
	ImGui::ListBox("Midpoints", &current_item, vector_getter, static_cast<void*>(&vec), vec.size());
}

void gui::detail::draw_angles(polyd_t &polygon, vector_getter_t vector_getter)
{
	str_list_t vec;
	auto angles = polygon.angles();
	std::for_each(angles.begin(), angles.end(), [&vec](auto angle) { vec.emplace_back(fmt::format("{0:.1f}", angle)); });

	static int current_item = -1;
	ImGui::ListBox("Angles", &current_item, vector_getter, static_cast<void*>(&vec), vec.size());
}

void gui::detail::draw_medians(polyd_t &polygon, vector_getter_t vector_getter)
{
	str_list_t vec;
	auto medians = polygon.triangle_medians();
	std::for_each(medians.begin(), medians.end(), [&vec](auto median) { vec.emplace_back(fmt::format("{0:.1f}", median)); });

	static int current_item = -1;
	ImGui::ListBox("Medians", &current_item, vector_getter, static_cast<void*>(&vec), vec.size());
}

void gui::detail::message_loop()
{
	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		
		ImGui_ImplDX9_NewFrame();

		// LISTBOX HELPER
		auto vector_getter = [](void* vec, int idx, const char** out_text)
		{
			auto& vector = *static_cast<str_list_t*>(vec);
			if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
			*out_text = vector.at(idx).c_str();
			return true;
		};

		static pt_list_t point_list;
		static str_list_t text_point_list;

		// DRAW DATA WINDOW
		if (ImGui::Begin("CAS", nullptr, ImVec2(400, 300), 1.f, ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::SetWindowPos(ImVec2(700, 300), ImGuiSetCond_FirstUseEver);
		
			static float x_input, y_input;
			gui::detail::draw_points(point_list, x_input, y_input, text_point_list, vector_getter);

			polyd_t polygon{ point_list };

			// LIST MATHEMATICAL DATA
			if (point_list.size() > 2)
			{

				gui::detail::draw_side_lengths(polygon, vector_getter);
				gui::detail::draw_mid_points(polygon, vector_getter);
				gui::detail::draw_angles(polygon, vector_getter);
			}

			// TRIANGLE SPECIFIC DATA
			if (point_list.size() == 3)
			{
				gui::detail::draw_medians(polygon, vector_getter);
			}
			
			ImGui::PopItemWidth();
			ImGui::End();
		}

		// DRAW GEOMETRY WINDOW WHEN > 1 POINTS ADDED
		if (point_list.size() > 1 && ImGui::Begin("GEOMETRY", nullptr, ImVec2(500, 500), 1.f, ImGuiWindowFlags_NoSavedSettings))
		{
			static auto scale_coefficient = 10.f;
			ImGui::SliderFloat("Scaling coefficient", &scale_coefficient, 1, 100, "%.0f");

			ImGui::SetWindowPos(ImVec2(200, 200), ImGuiSetCond_FirstUseEver);

			const auto window_size = ImGui::GetWindowSize();
			const auto window_position = ImGui::GetWindowPos();

			for (size_t i = 0; i < point_list.size(); i++)
			{
				auto current_point_vec = point_list.at(i);
				auto next_point_vec = point_list[(i + 1) % point_list.size()];

				auto current_point = ImVec2(current_point_vec.x(), current_point_vec.y());
				auto next_point = ImVec2(next_point_vec.x(), next_point_vec.y());


				// SCALE AND CONVERT VECTOR (INVERT Z)
				current_point.x = current_point.x * scale_coefficient + window_position.x + window_size.x / 2;
				current_point.y = -current_point.y * scale_coefficient + window_position.y + window_size.y / 2;
				next_point.x = next_point.x * scale_coefficient + window_position.x + window_size.x / 2;
				next_point.y = -next_point.y * scale_coefficient + window_position.y + window_size.y / 2;

				ImGui::GetWindowDrawList()->AddLine(current_point, next_point, ImColor(255, 0, 0));
			}

			ImGui::End();
		}

		gui::detail::g_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(50, 200, 255, 255), 0, 0);

		if (gui::detail::g_device->BeginScene() >= 0)
		{
			ImGui::Render();
			gui::detail::g_device->EndScene();
		}

		gui::detail::g_device->Present(NULL, NULL, NULL, NULL);
		Sleep(1);
	}
}

bool gui::detail::create_window(const std::wstring& interface_name)
{
	gui::detail::g_window = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		gui::detail::window_procedure,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		NULL,
		NULL,
		interface_name.c_str(),
		NULL
	};

	//graphical_interface::detail::g_window.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));

	RegisterClassEx(&gui::detail::g_window);
	HWND window_handle = CreateWindow(interface_name.c_str(), interface_name.c_str(), WS_POPUP | WS_EX_TOPMOST | WS_EX_LAYERED, 0, 0, 1920, 1080, NULL, NULL, gui::detail::g_window.hInstance, NULL);

	if (!gui::detail::create_device(interface_name, window_handle))
	{
		std::cout << "Failed to create device" << std::endl;
		return false;
	}

	ImGui_ImplDX9_Init(window_handle, gui::detail::g_device);
	
	//SetLayeredWindowAttributes(window_handle, RGB(50, 200, 255), 0, LWA_COLORKEY);
	//SetLayeredWindowAttributes(window_handle, RGB(0, 0, 0), 255, LWA_ALPHA);

	ShowWindow(window_handle, SW_SHOW);
	UpdateWindow(window_handle);

	//MARGINS margin{ -1 };
	//DwmExtendFrameIntoClientArea(window_handle, &margin);
	
	return true;
}

bool gui::detail::create_device(const std::wstring& interface_name, HWND window_handle)
{
	gui::detail::g_direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (gui::detail::g_direct3d == 0)
	{
		UnregisterClass(interface_name.c_str(), gui::detail::g_window.hInstance);
		return false;
	}
	ZeroMemory(&gui::detail::g_params, sizeof(gui::detail::g_params));
	gui::detail::g_params.Windowed = TRUE;
	gui::detail::g_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	gui::detail::g_params.BackBufferFormat = D3DFMT_UNKNOWN;
	gui::detail::g_params.EnableAutoDepthStencil = TRUE;
	gui::detail::g_params.AutoDepthStencilFormat = D3DFMT_D16;
	gui::detail::g_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (gui::detail::g_direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_handle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &gui::detail::g_params, &gui::detail::g_device) < 0)
	{
		gui::detail::g_direct3d->Release();
		UnregisterClass(interface_name.c_str(), gui::detail::g_window.hInstance);
		return false;
	}

	return true;
}