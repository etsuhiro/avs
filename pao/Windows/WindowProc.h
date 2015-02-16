#pragma once

#include "windows.h"

namespace pao
{
	class IWindowProc {
	public:
		LRESULT operator()(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		virtual LRESULT WmCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WmPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WmLButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WmMButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WmRButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WmMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WmDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WmClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
	};
}