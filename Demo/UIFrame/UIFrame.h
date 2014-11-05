#pragma once

#include "resource.h"
#include "localfont.h"

class CDYFrameWnd : public WindowImplBase
{
public:
	explicit CDYFrameWnd(LPCTSTR pszXMLPath):strXMLPath_(pszXMLPath){

	};
	explicit CDYFrameWnd(){
		m_paddLocalFont=new CAddLocalFont();
	};

	virtual ~CDYFrameWnd();
	virtual void OnFinalMessage(HWND hwdn)
	{
		//不是new的窗口实例，不需要自己释放
		//delete this;
		//_CrtDumpMemoryLeaks();
	}

	virtual LPCTSTR GetWindowClassName() const { return _T("Window"); }
	virtual CDuiString GetSkinFile()                { return _T("Window.xml"); }
	virtual CDuiString GetSkinFolder()              { return _T("UIFrameSkin"); }
	virtual void  InitWindow();
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	CAddLocalFont* m_paddLocalFont;
public:
	bool OnMsgBtnClick( TNotifyUI* pTNotifyUI );
	bool OnMsgBtnMouseEnter( TEventUI* pTEventUI );
	bool OnMsgBtnMouseLeave( TEventUI* pTEventUI );
	bool btnOnNotify(void* pNotify);

protected:
	CDuiString strXMLPath_;
	int nsldValue_;

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
