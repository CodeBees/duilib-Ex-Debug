#pragma once
#ifndef __LAYERED_HPP_2014_08_06
#define __LAYERED_HPP_2014_08_06
#include <sstream>
#include <string>
#include <vector>

namespace DuiLib
{
	class CLayeredWnd : public CWindowWnd, public CControlUI
	{
	public:
		CLayeredWnd()
			: m_bFirstPaint(true)
		{}
		virtual ~CLayeredWnd()
		{}
		
		virtual void	DoInit()
		{
			RECT rcParent = {0};
			::GetWindowRect(CControlUI::GetManager()->GetPaintWindow(), &rcParent);
			Create(CControlUI::GetManager()->GetPaintWindow(), NULL, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW | WS_EX_LAYERED
			 	, 0, 0, CControlUI::m_cxyFixed.cx, CControlUI::m_cxyFixed.cy);
			UpDateLayered();
			CControlUI::SetVisible(true);
			CControlUI::SetMouseEnabled(false);
			_ProofreadWnd();
		}
		
		virtual void	UpDateLayered()
		{
			RECT rcClient = {0};
			::GetClientRect(m_hWnd, &rcClient);
			DWORD dwWidth = rcClient.right - rcClient.left;
			DWORD dwHeight = rcClient.bottom - rcClient.top;

			HDC hDcPaint = ::GetDC(m_hWnd);
			HDC hDcBackground = ::CreateCompatibleDC(hDcPaint);
			COLORREF* pBackgroundBits;
			HBITMAP hbmpBackground = _CreateMyBitmap(hDcPaint, dwWidth, dwHeight, &pBackgroundBits);
			::ZeroMemory(pBackgroundBits, dwWidth * dwHeight * 4);
			HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hDcBackground, hbmpBackground);
			CRenderEngine::DrawImageString(hDcBackground, &m_PaintManager, rcClient, rcClient, GetBkImage(), NULL);
			RECT rcWnd = { 0 };
			::GetWindowRect(m_hWnd, &rcWnd);
			BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
			POINT ptPos   = { rcWnd.left, rcWnd.top };
			SIZE sizeWnd  = { dwWidth, dwHeight };
			POINT ptSrc   = { 0, 0 };
			UpdateLayeredWindow(m_hWnd, hDcPaint, &ptPos, &sizeWnd, hDcBackground, &ptSrc, 0, &bf, ULW_ALPHA);
			::SelectObject(hDcBackground, hOldBitmap);
			if( hDcBackground != NULL ) ::DeleteDC(hDcBackground);
			if( hbmpBackground != NULL ) ::DeleteObject(hbmpBackground);
			::ReleaseDC(m_hWnd, hDcPaint);
			_Hollow();
		}
		
		
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			if (uMsg == WM_CREATE)
			{
				CControlUI* pRoot = new CControlUI;
				m_PaintManager.Init(m_hWnd);
				m_PaintManager.AttachDialog(pRoot);
			}
			else if(uMsg == WM_MOVE)
			{
				_ProofreadWnd();
			}
			else if (uMsg == WM_NCHITTEST)
			{
				POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
				::ScreenToClient(*this, &pt);
				RECT rcClient;
				::GetClientRect(*this, &rcClient);
				RECT rcCaption = m_PaintManager.GetCaptionRect();
				if (	pt.x >= rcClient.left + rcCaption.left 
					&&	pt.x < rcClient.right - rcCaption.right 
					&&	pt.y >= rcCaption.top
					&&	pt.y < rcCaption.bottom ) 
				{
					return HTCAPTION;
				}
				else
					::SetFocus(CControlUI::GetManager()->GetPaintWindow());

				return HTCLIENT;
			}
			return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		}
		
		
		virtual void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
		{
			if( _tcscmp(pstrName, _T("caption")) == 0 ) 
			{
				RECT rcCaption = { 0 };
				LPTSTR pstr = NULL;
				rcCaption.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rcCaption.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
				rcCaption.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
				rcCaption.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
				m_PaintManager.SetCaptionRect(rcCaption);
			}
			else if( _tcscmp(pstrName, _T("HollowRect")) == 0 ) 
			{
				  _ParseHollowRect(pstrValue);
			}
			else  if( _tcscmp(pstrName, _T("pos")) == 0 ) {
				RECT rcPos = { 0 };
				LPTSTR pstr = NULL;
				rcPos.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rcPos.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
				rcPos.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
				rcPos.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
				SIZE szXY = {rcPos.left, rcPos.top};
				SetFixedXY(szXY);
				SetFixedWidth(rcPos.right - rcPos.left);
				SetFixedHeight(rcPos.bottom - rcPos.top);
			}
			else
				CControlUI::SetAttribute(pstrName, pstrValue);
		}
		
		
		virtual void	SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
		{
			CControlUI::m_pManager = pManager;
			CControlUI::m_pParent = pParent;
			if(bInit) CControlUI::Init();
		}
		
		
		virtual void	DoPaint(HDC hDC, const RECT& rcPaint)
		{
			if (m_bFirstPaint)
			{
				RECT rcWnd = {0};
				::GetWindowRect(CControlUI::GetManager()->GetPaintWindow(), &rcWnd);
				::SetWindowPos(GetHWND(), NULL
					, rcWnd.left + CControlUI::m_cXY.cx
					, rcWnd.top + CControlUI::m_cXY.cy
					, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
				m_bFirstPaint = false;
				CWindowWnd::ShowWindow();
				::SetFocus(CControlUI::GetManager()->GetPaintWindow());
			}
		}
		
		virtual LPCTSTR GetClass() const{return _T("Layered");}
		virtual LPCTSTR GetWindowClassName() const{return _T("Layered");}
		
	protected:
		inline void		_ProofreadWnd()
		{
			RECT rcLayered = {0};
			::GetWindowRect(GetHWND(), &rcLayered);
			::SetWindowPos(CControlUI::GetManager()->GetPaintWindow(), NULL
				, rcLayered.left - CControlUI::m_cXY.cx
				, rcLayered.top - CControlUI::m_cXY.cy
				, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
		}
		inline void		_Hollow()
		{
			RECT rcLayered = {0};
			::GetWindowRect(GetHWND(), &rcLayered);
 			HRGN hLayered;
 			hLayered = ::CreateRectRgn(rcLayered.left, rcLayered.top, rcLayered.right, rcLayered.bottom);
 			for (auto iter = m_vecHollowRect.begin(); iter != m_vecHollowRect.end(); ++iter)
 			{
				HRGN hUnLayered;
				hUnLayered = ::CreateRectRgn((*iter).left, (*iter).top, (*iter).right, (*iter).bottom);
				::CombineRgn(hLayered, hLayered, hUnLayered, RGN_XOR);
 			}
			::SetWindowRgn(GetHWND(), hLayered, TRUE); 
		}
		
		inline void		_ParseHollowRect(LPCTSTR _lpStr)
		{
#ifdef UNICODE
			std::wstringstream ss;
#else
			std::stringstream ss;
#endif
			ss << _lpStr;
			while (!ss.eof())
			{
#ifdef UNICODE
				std::wstring str;
#else
				std::string str;
#endif
				ss >> str;
				LPCTSTR lpRect = str.c_str() + 1;
				LPTSTR pstr = NULL;
				RECT rect = {0};
				rect.left = _tcstol(lpRect, &pstr, 10);  ASSERT(pstr);    
				rect.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
				rect.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
				rect.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);  
				m_vecHollowRect.push_back(rect);
			}
			
		}
		inline HBITMAP	_CreateMyBitmap(HDC hDC, int cx, int cy, COLORREF** pBits)
		{
			LPBITMAPINFO lpbiSrc = NULL;
			lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
			if (lpbiSrc == NULL) return NULL;
			lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			lpbiSrc->bmiHeader.biWidth = cx;
			lpbiSrc->bmiHeader.biHeight = cy;
			lpbiSrc->bmiHeader.biPlanes = 1;
			lpbiSrc->bmiHeader.biBitCount = 32;
			lpbiSrc->bmiHeader.biCompression = BI_RGB;
			lpbiSrc->bmiHeader.biSizeImage = cx * cy;
			lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
			lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
			lpbiSrc->bmiHeader.biClrUsed = 0;
			lpbiSrc->bmiHeader.biClrImportant = 0;
			HBITMAP hBitmap = CreateDIBSection (hDC, lpbiSrc, DIB_RGB_COLORS, (void **)pBits, NULL, NULL);
			delete [] lpbiSrc;
			return hBitmap;
		}
	protected:
		CPaintManagerUI		m_PaintManager;
		std::vector<RECT>	m_vecHollowRect;
		bool				m_bFirstPaint;
	};
}

#endif