
/*
	该示例为网友 crax：qq 771545803 放出的代码
*/



#include "..\..\DuiLib\UIlib.h"
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\Lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\..\\Lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\Lib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\..\\Lib\\DuiLib.lib")
#   endif
#endif

#include "Layered.hpp"

class CMainFram : public DuiLib::WindowImplBase
{
public:
	virtual void Notify(DuiLib::TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (msg.pSender->GetName() == _T("ok"))
				::MessageBox(GetHWND(), _T("登陆"), _T("登陆"), MB_OK);
			else if (msg.pSender->GetName() == _T("cancel"))
			{
				Close();
				//::PostQuitMessage(0);
			}
				
		}
	}
	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcscmp(pstrClass, _T("Layered")) == 0)
		{
			return new DuiLib::CLayeredWnd;
		}
		return nullptr;
	}
	virtual DuiLib::CDuiString GetSkinFolder(){return _T("TestLayeredWndSkin");}
	virtual DuiLib::CDuiString GetSkinFile(){return _T("Login.xml");}
	virtual LPCTSTR GetWindowClassName(void) const{return _T("MainFram");}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	CMainFram fram;
	fram.Create(NULL, NULL, UI_WNDSTYLE_DIALOG, NULL);
	fram.CenterWindow();
	fram.ShowModal();
	return 0;
}