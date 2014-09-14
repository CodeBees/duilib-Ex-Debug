#pragma once

#include "resource.h"
//导入的资源类型必须为ZIPRES，和WindowImplBase的定义一致，而且是为所有资源打包，打包资源的文件夹

class CDYFrameWnd : public WindowImplBase
{
public:
	explicit CDYFrameWnd(LPCTSTR pszXMLPath) :strXMLPath_(pszXMLPath){};
	explicit CDYFrameWnd(){};

	virtual ~CDYFrameWnd();


	virtual LPCTSTR GetWindowClassName() const { return _T("Window"); }
	virtual CDuiString GetSkinFile()                { return _T("Window.xml"); }
	virtual CDuiString GetSkinFolder()              { return _T(""); }  //为UILIB_ZIPRESOURCE时候用不到，随便实现
	virtual void  InitWindow();
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	virtual void Notify(TNotifyUI& msg);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LPCTSTR GetResourceID() const
	{
		return MAKEINTRESOURCE(IDR_ZIP_SKIN);
	};
	virtual UILIB_RESOURCETYPE GetResourceType() const{ return UILIB_ZIPRESOURCE; };

protected:
	CDuiString strXMLPath_;

};