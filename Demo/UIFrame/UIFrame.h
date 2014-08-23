#pragma once

#include "resource.h"

class CDYFrameWnd : public WindowImplBase
{
public:
	explicit CDYFrameWnd(LPCTSTR pszXMLPath):strXMLPath_(pszXMLPath){};
	explicit CDYFrameWnd(){};
	
	virtual ~CDYFrameWnd();


	virtual LPCTSTR GetWindowClassName() const { return _T("Window"); }
	virtual CDuiString GetSkinFile()                { return _T("Window.xml"); }
	virtual CDuiString GetSkinFolder()              { return _T("UIFrameSkin"); }
	virtual void  InitWindow();
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

protected:
	CDuiString strXMLPath_;

};


// 以XML生成界面的窗口基类
class CXMLWnd : public WindowImplBase
{
public:
	explicit CXMLWnd(LPCTSTR pszXMLPath) 
		: m_strXMLPath(pszXMLPath){}

public:
	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("XMLWnd");
	}

	virtual CDuiString GetSkinFile()
	{
		return m_strXMLPath;
	}

	virtual CDuiString GetSkinFolder()
	{
		return _T("");
	}

protected:
	CDuiString m_strXMLPath;
};
