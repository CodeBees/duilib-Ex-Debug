#pragma once

#include "resource.h"
#pragma  once
#include "stdafx.h"




//////////////////////////////////////////////////////////////////////////
///

class CPage1 : public CNotifyPump
{
public:
	CPage1();
	void SetPaintMagager(CPaintManagerUI* pPaintMgr);

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged( TNotifyUI &msg );
	virtual void OnItemClick( TNotifyUI &msg );
private:
	CPaintManagerUI* m_pPaintManager;
};

//////////////////////////////////////////////////////////////////////////

class CRichListWnd : public WindowImplBase
{
public:
	CRichListWnd(void);
	virtual ~CRichListWnd(void);

	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFolder(){
		return _T("skin\\RichListRes");
	};
	virtual CDuiString GetSkinFile(){
		return _T("duilib.xml");
	};
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void Notify( TNotifyUI &msg );
	
	virtual void InitWindow();
	virtual LRESULT OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	
	virtual UILIB_RESOURCETYPE GetResourceType() const;

	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnSelectChanged( TNotifyUI &msg );
	virtual void OnItemClick( TNotifyUI &msg );

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CPage1 m_Page1;
	
};



