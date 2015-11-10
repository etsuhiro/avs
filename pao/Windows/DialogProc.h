#pragma once

#include "windows.h"

namespace pao
{
	class IDialogProc {
		HWND m_hwnd;

	public:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:
		virtual LRESULT OnNCCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnLButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnRButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual void OnDestroy(HWND hWnd);
		virtual LRESULT OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
		virtual void OnSize(HWND hWnd, UINT flag, int width, int height);
	};
}
