#pragma once
#include "stdafx.h"

using vector_getter_t = bool(*)(void* data, int idx, const char** out_text);
using str_list_t = std::vector<std::string>;
using pt_list_t = std::vector<vector2d_t>;

namespace gui
{
	void initialise(const std::wstring& interface_name);
	
	namespace detail
	{

		extern LPDIRECT3DDEVICE9 g_device;
		extern D3DPRESENT_PARAMETERS g_params;
		extern WNDCLASSEX g_window;
		extern LPDIRECT3D9 g_direct3d;

		void message_loop();
		bool create_window(const std::wstring& interface_name);
		bool create_device(const std::wstring& interface_name, HWND window_handle);
		LRESULT WINAPI window_procedure(HWND window_handle, UINT msg, WPARAM wparam, LPARAM lparam);

		void draw_points(pt_list_t &point_list, float &x_input, float &y_input, str_list_t &text_point_list, vector_getter_t vector_getter);
		void draw_side_lengths(polyd_t &polygon, vector_getter_t vector_getter);
		void draw_mid_points(polyd_t &polygon, vector_getter_t vector_getter);
		void draw_angles(polyd_t &polygon, vector_getter_t vector_getter);
		void draw_medians(polyd_t &polygon, vector_getter_t vector_getter);

	}
};