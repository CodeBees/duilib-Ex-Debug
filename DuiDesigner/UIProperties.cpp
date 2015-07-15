#include "StdAfx.h"
#include "UIProperties.h"
#include "DuiDesigner.h"

#include "UIDesignerView.h"
#include "ImageDialog.h"
#include "DialogCustomFonts.h"
#include "DialogDefaultAttribList.h"

using DuiLib::TRelativePosUI;
using DuiLib::IListOwnerUI;

//////////////////////////////////////////////////////////////////////////
//CMFCPropertyGridColor32Property
//32位真彩色
CMFCPropertyGridColor32Property::CMFCPropertyGridColor32Property(const CString& strName, const COLORREF& color,CPalette* pPalette/*=NULL*/,LPCTSTR lpszDescr/*=NULL*/,DWORD_PTR dwData/*=0*/)
	:CMFCPropertyGridColorProperty(strName,color,pPalette,lpszDescr,dwData)
{
}

BOOL CMFCPropertyGridColor32Property::OnUpdateValue()
{
	ASSERT_VALID(this);

	if (m_pWndInPlace == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	CString strText;
	m_pWndInPlace->GetWindowText(strText);

	COLORREF colorCurr = m_Color;
	int nA = 0,nR = 0, nG = 0, nB = 0;
	_stscanf_s(strText, _T("%2x%2x%2x%2x"), &nA, &nR, &nG, &nB);
	if(nA == 0 && (nR !=0 || nG != 0 || nB !=  0))
		nA = 0xFF;
	m_Color = ARGB(nA, nR, nG, nB);

	if (colorCurr != m_Color)
	{
		m_pWndList->OnPropertyChanged(this);
	}

	return TRUE;
}

void CMFCPropertyGridColor32Property::OnDrawValue(CDC* pDC, CRect rect)
{
	CRect rectColor = rect;

	rect.left += rect.Height();
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);

	rectColor.right = rectColor.left + rectColor.Height();
	rectColor.DeflateRect(1, 1);
	rectColor.top++;
	rectColor.left++;

	CBrush br(m_Color == (COLORREF)-1 ? m_ColorAutomatic : (m_Color&0x00FFFFFF));
	pDC->FillRect(rectColor, &br);
	pDC->Draw3dRect(rectColor, 0, 0);
}

CString CMFCPropertyGridColor32Property::FormatProperty()
{
	ASSERT_VALID(this);

	CString str;
	str.Format(_T("%02x%02x%02x%02x"),GetAValue(m_Color),GetRValue(m_Color),GetGValue(m_Color),GetBValue(m_Color));

	return str;
}

//////////////////////////////////////////////////////////////////////////
//CMFCPropertyGridImageProperty

IMPLEMENT_DYNAMIC(CMFCPropertyGridImageProperty, CMFCPropertyGridProperty)

	CMFCPropertyGridImageProperty::CMFCPropertyGridImageProperty(const CString& strName, const CString& strImage, LPCTSTR lpszDescr/* = NULL*/, DWORD_PTR dwData/* = 0*/)
	: CMFCPropertyGridProperty(strName, COleVariant((LPCTSTR)strImage), lpszDescr, dwData)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
}

void CMFCPropertyGridImageProperty::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CString strImage = m_varValue.bstrVal;
	CImageDialog dlg(strImage);
	if(dlg.DoModal()==IDOK)
	{
		strImage=dlg.GetImage();
	}

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetWindowText(strImage);
	}
	m_varValue = (LPCTSTR) strImage;

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
//CMFCPropertyGridCustomFontsProperty

IMPLEMENT_DYNAMIC(CMFCPropertyGridCustomFontsProperty, CMFCPropertyGridProperty)

	CMFCPropertyGridCustomFontsProperty::CMFCPropertyGridCustomFontsProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr/* = NULL*/, DWORD_PTR dwData/* = 0*/)
	: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
}

void CMFCPropertyGridCustomFontsProperty::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CDialogCustomFonts dlg;
	dlg.DoModal();

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
//CMFCPropertyGridDefaultAttribListProperty

IMPLEMENT_DYNAMIC(CMFCPropertyGridDefaultAttribListProperty, CMFCPropertyGridProperty)

	CMFCPropertyGridDefaultAttribListProperty::CMFCPropertyGridDefaultAttribListProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr/* = NULL*/, DWORD_PTR dwData/* = 0*/)
	: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData)
{
	m_dwFlags = AFX_PROP_HAS_BUTTON;
}

void CMFCPropertyGridDefaultAttribListProperty::OnClickButton(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	m_bButtonIsDown = TRUE;
	Redraw();

	CDialogDefaultAttribList dlg;
	dlg.DoModal();

	m_bButtonIsDown = FALSE;
	Redraw();

	if (m_pWndInPlace != NULL)
	{
		m_pWndInPlace->SetFocus();
	}
	else
	{
		m_pWndList->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
//CUIProperties

UINT WM_UI_PROPERTY_CHANGED = ::RegisterWindowMessage(_T("WM_UI_PROPERTY_CHANGED"));

IMPLEMENT_DYNAMIC(CUIProperties, CWnd)

	CUIProperties::CUIProperties(void)
{
}

CUIProperties::~CUIProperties(void)
{
}

BEGIN_MESSAGE_MAP(CUIProperties, CWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_COMMAND(ID_PROPERTIES_TOOLBAR_SORT, OnPropertiesSort)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_TOOLBAR_SORT, OnUpdatePropertiesSort)
	ON_COMMAND(ID_PROPERTIES_TOOLBAR_ALPHABETIC, OnPropertiesAlphaBetic)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_TOOLBAR_ALPHABETIC, OnUpdatePropertiesAlphaBetic)
END_MESSAGE_MAP()

BOOL CUIProperties::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(afxGlobalData.RegisterWindowClass(_T("UIPropWnd")), _T(""), dwStyle, rect, pParentWnd, nID, NULL);
}

int CUIProperties::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if(!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();

	return 0;
}

void CUIProperties::AdjustLayout()
{
	if(GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() -cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CUIProperties::SetVSDotNetLook(BOOL bSet)
{
	m_wndPropList.SetVSDotNetLook(bSet);
	m_wndPropList.SetGroupNameFullWidth(bSet);
}

void CUIProperties::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	m_wndPropList.SetFocus();
}

void CUIProperties::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AdjustLayout();
}

LRESULT CUIProperties::OnPropertyChanged(WPARAM wp, LPARAM lp)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty *)lp;

	int nLevel = pProp->GetHierarchyLevel();
	for(; nLevel>1; nLevel--)
		pProp = pProp->GetParent();

	GetOwner()->SendMessage(WM_UI_PROPERTY_CHANGED, wp, LPARAM(pProp));

	return TRUE;
}

void CUIProperties::OnPropertiesSort()
{
	m_wndPropList.SetAlphabeticMode(FALSE);
}

void CUIProperties::OnUpdatePropertiesSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(!m_wndPropList.IsAlphabeticMode());
}

void CUIProperties::OnPropertiesAlphaBetic()
{
	m_wndPropList.SetAlphabeticMode(TRUE);
}

void CUIProperties::OnUpdatePropertiesAlphaBetic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CUIProperties::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();

	CMFCPropertyGridProperty* pPropUI=NULL;
	CMFCPropertyGridProperty* pValueList=NULL;
	CMFCPropertyGridProperty* pProp=NULL;
	CMFCPropertyGridColorProperty* pPropColor=NULL;
	CMFCPropertyGridImageProperty* pPropImage=NULL;

	//Window
#pragma region Window
	pPropUI=new CMFCPropertyGridProperty(_T("Window"),classWindow);

	pValueList=new CMFCPropertyGridProperty(_T("Size"),tagWindowSize,TRUE);//size
	pProp=new CMFCPropertyGridProperty(_T("Width"),(_variant_t)(LONG)0,_T("窗体的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Height"),(_variant_t)(LONG)0,_T("窗体的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("Caption"),tagCaption,TRUE);//caption
	pProp=new CMFCPropertyGridProperty(_T("Left"),(_variant_t)(LONG)0,_T("标题的Left位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Top"),(_variant_t)(LONG)0,_T("标题的Top位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Right"),(_variant_t)(LONG)0,_T("标题的Right位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Bottom"),(_variant_t)(LONG)0,_T("标题的Bottom位置"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("SizeBox"),tagSizeBox,TRUE);//sizebox
	pProp=new CMFCPropertyGridProperty(_T("Left"),(_variant_t)(LONG)0,_T("尺寸盒的Left位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Top"),(_variant_t)(LONG)0,_T("尺寸盒的Top位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Right"),(_variant_t)(LONG)0,_T("尺寸盒的Right位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Bottom"),(_variant_t)(LONG)0,_T("尺寸盒的Bottom位置"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("RoundCorner"),tagRoundCorner,TRUE);//roundcorner
	pProp=new CMFCPropertyGridProperty(_T("Width"),(_variant_t)(LONG)0,_T("圆角的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Height"),(_variant_t)(LONG)0,_T("圆角的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("MinInfo"),tagMinInfo,TRUE);//mininfo
	pProp=new CMFCPropertyGridProperty(_T("MinWidth"),(_variant_t)(LONG)0,_T("窗口的最小跟踪宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("MinHeight"),(_variant_t)(LONG)0,_T("窗口的最小跟踪高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("MaxInfo"),tagMinInfo,TRUE);//maxinfo
	pProp=new CMFCPropertyGridProperty(_T("MaxWidth"),(_variant_t)(LONG)0,_T("窗口的最大跟踪宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Maxeight"),(_variant_t)(LONG)0,_T("窗口的最大跟踪高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp=new CMFCPropertyGridProperty(_T("ShowDirty"),(_variant_t)false,_T("指示是否显示更新区域"),tagShowDirty);//showdirty
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridCustomFontsProperty(_T("CustomFonts"),(_variant_t)_T("字体管理"),_T("管理自定义的字体"),tagCustomFonts);//customfonts
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridDefaultAttribListProperty(_T("DefaultAttribList"),(_variant_t)_T("默认属性列表管理"),_T("管理默认的属性列表"),tagDefaultAttribList);//defaultattriblist
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	// tagAlpha
	pProp=new CMFCPropertyGridProperty(_T("Alpha"),(_variant_t)(LONG)0,_T("窗口的alpha值(0-255)\n255"),tagAlpha);
	pPropUI->AddSubItem(pProp);

	// tagBkTrans
	pProp=new CMFCPropertyGridProperty(_T("BkTrans"),(_variant_t)false,_T("窗口是否使用静态透明背景\nfalse"),tagBkTrans);
	pPropUI->AddSubItem(pProp);

	// tagDefaultFontColor
	pPropColor=new CMFCPropertyGridColor32Property(_T("DefaultFontColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定默认的字体颜色"),tagDefaultFontColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	// tagSelectedFontColor
	pPropColor=new CMFCPropertyGridColor32Property(_T("SelectedColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定默认的selected字体颜色"),tagSelectedFontColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//	DisabledFontColor
	pPropColor=new CMFCPropertyGridColor32Property(_T("DisabledFontColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定默认的Disabled字体颜色"),tagDisabledFontColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	// tagLinkFontColor
	pPropColor=new CMFCPropertyGridColor32Property(_T("LinkFontColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定默认的link字体颜色"),tagLinkFontColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	// tagLinkHoverFontColor
	pPropColor=new CMFCPropertyGridColor32Property(_T("LinkHoverFontColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定默认的linkhoverfont字体颜色"),tagLinkHoverFontColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Window

	//Control
#pragma region Control
	pPropUI=new CMFCPropertyGridProperty(_T("Control"),classControl);

	pProp=new CMFCPropertyGridProperty(_T("Name"),(_variant_t)_T(""),_T("控件的名称"),tagName);//name
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("VirtualWnd"),(_variant_t)_T(""),_T("控件的虚拟窗口名称"),tagVirtualWnd);//VirtualWnd
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("Text"),(_variant_t)_T(""),_T("控件的显示文本"),tagText);//text
	pPropUI->AddSubItem(pProp);

	pValueList=new CMFCPropertyGridProperty(_T("Pos"),tagPos,TRUE);//pos
	pProp=new CMFCPropertyGridProperty(_T("Left"),(_variant_t)(LONG)0,_T("控件的Left位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Top"),(_variant_t)(LONG)0,_T("控件的Top位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Right"),(_variant_t)(LONG)0,_T("控件的Right位置"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Bottom"),(_variant_t)(LONG)0,_T("控件的Bottom位置"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("RelativePos"),tagRelativePos,TRUE);//relativepos
	pProp=new CMFCPropertyGridProperty(_T("MoveX"),(_variant_t)(LONG)0,_T("控件的水平位移"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("MoveY"),(_variant_t)(LONG)0,_T("控件的垂直位移"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("ZoomX"),(_variant_t)(LONG)0,_T("控件的水平比例"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("ZoomY"),(_variant_t)(LONG)0,_T("控件的垂直比例"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("Size"),tagSize,TRUE);//size
	pProp=new CMFCPropertyGridProperty(_T("Width"),(_variant_t)(LONG)0,_T("控件的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Height"),(_variant_t)(LONG)0,_T("控件的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("MinSize"),tagMinSize,TRUE);//minsize
	pProp=new CMFCPropertyGridProperty(_T("MinWidth"),(_variant_t)(LONG)0,_T("指定控件的最小宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("MinHeight"),(_variant_t)(LONG)0,_T("指定控件的最小高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("MaxSize"),tagMaxSize,TRUE);
	pProp=new CMFCPropertyGridProperty(_T("MaxWidth"),(_variant_t)(LONG)0,_T("指定控件的最大宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("MaxHeight"),(_variant_t)(LONG)0,_T("指定控件的最大高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("Padding"),tagPadding,TRUE);//padding
	pProp=new CMFCPropertyGridProperty(_T("Left"),(_variant_t)(LONG)0,_T("指定控件内部的左边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Top"),(_variant_t)(LONG)0,_T("指定控件内部的上边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Right"),(_variant_t)(LONG)0,_T("指定控件内部的右边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Bottom"),(_variant_t)(LONG)0,_T("指定控件内部的下边距"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pPropImage=new CMFCPropertyGridImageProperty(_T("BkImage"),_T(""),_T("指定控件的背景图片"),tagBkImage);//bkimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	//32位真彩色
	pPropColor=new CMFCPropertyGridColor32Property(_T("BkColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定控件的背景颜色"),tagBkColor);//bkcolor
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	pPropColor = new CMFCPropertyGridColor32Property(_T("BkColor2"), (LONG)ARGB(0, 0, 0, 0), NULL, _T("指定控件的背景颜色2"), tagBkColor2);//bkcolor2
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	pPropColor=new CMFCPropertyGridColorProperty(_T("BorderColor"),(LONG)RGB(0,0,0),NULL,_T("指定控件的边框颜色"),tagBorderColor);//bordercolor
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//focusbordercolor
	pPropColor=new CMFCPropertyGridColorProperty(_T("focusbordercolor"),(LONG)RGB(0,0,0),NULL,_T("指定控件边框获得焦点时边框的颜色"),tagFocusBorderColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	pProp=new CMFCPropertyGridProperty(_T("BorderSize"),(_variant_t)(LONG)0,_T("指定控件的边框线宽\n1"),tagBorderSize);//bordersize
	pPropUI->AddSubItem(pProp);

	//borderround
	pValueList=new CMFCPropertyGridProperty(_T("borderround"),tagBorderRound,TRUE);
	pProp=new CMFCPropertyGridProperty(_T("Width"),(_variant_t)(LONG)0,_T("边框圆角的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Height"),(_variant_t)(LONG)0,_T("边框圆角的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp=new CMFCPropertyGridProperty(_T("Enabled"),(_variant_t)true,_T("指示是否已启用该控件\nTrue"),tagEnabled);//enabled
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("Float"),(_variant_t)false,_T("确定该控件是固定的，还是浮动的\nFalse"),tagFloat);//float
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("Visible"),(_variant_t)true,_T("确定该控件是可见的，还是隐藏的\nTrue"),tagVisible);//visible
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("Mouse"),(_variant_t)true,_T("指示该控件是否响应鼠标操作\nTrue"),tagMouse);//mouse
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("Menu"),(_variant_t)false,_T("指示该控件是否需要右键菜单\nFalse"),tagMenu);//menu
	pPropUI->AddSubItem(pProp);

	//colorhsl
	pProp=new CMFCPropertyGridProperty(_T("ColorHSL"),(_variant_t)false,_T("指示该控件的颜色是否随窗口的hsl变化而变化\nFalse"),tagColorHSL);
	pPropUI->AddSubItem(pProp);

	//tooltip
	pProp=new CMFCPropertyGridProperty(_T("Tooltip"),(_variant_t)_T(""),_T("指示该控件鼠标悬浮提示"),tagTooltip);
	pPropUI->AddSubItem(pProp);

	//userdata
	pProp=new CMFCPropertyGridProperty(_T("UserData"),(_variant_t)_T(""),_T("指示该控件自定义标识\nFalse"),tagUserData);
	pPropUI->AddSubItem(pProp);

	//keyboard
	pProp=new CMFCPropertyGridProperty(_T("KeyBoard"),(_variant_t)false,_T("指示CButton类控件是否接受TabStop和按键事件\nFalse"),tagKeyBoard);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Control

	//Label
#pragma region Label
	pPropUI=new CMFCPropertyGridProperty(_T("Label"),classLabel);

	//align
	pProp=new CMFCPropertyGridProperty(_T("Align"),_T(""),_T("指示文本的水平对齐方式"),tagAlign);

	pProp->AddOption(_T("Left"));
	pProp->AddOption(_T("Center"));
	pProp->AddOption(_T("Right"));
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	//Valign
	pProp=new CMFCPropertyGridProperty(_T("VAlign"),_T(""),_T("指示文本的垂直对齐方式"),tagVAlign);
	pProp->AddOption(_T("Top"));
	pProp->AddOption(_T("Center"));
	pProp->AddOption(_T("Bottom"));
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	//textcolor
	pPropColor=new CMFCPropertyGridColorProperty(_T("TextColor"),(LONG)RGB(0,0,0),NULL,_T("指定文本的颜色"),tagTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//disabledtextcolor
	pPropColor=new CMFCPropertyGridColorProperty(_T("DisabledTextColor"),(LONG)RGB(0,0,0),NULL,_T("指定文本的颜色"),tagDisabledTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//font
	pProp=new CMFCPropertyGridProperty(_T("Font"),(_variant_t)(LONG)-1,_T("指定文本的字体"),tagFont);
	pPropUI->AddSubItem(pProp);

	//textpadding
	pValueList=new CMFCPropertyGridProperty(_T("TextPadding"),tagTextPadding,TRUE);
	pProp=new CMFCPropertyGridProperty(_T("Left"),(_variant_t)(LONG)0,_T("指定文本区域的左边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Top"),(_variant_t)(LONG)0,_T("指定文本区域的上边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Right"),(_variant_t)(LONG)0,_T("指定文本区域的右边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Bottom"),(_variant_t)(LONG)0,_T("指定文本区域的下边距"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	//showhtml
	pProp=new CMFCPropertyGridProperty(_T("ShowHtml"),(_variant_t)false,_T("指示是否使用HTML格式的文本"),tagShowHtml);
	pPropUI->AddSubItem(pProp);

	//endellipsis
	pProp=new CMFCPropertyGridProperty(_T("EndEllipsis"),(_variant_t)false,_T("指示句末显示不全是否使用...代替"),tagEndEllipsis);
	pPropUI->AddSubItem(pProp);
	//PathEllipsis
	pProp = new CMFCPropertyGridProperty(_T("PathEllipsis"), (_variant_t)false, _T("显示不全是否句中使用...代替"), tagPathEllipsis);
	pPropUI->AddSubItem(pProp);

	//enabledeffect-依赖lable的edit等控件未支持特效，除lable外的控件设置了无用
	pProp=new CMFCPropertyGridProperty(_T("EnabledEffect"),(_variant_t)false,_T("是否使用字体特效，只支持Lable"),tagEnabledEffect);
	pPropUI->AddSubItem(pProp);
	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Label

	//Button
#pragma region Button
	pPropUI=new CMFCPropertyGridProperty(_T("Button"),classButton);

	pPropImage=new CMFCPropertyGridImageProperty(_T("NormalImage"),_T(""),_T("指定按钮正常显示时的图片"),tagNormalImage);//normalimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("HotImage"),_T(""),_T("指定按钮获得热点时的图片"),tagHotImage);//hotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("PushedImage"),_T(""),_T("指定按钮被按压下时的图片"),tagPushedImage);//pushedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("FocusedImage"),_T(""),_T("指定按钮获得焦点后的图片"),tagFocusedImage);//focusedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("DisabledImage"),_T(""),_T("指定按钮被禁用后的图片"),tagDisabledImage);//disabledimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//tagHotTextColor
	pPropColor = new CMFCPropertyGridColorProperty(_T("hottextcolor"), (LONG)RGB(0, 0, 0), NULL, _T("按钮获得热点时字体颜色"), tagHotTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//	tagHotBKColor
	pPropColor=new CMFCPropertyGridColor32Property(_T("HotBKColor"),(LONG)ARGB(0,0,0,0),NULL,_T("按钮获得热点时背景颜色"),tagHotBKColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//	tagPushedTextColor
	pPropColor = new CMFCPropertyGridColorProperty(_T("PushedTextColor"), (LONG)RGB(0, 0, 0), NULL, _T("按钮被按下时字体颜色"), tagPushedTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);
	//tagPushedBKColor
	pPropColor=new CMFCPropertyGridColor32Property(_T("PushedBKColor"),(LONG)ARGB(0,0,0,0),NULL,_T("按钮被按下时背景颜色"),tagPushedBKColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//	tagfocusedtextcolor
	pPropColor = new CMFCPropertyGridColorProperty(_T("FocusedTextColor"), (LONG)RGB(0, 0, 0), NULL, _T("按钮获取到焦点时字体颜色"), tagfocusedtextcolor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Button

	//Edit
#pragma region Edit
	pPropUI=new CMFCPropertyGridProperty(_T("Edit"),classEdit);

	//normalimage
	pPropImage=new CMFCPropertyGridImageProperty(_T("NormalImage"),_T(""),_T("指定编辑框正常显示时的图片"),tagEditNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//hotimage
	pPropImage=new CMFCPropertyGridImageProperty(_T("HotImage"),_T(""),_T("指定编辑框获得热点时的图片"),tagEditHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//focusedimage
	pPropImage=new CMFCPropertyGridImageProperty(_T("FocusedImage"),_T(""),_T("指定编辑框获得焦点后的图片"),tagEditFocusedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//disabledimage
	pPropImage=new CMFCPropertyGridImageProperty(_T("DisabledImage"),_T(""),_T("指定编辑框被禁用后的图片"),tagEditDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	//tagEditFloatPointOnly
	pProp = new CMFCPropertyGridProperty(_T("FloatPointOnly"), (_variant_t)false, _T("是否只允许浮点数输入到编辑框"), tagEditFloatPointOnly);
	pPropUI->AddSubItem(pProp);
	//tagNumberOnly
	pProp=new CMFCPropertyGridProperty(_T("numberonly"),(_variant_t)false,_T("是否只允许数字输入到编辑框"),tagNumberOnly);
	pPropUI->AddSubItem(pProp);
	//readonly
	pProp=new CMFCPropertyGridProperty(_T("ReadOnly"),(_variant_t)false,_T("指示是否只读"),tagReadOnly);
	pPropUI->AddSubItem(pProp);

	//password
	pProp=new CMFCPropertyGridProperty(_T("Password"),(_variant_t)false,_T("指示是否使用密码框"),tagPassword);
	pPropUI->AddSubItem(pProp);

	//maxchar
	pProp=new CMFCPropertyGridProperty(_T("MaxChar"),(_variant_t)(LONG)0,_T("指示输入字符最大长度\n255"),tagMaxChar);
	pPropUI->AddSubItem(pProp);

	//nativebkcolor
	pPropColor=new CMFCPropertyGridColor32Property(_T("NativeBKColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定Windows原生Edit控件的背景颜色"),tagNativeBKColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Edit

#pragma region RichEdit
	pPropUI = new CMFCPropertyGridProperty(_T("RichEdit"), classRichEdit);


	//VscrollBar
	pProp = new CMFCPropertyGridProperty(_T("vscrollbar"), (_variant_t)false, _T("是否使用竖向滚动条"), tagREVscrollBar);
	pPropUI->AddSubItem(pProp);
	//AutoVscroll,
	pProp = new CMFCPropertyGridProperty(_T("autovscroll"), (_variant_t)false, _T("是否随输入竖向滚动"), tagREAutoVscroll);
	pPropUI->AddSubItem(pProp);
	//HscrollBar
	pProp = new CMFCPropertyGridProperty(_T("hscrollbar"), (_variant_t)false, _T("是否使用横向滚动条"), tagREHscrollBar);
	pPropUI->AddSubItem(pProp);
	//
	pProp = new CMFCPropertyGridProperty(_T("autohscroll"), (_variant_t)false, _T("是否随输入横向滚动"), tagREAutoHscroll);
	pPropUI->AddSubItem(pProp);
	//richedit textpadding
	pValueList=new CMFCPropertyGridProperty(_T("TextPadding"),tagRETextPadding,TRUE);
	pProp=new CMFCPropertyGridProperty(_T("Left"),(_variant_t)(LONG)0,_T("指定文本区域的左边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Top"),(_variant_t)(LONG)0,_T("指定文本区域的上边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Right"),(_variant_t)(LONG)0,_T("指定文本区域的右边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Bottom"),(_variant_t)(LONG)0,_T("指定文本区域的下边距"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);
	//
	pProp = new CMFCPropertyGridProperty(_T("wanttab"), (_variant_t)true, _T("是否接受tab按键消息"), tagREWantTab);
	pPropUI->AddSubItem(pProp);
	//
	pProp = new CMFCPropertyGridProperty(_T("wantreturn"), (_variant_t)true, _T("是否接受return按键消息"), tagREWantReturn);
	pPropUI->AddSubItem(pProp);
	//
	pProp = new CMFCPropertyGridProperty(_T("wantctrlreturn"), (_variant_t)true, _T("是否接受ctrl+return按键消息"),tagREWantCtrlReturn );
	pPropUI->AddSubItem(pProp);
	//
	pProp = new CMFCPropertyGridProperty(_T("rich"), (_variant_t)true, _T("是否使用富格式"), tagRERich);
	pPropUI->AddSubItem(pProp);
	//
	pProp = new CMFCPropertyGridProperty(_T("multiline"), (_variant_t)true, _T("是否使用多行"),tagREMultiline );
	pPropUI->AddSubItem(pProp);
	//
	pProp = new CMFCPropertyGridProperty(_T("readonly"), (_variant_t)false, _T("是否只读"), tagREReadonly);
	pPropUI->AddSubItem(pProp);
	//
	pProp = new CMFCPropertyGridProperty(_T("password"), (_variant_t)false, _T("是否显示密码符"),tagREPassword );
	pPropUI->AddSubItem(pProp);
	//align
	pProp = new CMFCPropertyGridProperty(_T("Align"), _T("Left"), _T("指示文本的水平对齐方式"), tagREAlign);
	pProp->AddOption(_T("Left"));
	pProp->AddOption(_T("Center"));
	pProp->AddOption(_T("Right"));
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	//font
	pProp = new CMFCPropertyGridProperty(_T("Font"), (_variant_t)(LONG)-1, _T("指定文本的字体"), tagREFont);
	pPropUI->AddSubItem(pProp);
	//textcolor
	pPropColor = new CMFCPropertyGridColorProperty(_T("TextColor"), (LONG)RGB(0, 0, 0), NULL, _T("指定文本的颜色"), tagRETextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);


	m_wndPropList.AddProperty(pPropUI);
#pragma endregion RichEdit

	//Option
#pragma region Option
	pPropUI=new CMFCPropertyGridProperty(_T("Option"),classOption);

	//Selected
	pProp = new CMFCPropertyGridProperty(_T("Selected"), (_variant_t)false, _T("指示是否已被选中"), tagSelected);//selected
	pPropUI->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Group"), (_variant_t)_T(""), _T("指定参与组合的名称"), tagGroup);//group
	pPropUI->AddSubItem(pProp);

	//tagSelectedImage
	pPropImage=new CMFCPropertyGridImageProperty(_T("SelectedImage"),_T(""),_T("指定复选框被选择后的图片"),tagSelectedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	//tagOptSelectedHotImage
	pPropImage = new CMFCPropertyGridImageProperty(_T("selectedhotimage"), _T(""), _T("指定复选框被选择后鼠标悬浮的的状态图片"), tagOptSelectedHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//tagSelectedpushImage
	pPropImage = new CMFCPropertyGridImageProperty(_T("selectedpushedimage"), _T(""), _T("指定复选框被选择后被按下的状态图片"), tagOptSelectedPushImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//SelectedTextColor
	pPropColor = new CMFCPropertyGridColorProperty(_T("Selectedtextcolor"), (LONG)RGB(0, 0, 0), NULL, _T("选中状态字体颜色"), tagSelectedTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//selectedbkcolor
	//32位真彩色
	pPropColor = new CMFCPropertyGridColor32Property(_T("selectedbkcolor"), (LONG)ARGB(0, 0, 0, 0), NULL, _T("指定选中状态的背景颜色"), tagSelectedBkColor);//bkcolor
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"), ::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);



	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Option

	//Progress
#pragma region Progress
	pPropUI=new CMFCPropertyGridProperty(_T("Progress"),classProgress);

	pPropImage=new CMFCPropertyGridImageProperty(_T("ForeImage"),_T(""),_T("指定进度条的前景图片"),tagForeImage);//foreimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pValueList=new CMFCPropertyGridProperty(_T("MinMax"),tagMinMax,TRUE);//minmax
	pProp=new CMFCPropertyGridProperty(_T("Min"),(_variant_t)(LONG)0,_T("指定进度条的最小值"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Max"),(_variant_t)(LONG)0,_T("指定进度条的最大值"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp=new CMFCPropertyGridProperty(_T("Value"),(_variant_t)(LONG)0,_T("指定进度条的值"),tagValue);//value
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("Hor"),(_variant_t)false,_T("指示进度条是否水平"),tagHor);//hor
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("IsStretchFore"),(_variant_t)false,_T("指定前景是否缩放"),tagIsStretchFore);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Progress

	//Slider
#pragma region Slider
	pPropUI=new CMFCPropertyGridProperty(_T("Slider"),classSlider);


	//tagSendMoveNotify
	pProp=new CMFCPropertyGridProperty(_T("SendMoveNotify"),(_variant_t)false,_T("是否在滑块滑动过程中通知主窗体正在发生滑动改变"),tagSendMoveNotify);
	pPropUI->AddSubItem(pProp);

	pPropImage=new CMFCPropertyGridImageProperty(_T("ThumbImage"),_T(""),_T("指定滑块的滑条图片"),tagThumbImage);//thumbimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("ThumbHotImage"),_T(""),_T("指定滑条获得热点时的图片"),tagThumbHotImage);//thumbhotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("ThumbPushedImage"),_T(""),_T("指定滑条被按压后的图片"),tagThumbPushedImage);//thumbpushedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pValueList=new CMFCPropertyGridProperty(_T("ThumbSize"),tagThumbSize,TRUE);//thumbsize
	pProp=new CMFCPropertyGridProperty(_T("Width"),(_variant_t)(LONG)0,_T("指定滑条的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Height"),(_variant_t)(LONG)0,_T("指定滑条的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Slider

	//ActiveX
#pragma region ActiveX
	pPropUI=new CMFCPropertyGridProperty(_T("ActiveX"),classActiveX);

	//clsid
	pProp=new CMFCPropertyGridProperty(_T("Clsid"),(_variant_t)_T(""),_T("指定ActiveX控件的Clsid值"),tagClsid);
	pPropUI->AddSubItem(pProp);

	//delaycreate
	pProp=new CMFCPropertyGridProperty(_T("DelayCreate"),(_variant_t)true,_T("指示是否延迟加载ActiveX控件"),tagDelayCreate);
	pPropUI->AddSubItem(pProp);//added by 邓景仁 2011-09-08

	// modulename
	pProp=new CMFCPropertyGridProperty(_T("ModuleName"),(_variant_t)_T(""),_T("指示从指定位置加载ActiveX控件\n如(flash/flash.ocx)"),tagModuleName);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion ActiveX

#pragma region ColorPalette
	pPropUI=new CMFCPropertyGridProperty(_T("ColorPalette"),classColorPalette);
	//palletheight
	pProp=new CMFCPropertyGridProperty(_T("palletheight"),(_variant_t)(LONG)0,_T("指定调色板控件的调色区域高度值"),tagCPPalletheight);
	pPropUI->AddSubItem(pProp);
	//barheight
	pProp=new CMFCPropertyGridProperty(_T("barheight"),(_variant_t)(LONG)0,_T("指定调色板控件的明度条高度值"),tagCPBarHeight);
	pPropUI->AddSubItem(pProp);
	//thumbimage
	pPropImage=new CMFCPropertyGridImageProperty(_T("thumbimage"),_T(""),_T("指定控件的背景图片"),tagCPThumbImage);//bkimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion ColorPalette

#pragma  region Date

#pragma  endregion Date

	//Container
#pragma region Container
	pPropUI=new CMFCPropertyGridProperty(_T("Container"),classContainer);

	pValueList=new CMFCPropertyGridProperty(_T("Inset"),tagInset,TRUE);//inset
	pProp=new CMFCPropertyGridProperty(_T("Left"),(_variant_t)(LONG)0,_T("指定容器客户区域的左边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Top"),(_variant_t)(LONG)0,_T("指定容器客户区域的上边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Right"),(_variant_t)(LONG)0,_T("指定容器客户区域的右边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Bottom"),(_variant_t)(LONG)0,_T("指定容器客户区域的下边距\n0"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pProp=new CMFCPropertyGridProperty(_T("ChildPadding"),(_variant_t)(LONG)0,_T("指定子控件之间的间距\n0"),tagChildPadding);//childpadding
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("MouseChild"),(_variant_t)false,_T("指定本控件的子控件是否可以响应用户操作\nTrue"),tagMouseChild);//mousechild
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("HScrollBar"),(_variant_t)false,_T("指示是否启用水平滚动条\nFalse"),tagHScrollBar);//hscrollbar
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("VScrollBar"),(_variant_t)false,_T("指示是否启用垂直滚动条\nFalse"),tagVScrollBar);//vscrollbar
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Container

	//Item
#pragma region Item
	pPropUI=new CMFCPropertyGridProperty(_T("Item"),classItem);

	pPropImage=new CMFCPropertyGridImageProperty(_T("ItemBkImage"),_T(""),_T("指定组项正常显示时的图片"),tagItemBkImage);//itembkimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("ItemHotImage"),_T(""),_T("指定组项获得热点时的图片"),tagItemHotImage);//itemhotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("ItemSelectedImage"),_T(""),_T("指定组项被选择时的图片"),tagItemSelectedImage);//itemselectedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("ItemDisabledImage"),_T(""),_T("指定组项被禁用后的图片"),tagItemDisabledImage);//itemdisabledimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	//itemtextpadding
	pValueList=new CMFCPropertyGridProperty(_T("ItemTextPadding"),tagItemTextPadding,TRUE);
	pProp=new CMFCPropertyGridProperty(_T("Left"),(_variant_t)(LONG)0,_T("指定组项文本区域的左边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Top"),(_variant_t)(LONG)0,_T("指定组项文本区域的上边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Right"),(_variant_t)(LONG)0,_T("指定组项文本区域的右边距\n0"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Bottom"),(_variant_t)(LONG)0,_T("指定组项文本区域的下边距\n0"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	//itemalign
	pProp=new CMFCPropertyGridProperty(_T("ItemAlign"),_T("Center"),_T("指示组项文本的对齐方式\nCenter"),tagItemAlign);
	pProp->AddOption(_T("Center"));
	pProp->AddOption(_T("Left"));
	pProp->AddOption(_T("Right"));
	pProp->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	//itemtextcolor
	pPropColor=new CMFCPropertyGridColor32Property(_T("ItemTextColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定组项文本的颜色"),tagItemTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itembkcolor
	pPropColor=new CMFCPropertyGridColor32Property(_T("ItemBkColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定组项背景的颜色"),tagItemBkColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemselectedtextcolor
	pPropColor=new CMFCPropertyGridColor32Property(_T("ItemSelectedTextColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定组项被选中后文本的颜色"),tagItemSelectedTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemselectedbkcolor
	pPropColor=new CMFCPropertyGridColor32Property(_T("ItemSelectedBkColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定组项被选中后背景的颜色"),tagItemSelectedBkColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemhottextcolor
	pPropColor=new CMFCPropertyGridColor32Property(_T("ItemHotTextColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定组项获得热点时文本的颜色"),tagItemHotTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemhotbkcolor
	pPropColor=new CMFCPropertyGridColor32Property(_T("ItemHotBkColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定组项获得热点时背景的颜色"),tagItemHotBkColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemdisabledtextcolor
	pPropColor=new CMFCPropertyGridColor32Property(_T("ItemDisabledTextColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定组项被禁用后文本的颜色"),tagItemDisabledTextColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//itemdisabledbkcolor
	pPropColor=new CMFCPropertyGridColor32Property(_T("ItemDisabledBkColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定组项被禁用后背景的颜色"),tagItemDisabledBkColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//ItemLineColor
	pPropColor=new CMFCPropertyGridColor32Property(_T("ItemLineColor"),(LONG)ARGB(0,0,0,0),NULL,_T("指定组项分割线的颜色"),tagItemLineColor);
	pPropColor->EnableOtherButton(_T("其他..."));
	pPropColor->EnableAutomaticButton(_T("默认"),::GetSysColor(COLOR_3DFACE));
	pPropUI->AddSubItem(pPropColor);

	//ItemFont
	pProp=new CMFCPropertyGridProperty(_T("ItemFont"),(_variant_t)(LONG)-1,_T("指定组项文本的字体\n-1"),tagItemFont);
	pPropUI->AddSubItem(pProp);

	//ItemShowHtml
	pProp=new CMFCPropertyGridProperty(_T("ItemShowHtml"),(_variant_t)false,_T("指示是否使用Html格式文本\nFalse"),tagItemShowHtml);
	pPropUI->AddSubItem(pProp);

	//MultiExpanding
	pProp=new CMFCPropertyGridProperty(_T("MultiExpanding"),(_variant_t)false,_T("指示是否支持多个item同时打开\nFalse"),tagMultiExpanding);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Item

	//Combo
#pragma region Combo
	pPropUI=new CMFCPropertyGridProperty(_T("Combo"),classCombo);

	pPropImage=new CMFCPropertyGridImageProperty(_T("NormalImage"),_T(""),_T("指定组合框正常显示时的图片"),tagComboNormalImage);//normalimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("HotImage"),_T(""),_T("指定组合框获得热点时的图片"),tagComboHotImage);//hotimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("PushedImage"),_T(""),_T("指定组合框被按压下时的图片"),tagComboPushedImage);//pushedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("FocusedImage"),_T(""),_T("指定组合框获得焦点后的图片"),tagComboFocusedImage);//focusedimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("DisabledImage"),_T(""),_T("指定组合框被禁用后的图片"),tagComboDisabledImage);//disabledimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pValueList=new CMFCPropertyGridProperty(_T("TextPadding"),tagComboTextPadding,TRUE);//textpadding
	pProp=new CMFCPropertyGridProperty(_T("Left"),(_variant_t)(LONG)0,_T("指定文本区域的左边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Top"),(_variant_t)(LONG)0,_T("指定文本区域的上边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Right"),(_variant_t)(LONG)0,_T("指定文本区域的右边距"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Bottom"),(_variant_t)(LONG)0,_T("指定文本区域的下边距"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	pValueList=new CMFCPropertyGridProperty(_T("DropBoxSize"),tagComboDropBoxSize,TRUE);//dropboxsize
	pProp=new CMFCPropertyGridProperty(_T("Width"),(_variant_t)(LONG)0,_T("下拉列表的宽度"));
	pValueList->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Height"),(_variant_t)(LONG)0,_T("下拉列表的高度"));
	pValueList->AddSubItem(pProp);
	pPropUI->AddSubItem(pValueList);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion Combo

	//VerticalLayout
#pragma region VerticalLayout
	pPropUI=new CMFCPropertyGridProperty(_T("VerticalLayout"),classVerticalLayout);

	pProp=new CMFCPropertyGridProperty(_T("SepHeight"),(_variant_t)(LONG)0,_T("分隔符高度,正负表示分隔符在顶部还是底部,如(4)\n0"),tagSepHeight);//sepwidth
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("SepImm"),(_variant_t)false,_T("拖动分隔符是否立即改变大小\nfalse"),tagVLSepImm);//sepimm
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);

#pragma endregion VerticalLayout

	//HorizontalLayout
#pragma region HorizontalLayout
	pPropUI=new CMFCPropertyGridProperty(_T("HorizontalLayout"),classHorizontalLayout);

	pProp=new CMFCPropertyGridProperty(_T("SepWidth"),(_variant_t)(LONG)0,_T("分隔符宽,正负表示分隔符在左边还是右边\n0"),tagSepWidth);//sepwidth
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("SepImm"),(_variant_t)false,_T("拖动分隔符是否立即改变大小\nfalse"),tagHLSepImm);//sepimm
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion HorizontalLayout


	//TileLayout
#pragma region TileLayout
	pPropUI=new CMFCPropertyGridProperty(_T("TileLayout"),classTileLayout);

	pProp=new CMFCPropertyGridProperty(_T("Columns"),(_variant_t)(LONG)0,_T("指定并列布局的列数"),tagColumns);//columns
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion TileLayout

	//tablayout
#pragma region TabLayout
	pPropUI=new CMFCPropertyGridProperty(_T("TabLayout"),classTabLayout);

	// selectedid
	pProp=new CMFCPropertyGridProperty(_T("selectedid"),(_variant_t)(LONG)0,_T("默认选中的页面ID\n从0开始计数"),tagSelectedID);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);

#pragma endregion TabLayout
	//childlayout
#pragma region ChildLayout
	pPropUI=new CMFCPropertyGridProperty(_T("ChildLayout"),classChildLayout);
	// xmlfile
	pProp=new CMFCPropertyGridProperty(_T("xmlfile"),_T(""),_T("子窗体XML布局文件"),tagCLXMLFile);
	pPropUI->AllowEdit(FALSE);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);

#pragma endregion  ChildLayout

	//List
#pragma region List
	pPropUI=new CMFCPropertyGridProperty(_T("List"),classList);

	pProp=new CMFCPropertyGridProperty(_T("Header"),(_variant_t)false,_T("指定是否显示列表表头\nTrue"),tagListHeader);
	pPropUI->AddSubItem(pProp);

	pPropImage=new CMFCPropertyGridImageProperty(_T("headerbkimage"),_T(""),_T("指定表头背景图片"),tagListHeaderBKImage);//normalimage
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion List

	//tagListElement
#pragma  region ListElement
	pPropUI = new CMFCPropertyGridProperty(_T("ListElement"), classListElement);
	pProp = new CMFCPropertyGridProperty(_T("selected"), (_variant_t)false, _T("是否选中,如(true)"), tagLESelect);
	pPropUI->AddSubItem(pProp);
	m_wndPropList.AddProperty(pPropUI);
#pragma  endregion ListElement



	//ScrollBar
#pragma region ScrollBar

	pPropUI=new CMFCPropertyGridProperty(_T("ScrollBar"),classScrollBar);

	pProp=new CMFCPropertyGridProperty(_T("Range"),(_variant_t)(LONG)0,_T("指定滚动范围\n100"),tagScrollBarRange);
	pPropUI->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("Value"),(_variant_t)(LONG)0,_T("指定滚动位置\n0"),tagScrollBarValue);
	pPropUI->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("LineSize"),(_variant_t)(LONG)0,_T("指定滚动一行的大小\n8"),tagScrollBarLineSize);
	pPropUI->AddSubItem(pProp);

	pProp=new CMFCPropertyGridProperty(_T("ShowButton1"),(_variant_t)false,_T("是否显示左或上按钮\nTrue"),tagScrollBarShowButton1);
	pPropUI->AddSubItem(pProp);
	pProp=new CMFCPropertyGridProperty(_T("ShowButton2"),(_variant_t)false,_T("是否显示左或上按钮\nTrue"),tagScrollBarShowButton2);
	pPropUI->AddSubItem(pProp);

	pPropImage=new CMFCPropertyGridImageProperty(_T("Button1NormalImage"),_T(""),_T("指定左或上按钮普通状态图片"),tagScrollBarButton1NormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("Button1HotImage"),_T(""),_T("指定左或上按钮鼠标悬浮状态图片"),tagScrollBarButton1HotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("Button1PushedImage"),_T(""),_T("指定左或上按钮鼠标按下状态图片"),tagScrollBarButton1PushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("Button1DisabledImage"),_T(""),_T("指定左或上按钮鼠标禁用状态图片"),tagScrollBarButton1DisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("Button2NormalImage"),_T(""),_T("指定左或上按钮普通状态图片"),tagScrollBarButton2NormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("Button2HotImage"),_T(""),_T("指定左或上按钮鼠标悬浮状态图片"),tagScrollBarButton2HotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("Button2PushedImage"),_T(""),_T("指定左或上按钮鼠标按下状态图片"),tagScrollBarButton2PushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("Button2DisabledImage"),_T(""),_T("指定左或上按钮鼠标悬禁用态图片"),tagScrollBarButton2DisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("ThumbNormalImage"),_T(""),_T("指定滑块普通状态图片"),tagScrollBarThumbNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("ThumbHotImage"),_T(""),_T("指定滑块鼠标悬浮状态图片"),tagScrollBarThumbHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("ThumbPushedImage"),_T(""),_T("指定滑块鼠标按下状态图片"),tagScrollBarThumbPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("ThumbDisabledImage"),_T(""),_T("指定滑块禁用状态图片"),tagScrollBarThumbDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("RailNormalImage"),_T(""),_T("指定滑块中间标识普通状态图片"),tagScrollBarRailNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("RailHotImage"),_T(""),_T("指定滑块中间标识鼠标悬浮状态图片"),tagScrollBarRailHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("RailPushedImage"),_T(""),_T("指定滑块中间标识鼠标按下状态图片"),tagScrollBarRailPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("RailDisabledImage"),_T(""),_T("指定滑块中间标识禁用状态图片"),tagScrollBarRailDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	pPropImage=new CMFCPropertyGridImageProperty(_T("BKNormalImage"),_T(""),_T("指定背景普通状态图片"),tagScrollBarBKNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("BKHotImage"),_T(""),_T("指定背景鼠标悬浮状态图片"),tagScrollBarBKHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("BKPushedImage"),_T(""),_T("指定背景鼠标按下状态图片"),tagScrollBarBKPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);
	pPropImage=new CMFCPropertyGridImageProperty(_T("BKDisabledImage"),_T(""),_T("指定背景禁用状态图片"),tagScrollBarBKDisabledImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);

#pragma endregion ScrollBar



#pragma region ListHeaderItem
	pPropUI=new CMFCPropertyGridProperty(_T("ListHeaderItem"),classListHeaderItem);

	// dragable
	pProp=new CMFCPropertyGridProperty(_T("Dragable"),(_variant_t)true,_T("是否可拖动改变大小\ntrue"),tagDragable);
	pPropUI->AddSubItem(pProp);

	// sepwidth
	pProp=new CMFCPropertyGridProperty(_T("SepWidth"),(_variant_t)(LONG)0,_T("分隔符宽\n4"),tagListHeaderItemSepWidth);
	pPropUI->AddSubItem(pProp);

	// normalimage
	pPropImage=new CMFCPropertyGridImageProperty(_T("NormalImage"),_T(""),_T("普通状态图片"),tagListHeaderItemNormalImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// hotimage
	pPropImage=new CMFCPropertyGridImageProperty(_T("HotImage"),_T(""),_T("鼠标悬浮的状态图片"),tagListHeaderItemHotImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// PushedImage
	pPropImage=new CMFCPropertyGridImageProperty(_T("PushedImage"),_T(""),_T("鼠标按下的状态图片"),tagListHeaderItemPushedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// focusedimage
	pPropImage=new CMFCPropertyGridImageProperty(_T("FocusedImage"),_T(""),_T("获得焦点时的状态图片"),tagListHeaderItemFocusedImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	// sepimage
	pPropImage=new CMFCPropertyGridImageProperty(_T("SepImage"),_T(""),_T("拖动条图片"),tagSepImage);
	pPropImage->AllowEdit(FALSE);
	pPropUI->AddSubItem(pPropImage);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion ListHeaderItem

#pragma region WebBrowser

	pPropUI=new CMFCPropertyGridProperty(_T("WebBrowser"),classWebBrowser);

	// homepage
	pProp=new CMFCPropertyGridProperty(_T("homepage"),(_variant_t)_T(""),_T("默认网址\n"),tagWebBrowserHomePage);
	pPropUI->AddSubItem(pProp);

	// autonavi
	pProp = new CMFCPropertyGridProperty(_T("autonavi"), (_variant_t)true, _T("是否显示默认页面\ntrue"), tagWebBrowserAutoNavi);
	pPropUI->AddSubItem(pProp);

	m_wndPropList.AddProperty(pPropUI);
#pragma endregion WebBrowser

	HideAllProperties();
}

void CUIProperties::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

void CUIProperties::HideAllProperties(BOOL bAdjustLayout/*=FALSE*/)
{
	for(int i=0; i<m_wndPropList.GetPropertyCount(); i++)
		m_wndPropList.GetProperty(i)->Show(FALSE, bAdjustLayout);

	m_pControl=NULL;
}

void CUIProperties::ShowProperty(CControlUI* pControl)
{
	if(m_pControl == pControl)
		return;

	if(pControl == NULL)
	{
		HideAllProperties(TRUE);
		return;
	}

	HideAllProperties();

	m_wndPropList.SetCurSel(NULL, FALSE);

	ExtendedAttributes mDummy;
	ExtendedAttributes* pExtended=(ExtendedAttributes*)pControl->GetTag();
	if (pExtended == NULL)
	{
		pExtended = &mDummy;
		ZeroMemory(pExtended, sizeof(ExtendedAttributes));
		LPCTSTR pstrClass = pControl->GetClass( );
		SIZE_T cchLen = _tcslen(pstrClass);
		switch (cchLen)
		{
		case 16:
			if (_tcscmp(_T("ListHeaderItemUI"), pstrClass) == 0) pExtended->nClass = classListHeaderItem;
			break;
		case 12:
			if (_tcscmp(_T("ListHeaderUI"), pstrClass) == 0) pExtended->nClass = classListHeader;
			break;
		}
	}

	switch(pExtended->nClass)
	{
	case classWindow:
		ShowWindowProperty(pControl);
		break;
	case classControl:
		ShowControlProperty(pControl);
		break;
	case classLabel:
	case classText:
		ShowLabelProperty(pControl);
		break;
	case classButton:
		ShowButtonProperty(pControl);
		break;
	case classEdit:
		ShowEditProperty(pControl);
		break;
	case classRichEdit:
		ShowRichEditProperty(pControl);
		break;
	case classOption:
	case classCheckBox:
		ShowOptionProperty(pControl);
		break;
	case classProgress:
		ShowProgressProperty(pControl);
		break;
	case classSlider:
		ShowSliderProperty(pControl);
		break;
	case classCombo: 
		ShowComboProperty(pControl);
		break;
	case classActiveX:
		ShowActiveXProperty(pControl);
		break;
	case classContainer:
		ShowContainerProperty(pControl);
		break;
	case classVerticalLayout:
		ShowVerticalLayoutProperty(pControl);
		break;
	case classHorizontalLayout:
		ShowHorizontalLayoutProperty(pControl);
		break;
	case classTileLayout:
		ShowTileLayoutProperty(pControl);
		break;
	case classTabLayout:
		ShowTabLayoutPropery(pControl);
		break;
	case classChildLayout:
		ShowChildLayoutProperty(pControl);
		break;
	case classList:
		ShowListProperty(pControl);
		break;
	case classListHeader:
		ShowListHeadProper(pControl);
		break;
	case classListHeaderItem:
		ShowListHeaderItemPropery(pControl);
		break;
	case classListTextElement:
	case classListLabelElement:
		ShowListElementProperty(pControl);
		break;
	case classListContainerElement:/*!*/
		ShowListContainerElementProperty(pControl);
		break;
	case classScrollBar:
		ShowScrollBarProperty(pControl);
		break;
	case classWebBrowser:
		ShowWebBrowserPropery(pControl);
		break;
	case classColorPalette:
		ShowColorPaletteProperty(pControl);
		break;
	case classTreeView:
		ShowTreeViewProperty(pControl);
		break;
	case classTreeNode:
		ShowTreeNodeProperty(pControl);
		break;
	case classFadeButton:
		ShowFadeButtonProperty(pControl);
		break;
	default:
		ShowControlProperty(pControl);
		break;
	}

	m_pControl=pControl;
	m_wndPropList.AdjustLayout();
}

void CUIProperties::ShowWindowProperty(CControlUI* pControl)
{
	ASSERT(pControl);
	CWindowUI* pForm=static_cast<CWindowUI*>(pControl->GetInterface(_T("Form")));
	ASSERT(pForm);

	CMFCPropertyGridProperty* pPropForm=m_wndPropList.FindItemByData(classWindow,FALSE);
	ASSERT(pPropForm);

	//size
	SIZE size=pForm->GetInitSize();
	CMFCPropertyGridProperty* pValueList=pPropForm->GetSubItem(tagWindowSize-tagWindow);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)size.cy);
	//caption
	RECT rect=pForm->GetCaptionRect();
	pValueList=pPropForm->GetSubItem(tagCaption-tagWindow);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);
	//sizebox
	rect=pForm->GetSizeBox();
	pValueList=pPropForm->GetSubItem(tagSizeBox-tagWindow);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);
	//roundcorner
	size=pForm->GetRoundCorner();
	pValueList=pPropForm->GetSubItem(tagRoundCorner-tagWindow);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)size.cy);
	//mininfo
	size=pForm->GetMinInfo();
	pValueList=pPropForm->GetSubItem(tagMinInfo-tagWindow);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)size.cy);
	//maxinfo
	size=pForm->GetMaxInfo();
	pValueList=pPropForm->GetSubItem(tagMaxInfo-tagWindow);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)size.cy);
	//showdirty
	pPropForm->GetSubItem(tagShowDirty-tagWindow)->SetValue((_variant_t)pForm->IsShowUpdateRect());
	pPropForm->GetSubItem(tagShowDirty-tagWindow)->SetOriginalValue((_variant_t)pForm->IsShowUpdateRect());

	// tagAlpha
	pPropForm->GetSubItem(tagAlpha-tagWindow)->SetValue((_variant_t)(LONG)pForm->GetAlpha());
	pPropForm->GetSubItem(tagAlpha-tagWindow)->SetOriginalValue((_variant_t)(LONG)pForm->GetAlpha());

	// tagBkTrans
	pPropForm->GetSubItem(tagBkTrans-tagWindow)->SetValue((_variant_t)pForm->GetBackgroundTransparent());
	pPropForm->GetSubItem(tagBkTrans-tagWindow)->SetOriginalValue((_variant_t)pForm->GetBackgroundTransparent());

	// tagDefaultFontColor
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(tagDefaultFontColor-tagWindow))->SetColor((_variant_t)(LONG)(pForm->GetDefaultFontColor()));
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(tagDefaultFontColor-tagWindow))->SetOriginalValue((_variant_t)(LONG)(pForm->GetDefaultFontColor()));

	// tagSelectedFontColor
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(tagSelectedFontColor-tagWindow))->SetColor((_variant_t)(LONG)(pForm->GetDefaultSelectedFontColor()));
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(tagSelectedFontColor-tagWindow))->SetOriginalValue((_variant_t)(LONG)(pForm->GetDefaultSelectedFontColor()));

	// tagDisabledFontColor
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(tagDisabledFontColor-tagWindow))->SetColor((_variant_t)(LONG)(pForm->GetDefaultDisabledFontColor()));
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(tagDisabledFontColor-tagWindow))->SetOriginalValue((_variant_t)(LONG)(pForm->GetDefaultDisabledFontColor()));

	// tagLinkFontColor
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(tagLinkFontColor-tagWindow))->SetColor((_variant_t)(LONG)(pForm->GetDefaultLinkFontColor()));
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(tagLinkFontColor-tagWindow))->SetOriginalValue((_variant_t)(LONG)(pForm->GetDefaultLinkFontColor()));

	// tagLinkHoverFontColor
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(tagLinkHoverFontColor-tagWindow))->SetColor((_variant_t)(LONG)(pForm->GetDefaultLinkHoverFontColor()));
	static_cast<CMFCPropertyGridColor32Property*>(pPropForm->GetSubItem(tagLinkHoverFontColor-tagWindow))->SetOriginalValue((_variant_t)(LONG)(pForm->GetDefaultLinkHoverFontColor()));

	pPropForm->Show(TRUE,FALSE);
}

void CUIProperties::ShowControlProperty(CControlUI* pControl)
{
	ASSERT(pControl);

	CMFCPropertyGridProperty* pPropControl=m_wndPropList.FindItemByData(classControl,FALSE);
	ASSERT(pPropControl);

	//name
	pPropControl->GetSubItem(tagName-tagControl)->SetValue((_variant_t)pControl->GetName());
	pPropControl->GetSubItem(tagName-tagControl)->SetOriginalValue((_variant_t)pControl->GetName());
	//tagVirtualWnd
	pPropControl->GetSubItem(tagVirtualWnd-tagControl)->SetValue((_variant_t)pControl->GetCurVirtualWnd());
	pPropControl->GetSubItem(tagVirtualWnd-tagControl)->SetOriginalValue((_variant_t)pControl->GetCurVirtualWnd());
	//text
	pPropControl->GetSubItem(tagText-tagControl)->SetValue((_variant_t)pControl->GetText());
	pPropControl->GetSubItem(tagText-tagControl)->SetOriginalValue((_variant_t)pControl->GetText());
	//pos
	SIZE szXY=pControl->GetFixedXY();
	int nWidth=pControl->GetFixedWidth();
	int nHeight=pControl->GetFixedHeight();
	CMFCPropertyGridProperty* pValueList=pPropControl->GetSubItem(tagPos-tagControl);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)szXY.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)szXY.cy);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)(szXY.cx+nWidth));
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)(szXY.cy+nHeight));
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)szXY.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)szXY.cy);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)(szXY.cx+nWidth));
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)(szXY.cy+nHeight));
	//relativepos
	TRelativePosUI posRelative=pControl->GetRelativePos();
	pValueList=pPropControl->GetSubItem(tagRelativePos-tagControl);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)posRelative.nMoveXPercent);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)posRelative.nMoveYPercent);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)posRelative.nZoomXPercent);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)posRelative.nZoomYPercent);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)posRelative.nMoveXPercent);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)posRelative.nMoveYPercent);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)posRelative.nZoomXPercent);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)posRelative.nZoomYPercent);
	//size
	pValueList=pPropControl->GetSubItem(tagSize-tagControl);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pControl->GetWidth());
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pControl->GetHeight());
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pControl->GetWidth());
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pControl->GetHeight());
	//minsize
	pValueList=pPropControl->GetSubItem(tagMinSize-tagControl);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pControl->GetMinWidth());
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pControl->GetMinHeight());
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pControl->GetMinWidth());
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pControl->GetMinHeight());
	//maxsize
	pValueList=pPropControl->GetSubItem(tagMaxSize-tagControl);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pControl->GetMaxWidth());
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pControl->GetMaxHeight());
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pControl->GetMaxWidth());
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pControl->GetMaxHeight());
	//padding
	pValueList=pPropControl->GetSubItem(tagPadding-tagControl);
	RECT rect=pControl->GetPadding();
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);
	//bkimage
	pPropControl->GetSubItem(tagBkImage-tagControl)->SetValue((_variant_t)pControl->GetBkImage());
	pPropControl->GetSubItem(tagBkImage-tagControl)->SetOriginalValue((_variant_t)pControl->GetBkImage());
	//bkcolor
	DWORD dwColor = pControl->GetBkColor();
	DWORD dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	//static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(tagBkColor-tagControl))->SetColor((_variant_t)(LONG)(pControl->GetBkColor()));
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(tagBkColor - tagControl))->SetColor((_variant_t)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(tagBkColor - tagControl))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);
	//bkcolor2
	dwColor = pControl->GetBkColor2();
	dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(tagBkColor2 - tagControl))->SetColor((_variant_t)(LONG)(dwARGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(tagBkColor2 - tagControl))->SetOriginalValue((_variant_t)(LONG)(dwARGBColor));
	//bordercolor
	dwColor = pControl->GetBorderColor();
	dwARGBColor = RGB(static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(tagBorderColor - tagControl))->SetColor((_variant_t)(LONG)(dwARGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(tagBorderColor - tagControl))->SetOriginalValue((_variant_t)(LONG)(dwARGBColor));
	//focusbordercolor
	dwColor = pControl->GetFocusBorderColor();
	dwARGBColor = RGB(static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));
	//dwARGBColor = ARGB(0, static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(tagFocusBorderColor - tagControl))->SetColor((_variant_t)(LONG)(dwARGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropControl->GetSubItem(tagFocusBorderColor - tagControl))->SetOriginalValue((_variant_t)(LONG)(dwARGBColor));
	//bordersize
	pPropControl->GetSubItem(tagBorderSize-tagControl)->SetValue((_variant_t)(LONG)pControl->GetBorderSize());
	pPropControl->GetSubItem(tagBorderSize-tagControl)->SetOriginalValue((_variant_t)(LONG)pControl->GetBorderSize());
	//borderround
	pValueList=pPropControl->GetSubItem(tagBorderRound-tagControl);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pControl->GetBorderRound().cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pControl->GetBorderRound().cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pControl->GetBorderRound().cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pControl->GetBorderRound().cy);
	//enabled
	pPropControl->GetSubItem(tagEnabled-tagControl)->SetValue((_variant_t)pControl->IsEnabled());
	pPropControl->GetSubItem(tagEnabled-tagControl)->SetOriginalValue((_variant_t)pControl->IsEnabled());
	//float
	pPropControl->GetSubItem(tagFloat-tagControl)->SetValue((_variant_t)pControl->IsFloat());
	pPropControl->GetSubItem(tagFloat-tagControl)->SetOriginalValue((_variant_t)pControl->IsFloat());
	//visible
	pPropControl->GetSubItem(tagVisible-tagControl)->SetValue((_variant_t)pControl->IsVisible());
	pPropControl->GetSubItem(tagVisible-tagControl)->SetOriginalValue((_variant_t)pControl->IsVisible());
	//mouse
	pPropControl->GetSubItem(tagMouse-tagControl)->SetValue((_variant_t)pControl->IsMouseEnabled());
	pPropControl->GetSubItem(tagMouse-tagControl)->SetOriginalValue((_variant_t)pControl->IsMouseEnabled());
	//menu
	pPropControl->GetSubItem(tagMenu-tagControl)->SetValue((_variant_t)pControl->IsContextMenuUsed());
	pPropControl->GetSubItem(tagMenu-tagControl)->SetOriginalValue((_variant_t)pControl->IsContextMenuUsed());
	//colorhsl
	pPropControl->GetSubItem(tagColorHSL-tagControl)->SetValue((_variant_t)pControl->IsColorHSL());
	pPropControl->GetSubItem(tagColorHSL-tagControl)->SetOriginalValue((_variant_t)pControl->IsColorHSL());
	//tooltip
	pPropControl->GetSubItem(tagTooltip-tagControl)->SetValue((_variant_t)pControl->GetToolTip());
	pPropControl->GetSubItem(tagTooltip-tagControl)->SetOriginalValue((_variant_t)pControl->GetToolTip());
	//userdata
	pPropControl->GetSubItem(tagUserData-tagControl)->SetValue((_variant_t)pControl->GetUserData());
	pPropControl->GetSubItem(tagUserData-tagControl)->SetOriginalValue((_variant_t)pControl->GetUserData());
	//keyboard
	pPropControl->GetSubItem(tagKeyBoard-tagControl)->SetValue((_variant_t)pControl->IsKeyboardEnabled());
	pPropControl->GetSubItem(tagKeyBoard-tagControl)->SetOriginalValue((_variant_t)pControl->IsKeyboardEnabled());

	pPropControl->Show(TRUE,FALSE);
}

void CUIProperties::ShowLabelProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CLabelUI* pLabel=static_cast<CLabelUI*>(pControl->GetInterface(_T("Label")));
	ASSERT(pLabel);

	CMFCPropertyGridProperty* pPropLabel=m_wndPropList.FindItemByData(classLabel,FALSE);
	ASSERT(pPropLabel);

	//align
	UINT uStyle=pLabel->GetTextStyle();
	CString strStyle;
	if(uStyle&DT_CENTER)
	{
		strStyle=_T("Center");
	}
	else if(uStyle&DT_RIGHT)
	{
		strStyle=_T("Right");
	}
	else
	{
		strStyle=_T("Left");
	}

	pPropLabel->GetSubItem(tagAlign-tagLabel)->SetValue((_variant_t)strStyle);
	pPropLabel->GetSubItem(tagAlign-tagLabel)->SetOriginalValue((_variant_t)strStyle);
	//valign
	CString strVStyle;
	//DT_TOP=DT_LEFT=0;
	if(uStyle&DT_VCENTER)
	{
		strVStyle=_T("Center");
	}
	else if(uStyle&DT_BOTTOM)
	{
		strVStyle=_T("Bottom");
	}
	else //(uStyle&DT_TOP)
	{
		strVStyle=_T("Top");
	}

	pPropLabel->GetSubItem(tagVAlign-tagLabel)->SetValue((_variant_t)strVStyle);
	pPropLabel->GetSubItem(tagVAlign-tagLabel)->SetOriginalValue((_variant_t)strVStyle);
	//textcolor
	DWORD dwColor = pLabel->GetTextColor();
	DWORD dwRGBColor = RGB(static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropLabel->GetSubItem(tagTextColor - tagLabel))->SetColor((_variant_t)(LONG)(dwRGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropLabel->GetSubItem(tagTextColor-tagLabel))->SetOriginalValue((_variant_t)(LONG)(dwRGBColor));
	//disabledtextcolor
	dwColor = pLabel->GetDisabledTextColor();
	dwRGBColor = RGB(static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropLabel->GetSubItem(tagDisabledTextColor - tagLabel))->SetColor((_variant_t)(LONG)(dwRGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropLabel->GetSubItem(tagDisabledTextColor - tagLabel))->SetOriginalValue((_variant_t)(LONG)(dwRGBColor));
	//font
	pPropLabel->GetSubItem(tagFont-tagLabel)->SetValue((_variant_t)(LONG)pLabel->GetFont());
	pPropLabel->GetSubItem(tagFont-tagLabel)->SetOriginalValue((_variant_t)(LONG)pLabel->GetFont());
	//textpadding
	CMFCPropertyGridProperty* pValueList=pPropLabel->GetSubItem(tagTextPadding-tagLabel);
	RECT rect=pLabel->GetTextPadding();
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);
	//showhtml
	pPropLabel->GetSubItem(tagShowHtml-tagLabel)->SetValue((_variant_t)pLabel->IsShowHtml());
	pPropLabel->GetSubItem(tagShowHtml-tagLabel)->SetOriginalValue((_variant_t)pLabel->IsShowHtml());
	//endellipsis
	DWORD dwStyle=pLabel->GetTextStyle();
	bool bEndEllipsis=(dwStyle&DT_END_ELLIPSIS) !=0 ;
	pPropLabel->GetSubItem(tagEndEllipsis-tagLabel)->SetValue((_variant_t)bEndEllipsis);
	pPropLabel->GetSubItem(tagEndEllipsis-tagLabel)->SetOriginalValue((_variant_t)bEndEllipsis);
	//tagPathEllipsis
	dwStyle = pLabel->GetTextStyle();
	bool bPathEllipsis = (dwStyle&DT_PATH_ELLIPSIS) != 0;
	pPropLabel->GetSubItem(tagPathEllipsis - tagLabel)->SetValue((_variant_t)bPathEllipsis);
	pPropLabel->GetSubItem(tagPathEllipsis - tagLabel)->SetOriginalValue((_variant_t)bPathEllipsis);
	//enabledeffect
	bool bEnabledEffect=pLabel->GetEnabledEffect();
	pPropLabel->GetSubItem(tagEnabledEffect-tagLabel)->SetValue((_variant_t)bEnabledEffect);
	pPropLabel->GetSubItem(tagEnabledEffect-tagLabel)->SetOriginalValue((_variant_t)bEnabledEffect);

	pPropLabel->Show(TRUE,FALSE);
}

void CUIProperties::ShowButtonProperty(CControlUI* pControl)
{
	ShowLabelProperty(pControl);

	ASSERT(pControl);
	CButtonUI* pButton=static_cast<CButtonUI*>(pControl->GetInterface(_T("Button")));
	ASSERT(pButton);

	CMFCPropertyGridProperty* pPropButton=m_wndPropList.FindItemByData(classButton,FALSE);
	ASSERT(pPropButton);

	//normalimage
	pPropButton->GetSubItem(tagNormalImage-tagButton)->SetValue((_variant_t)pButton->GetNormalImage());
	pPropButton->GetSubItem(tagNormalImage-tagButton)->SetOriginalValue((_variant_t)pButton->GetNormalImage());
	//hotimage
	pPropButton->GetSubItem(tagHotImage-tagButton)->SetValue((_variant_t)pButton->GetHotImage());
	pPropButton->GetSubItem(tagHotImage-tagButton)->SetOriginalValue((_variant_t)pButton->GetHotImage());
	//pushedimage
	pPropButton->GetSubItem(tagPushedImage-tagButton)->SetValue((_variant_t)pButton->GetPushedImage());
	pPropButton->GetSubItem(tagPushedImage-tagButton)->SetOriginalValue((_variant_t)pButton->GetPushedImage());
	//focusedimage
	pPropButton->GetSubItem(tagFocusedImage-tagButton)->SetValue((_variant_t)pButton->GetFocusedImage());
	pPropButton->GetSubItem(tagFocusedImage-tagButton)->SetOriginalValue((_variant_t)pButton->GetFocusedImage());
	//disabledimage
	pPropButton->GetSubItem(tagDisabledImage-tagButton)->SetValue((_variant_t)pButton->GetDisabledImage());
	pPropButton->GetSubItem(tagDisabledImage-tagButton)->SetOriginalValue((_variant_t)pButton->GetDisabledImage());
	//	tagHotTextColor
	DWORD dwColor = pButton->GetHotTextColor();
	DWORD dwRGBColor = RGB(static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(tagHotTextColor - tagButton))->SetColor((_variant_t)(LONG)(dwRGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(tagHotTextColor - tagButton))->SetOriginalValue((_variant_t)(LONG)(dwRGBColor));
	//tagHotBKColor

	dwColor = pButton->GetHotBkColor();
	DWORD dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(tagHotBKColor - tagButton))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(tagHotBKColor - tagButton))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);

	//tagPushedTextColor

	dwColor = pButton->GetPushedTextColor();
	dwRGBColor = RGB(static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(tagPushedTextColor - tagButton))->SetColor((_variant_t)(LONG)(dwRGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(tagPushedTextColor - tagButton))->SetOriginalValue((_variant_t)(LONG)(dwRGBColor));
	//tagPushedBKColor

	dwColor = pButton->GetPushedBKColor();
	dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(tagPushedBKColor - tagButton))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(tagPushedBKColor - tagButton))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);

	//tagfocusedtextcolor
	dwColor = pButton->GetFocusedTextColor();
	dwRGBColor = RGB(static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(tagfocusedtextcolor - tagButton))->SetColor((_variant_t)(LONG)(dwRGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropButton->GetSubItem(tagfocusedtextcolor - tagButton))->SetOriginalValue((_variant_t)(LONG)(dwRGBColor));


	pPropButton->Show(TRUE,FALSE);
}

void CUIProperties::ShowEditProperty(CControlUI* pControl)
{
	ShowLabelProperty(pControl);

	ASSERT(pControl);
	CEditUI* pEdit=static_cast<CEditUI*>(pControl->GetInterface(_T("Edit")));
	ASSERT(pEdit);

	CMFCPropertyGridProperty* pPropEdit=m_wndPropList.FindItemByData(classEdit,FALSE);
	ASSERT(pPropEdit);

	//normalimage
	pPropEdit->GetSubItem(tagEditNormalImage-tagEdit)->SetValue((_variant_t)pEdit->GetNormalImage());
	pPropEdit->GetSubItem(tagEditNormalImage-tagEdit)->SetOriginalValue((_variant_t)pEdit->GetNormalImage());
	//hotimage
	pPropEdit->GetSubItem(tagEditHotImage-tagEdit)->SetValue((_variant_t)pEdit->GetHotImage());
	pPropEdit->GetSubItem(tagEditHotImage-tagEdit)->SetOriginalValue((_variant_t)pEdit->GetHotImage());
	//focusedimage
	pPropEdit->GetSubItem(tagEditFocusedImage-tagEdit)->SetValue((_variant_t)pEdit->GetFocusedImage());
	pPropEdit->GetSubItem(tagEditFocusedImage-tagEdit)->SetOriginalValue((_variant_t)pEdit->GetFocusedImage());
	//disabledimage
	pPropEdit->GetSubItem(tagEditDisabledImage-tagEdit)->SetValue((_variant_t)pEdit->GetDisabledImage());
	pPropEdit->GetSubItem(tagEditDisabledImage-tagEdit)->SetOriginalValue((_variant_t)pEdit->GetDisabledImage());
	//tagEditFloatPointOnly
	pPropEdit->GetSubItem(tagEditFloatPointOnly - tagEdit)->SetValue((_variant_t)pEdit->IsFloatPointOnly( ));
	pPropEdit->GetSubItem(tagEditFloatPointOnly - tagEdit)->SetOriginalValue((_variant_t)pEdit->IsFloatPointOnly( ));
	//NumberOnly
	pPropEdit->GetSubItem(tagNumberOnly-tagEdit)->SetValue((_variant_t)pEdit->IsNumberOnly());
	pPropEdit->GetSubItem(tagNumberOnly-tagEdit)->SetOriginalValue((_variant_t)pEdit->IsNumberOnly());
	//readonly
	pPropEdit->GetSubItem(tagReadOnly-tagEdit)->SetValue((_variant_t)pEdit->IsReadOnly());
	pPropEdit->GetSubItem(tagReadOnly-tagEdit)->SetOriginalValue((_variant_t)pEdit->IsReadOnly());
	//password
	pPropEdit->GetSubItem(tagPassword-tagEdit)->SetValue((_variant_t)pEdit->IsPasswordMode());
	pPropEdit->GetSubItem(tagPassword-tagEdit)->SetOriginalValue((_variant_t)pEdit->IsPasswordMode());
	//maxchar
	pPropEdit->GetSubItem(tagMaxChar-tagEdit)->SetValue((_variant_t)(LONG)pEdit->GetMaxChar());
	pPropEdit->GetSubItem(tagMaxChar-tagEdit)->SetOriginalValue((_variant_t)(LONG)pEdit->GetMaxChar());
	//nativebkcolor
	DWORD dwColor = pEdit->GetNativeEditBkColor();
	DWORD dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropEdit->GetSubItem(tagNativeBKColor - tagEdit))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropEdit->GetSubItem(tagNativeBKColor - tagEdit))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);

	pPropEdit->Show(TRUE,FALSE);
}

void CUIProperties::ShowRichEditProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);
	ASSERT(pControl);
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(pControl->GetInterface(_T("RichEdit")));
	ASSERT(pRichEdit);

	CMFCPropertyGridProperty* pPropRichEdit = m_wndPropList.FindItemByData(classRichEdit, FALSE);
	ASSERT(pPropRichEdit);

	//vscroll
	bool bIsTrue = (WS_VSCROLL&pRichEdit->GetWinStyle());
	pPropRichEdit->GetSubItem(tagREVscrollBar - tagRichEdit)->SetValue((_variant_t)bIsTrue);
	pPropRichEdit->GetSubItem(tagREVscrollBar - tagRichEdit)->SetOriginalValue((_variant_t)bIsTrue);
	//autovscrollbar
	bIsTrue = (ES_AUTOVSCROLL&pRichEdit->GetWinStyle());
	pPropRichEdit->GetSubItem(tagREAutoVscroll - tagRichEdit)->SetValue((_variant_t)bIsTrue);
	pPropRichEdit->GetSubItem(tagREAutoVscroll - tagRichEdit)->SetOriginalValue((_variant_t)bIsTrue);
	//hscrollbar
	bIsTrue = (WS_HSCROLL&pRichEdit->GetWinStyle());
	pPropRichEdit->GetSubItem(tagREHscrollBar - tagRichEdit)->SetValue((_variant_t)bIsTrue);
	pPropRichEdit->GetSubItem(tagREHscrollBar - tagRichEdit)->SetOriginalValue((_variant_t)bIsTrue);
	//autohscroll
	bIsTrue = (ES_AUTOHSCROLL&pRichEdit->GetWinStyle());
	pPropRichEdit->GetSubItem(tagREAutoHscroll - tagRichEdit)->SetValue((_variant_t)bIsTrue);
	pPropRichEdit->GetSubItem(tagREAutoHscroll - tagRichEdit)->SetOriginalValue((_variant_t)bIsTrue);
	//
	//textpadding
	CMFCPropertyGridProperty* pValueList=pPropRichEdit->GetSubItem(tagRETextPadding-tagRichEdit);
	RECT rect=pRichEdit->GetTextPadding();
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);

	//wanttab
	pPropRichEdit->GetSubItem(tagREWantTab - tagRichEdit)->SetValue((_variant_t)pRichEdit->IsWantTab());
	pPropRichEdit->GetSubItem(tagREWantTab - tagRichEdit)->SetOriginalValue((_variant_t)pRichEdit->IsWantTab());
	//wantreturn 
	pPropRichEdit->GetSubItem(tagREWantReturn - tagRichEdit)->SetValue((_variant_t)pRichEdit->IsWantReturn());
	pPropRichEdit->GetSubItem(tagREWantReturn - tagRichEdit)->SetOriginalValue((_variant_t)pRichEdit->IsWantReturn());
	//WantCtrlReturn
	pPropRichEdit->GetSubItem(tagREWantCtrlReturn - tagRichEdit)->SetValue((_variant_t)pRichEdit->IsWantCtrlReturn());
	pPropRichEdit->GetSubItem(tagREWantCtrlReturn - tagRichEdit)->SetOriginalValue((_variant_t)pRichEdit->IsWantCtrlReturn());
	//rich
	pPropRichEdit->GetSubItem(tagRERich - tagRichEdit)->SetValue((_variant_t)pRichEdit->IsRich());
	pPropRichEdit->GetSubItem(tagRERich - tagRichEdit)->SetOriginalValue((_variant_t)pRichEdit->IsRich());
	//mutiline
	bIsTrue = (ES_MULTILINE&pRichEdit->GetWinStyle());
	pPropRichEdit->GetSubItem(tagREMultiline - tagRichEdit)->SetValue((_variant_t)bIsTrue);
	pPropRichEdit->GetSubItem(tagREMultiline - tagRichEdit)->SetOriginalValue((_variant_t)bIsTrue);
	//readonly
	bIsTrue = pRichEdit->IsReadOnly();
	pPropRichEdit->GetSubItem(tagREReadonly - tagRichEdit)->SetValue((_variant_t)bIsTrue);
	pPropRichEdit->GetSubItem(tagREReadonly - tagRichEdit)->SetOriginalValue((_variant_t)bIsTrue);
	//passord
	bIsTrue = (ES_PASSWORD&pRichEdit->GetWinStyle());
	pPropRichEdit->GetSubItem(tagREPassword - tagRichEdit)->SetValue((_variant_t)bIsTrue);
	pPropRichEdit->GetSubItem(tagREPassword - tagRichEdit)->SetOriginalValue((_variant_t)bIsTrue);
	//align
	UINT uStyle = pRichEdit->GetWinStyle();
	CString strStyle;
	if (uStyle&DT_CENTER)
	{
		strStyle = _T("Center");
	}
	else if (uStyle&DT_RIGHT)
	{
		strStyle = _T("Right");
	}
	else
	{
		strStyle = _T("Left");
	}
	pPropRichEdit->GetSubItem(tagREAlign - tagRichEdit)->SetValue((_variant_t)strStyle);
	pPropRichEdit->GetSubItem(tagREAlign - tagRichEdit)->SetOriginalValue((_variant_t)strStyle);
	//font
	pPropRichEdit->GetSubItem(tagREFont - tagRichEdit)->SetValue((_variant_t)(LONG)pRichEdit->GetFont());
	pPropRichEdit->GetSubItem(tagREFont - tagRichEdit)->SetOriginalValue((_variant_t)(LONG)pRichEdit->GetFont());
	//textcolor
	DWORD dwColor = pRichEdit->GetTextColor();
	DWORD dwARGBColor = RGB(static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));
	static_cast<CMFCPropertyGridColor32Property*>(pPropRichEdit->GetSubItem(tagRETextColor - tagRichEdit))->SetColor((_variant_t)(LONG)(dwARGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropRichEdit->GetSubItem(tagRETextColor - tagRichEdit))->SetOriginalValue((_variant_t)(LONG)(dwARGBColor));


	pPropRichEdit->Show(TRUE, FALSE);
}

void CUIProperties::ShowOptionProperty(CControlUI* pControl)
{
	ShowButtonProperty(pControl);

	ASSERT(pControl);
	COptionUI* pOption=static_cast<COptionUI*>(pControl->GetInterface(_T("Option")));
	ASSERT(pOption);

	CMFCPropertyGridProperty* pPropOption=m_wndPropList.FindItemByData(classOption,FALSE);
	ASSERT(pPropOption);



	//selectedimage
	pPropOption->GetSubItem(tagSelectedImage-tagOption)->SetValue((_variant_t)pOption->GetSelectedImage());
	pPropOption->GetSubItem(tagSelectedImage-tagOption)->SetOriginalValue((_variant_t)pOption->GetSelectedImage());

	//tagOptSelectedHotImage

	pPropOption->GetSubItem(tagOptSelectedHotImage - tagOption)->SetValue((_variant_t)pOption->GetSelectedHotImage( ));
	pPropOption->GetSubItem(tagOptSelectedHotImage - tagOption)->SetOriginalValue((_variant_t)pOption->GetSelectedHotImage( ));


	//tagOptSelectedPushImage

	pPropOption->GetSubItem(tagOptSelectedPushImage - tagOption)->SetValue((_variant_t)pOption->GetSelectedPushedImage( ));
	pPropOption->GetSubItem(tagOptSelectedPushImage - tagOption)->SetOriginalValue((_variant_t)pOption->GetSelectedPushedImage( ));



	//selectedtextcolor

	DWORD dwColor = pOption->GetSelectedTextColor();
	DWORD dwRGBColor = RGB(static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));
	static_cast<CMFCPropertyGridColor32Property*>(pPropOption->GetSubItem(tagSelectedTextColor - tagOption))->SetColor((_variant_t)(LONG)(dwRGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropOption->GetSubItem(tagSelectedTextColor - tagOption))->SetOriginalValue((_variant_t)(LONG)(dwRGBColor));

	//selectedbkcolor
	dwColor = pOption->GetSelectBkColor();
	DWORD dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropOption->GetSubItem(tagSelectedBkColor - tagOption))->SetColor((_variant_t)(LONG)(dwARGBColor));
	static_cast<CMFCPropertyGridColor32Property*>(pPropOption->GetSubItem(tagSelectedBkColor - tagOption))->SetOriginalValue((_variant_t)(LONG)(dwARGBColor));


	//selected
	pPropOption->GetSubItem(tagSelected-tagOption)->SetValue((_variant_t)pOption->IsSelected());
	pPropOption->GetSubItem(tagSelected-tagOption)->SetOriginalValue((_variant_t)pOption->IsSelected());
	//group
	pPropOption->GetSubItem(tagGroup-tagOption)->SetValue((_variant_t)pOption->GetGroup());
	pPropOption->GetSubItem(tagGroup-tagOption)->SetOriginalValue((_variant_t)pOption->GetGroup());

	pPropOption->Show(TRUE,FALSE);
}

void CUIProperties::ShowProgressProperty(CControlUI* pControl)
{
	ShowLabelProperty(pControl);

	ASSERT(pControl);
	CProgressUI* pProgress=static_cast<CProgressUI*>(pControl->GetInterface(_T("Progress")));
	ASSERT(pProgress);

	CMFCPropertyGridProperty* pPropProgress=m_wndPropList.FindItemByData(classProgress,FALSE);
	ASSERT(pPropProgress);

	//foreimage
	pPropProgress->GetSubItem(tagForeImage-tagProgress)->SetValue((_variant_t)pProgress->GetForeImage());
	pPropProgress->GetSubItem(tagForeImage-tagProgress)->SetOriginalValue((_variant_t)pProgress->GetForeImage());
	//minmax
	CMFCPropertyGridProperty* pValueList=pPropProgress->GetSubItem(tagMinMax-tagProgress);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pProgress->GetMinValue());
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pProgress->GetMaxValue());
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pProgress->GetMinValue());
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pProgress->GetMaxValue());
	//value
	pPropProgress->GetSubItem(tagValue-tagProgress)->SetValue((_variant_t)(LONG)pProgress->GetValue());
	pPropProgress->GetSubItem(tagValue-tagProgress)->SetOriginalValue((_variant_t)(LONG)pProgress->GetValue());
	//hor
	pPropProgress->GetSubItem(tagHor-tagProgress)->SetValue((_variant_t)pProgress->IsHorizontal());
	pPropProgress->GetSubItem(tagHor-tagProgress)->SetOriginalValue((_variant_t)pProgress->IsHorizontal());
	//tagIsStretchFore
	pPropProgress->GetSubItem(tagIsStretchFore-tagProgress)->SetValue((_variant_t)pProgress->IsStretchForeImage());
	pPropProgress->GetSubItem(tagIsStretchFore-tagProgress)->SetOriginalValue((_variant_t)pProgress->IsStretchForeImage());

	pPropProgress->Show(TRUE,FALSE);
}

void CUIProperties::ShowSliderProperty(CControlUI* pControl)
{
	ShowProgressProperty(pControl);

	ASSERT(pControl);
	CSliderUI* pSlider=static_cast<CSliderUI*>(pControl->GetInterface(_T("Slider")));
	ASSERT(pSlider);

	CMFCPropertyGridProperty* pPropSlider=m_wndPropList.FindItemByData(classSlider,FALSE);
	ASSERT(pPropSlider);

	//tagSendMoveNotify
	pPropSlider->GetSubItem(tagSendMoveNotify-tagSlider)->SetValue((_variant_t)pSlider->GetSendMoveNotify());
	pPropSlider->GetSubItem(tagSendMoveNotify-tagSlider)->SetOriginalValue((_variant_t)pSlider->GetSendMoveNotify());
	//thumbimage
	pPropSlider->GetSubItem(tagThumbImage-tagSlider)->SetValue((_variant_t)pSlider->GetThumbImage());
	pPropSlider->GetSubItem(tagThumbImage-tagSlider)->SetOriginalValue((_variant_t)pSlider->GetThumbImage());
	//thumbhotimage
	pPropSlider->GetSubItem(tagThumbHotImage-tagSlider)->SetValue((_variant_t)pSlider->GetThumbHotImage());
	pPropSlider->GetSubItem(tagThumbHotImage-tagSlider)->SetOriginalValue((_variant_t)pSlider->GetThumbHotImage());
	//thumbpushedimage
	pPropSlider->GetSubItem(tagThumbPushedImage-tagSlider)->SetValue((_variant_t)pSlider->GetThumbPushedImage());
	pPropSlider->GetSubItem(tagThumbPushedImage-tagSlider)->SetOriginalValue((_variant_t)pSlider->GetThumbPushedImage());
	//thumbsize
	CMFCPropertyGridProperty* pValueList=pPropSlider->GetSubItem(tagThumbSize-tagSlider);
	RECT rect=pSlider->GetThumbRect();
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)(rect.right-rect.left));
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)(rect.bottom-rect.top));
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)(rect.right-rect.left));
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)(rect.bottom-rect.top));

	pPropSlider->Show(TRUE,FALSE);
}

void CUIProperties::ShowComboProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);
	ShowItemProperty(pControl);

	ASSERT(pControl);
	CComboUI* pCombo=static_cast<CComboUI*>(pControl->GetInterface(_T("Combo")));
	ASSERT(pCombo);

	CMFCPropertyGridProperty* pPropCombo=m_wndPropList.FindItemByData(classCombo,FALSE);
	ASSERT(pPropCombo);

	//normalimage
	pPropCombo->GetSubItem(tagComboNormalImage-tagCombo)->SetValue((_variant_t)pCombo->GetNormalImage());
	pPropCombo->GetSubItem(tagComboNormalImage-tagCombo)->SetOriginalValue((_variant_t)pCombo->GetNormalImage());
	//hotimage
	pPropCombo->GetSubItem(tagComboHotImage-tagCombo)->SetValue((_variant_t)pCombo->GetHotImage());
	pPropCombo->GetSubItem(tagComboHotImage-tagCombo)->SetOriginalValue((_variant_t)pCombo->GetHotImage());
	//pushedimage
	pPropCombo->GetSubItem(tagComboPushedImage-tagCombo)->SetValue((_variant_t)pCombo->GetPushedImage());
	pPropCombo->GetSubItem(tagComboPushedImage-tagCombo)->SetOriginalValue((_variant_t)pCombo->GetPushedImage());
	//focusedimage
	pPropCombo->GetSubItem(tagComboFocusedImage-tagCombo)->SetValue((_variant_t)pCombo->GetFocusedImage());
	pPropCombo->GetSubItem(tagComboFocusedImage-tagCombo)->SetOriginalValue((_variant_t)pCombo->GetFocusedImage());
	//disabledimage
	pPropCombo->GetSubItem(tagComboDisabledImage-tagCombo)->SetValue((_variant_t)pCombo->GetDisabledImage());
	pPropCombo->GetSubItem(tagComboDisabledImage-tagCombo)->SetOriginalValue((_variant_t)pCombo->GetDisabledImage());
	//textpadding
	RECT rect=pCombo->GetTextPadding();
	CMFCPropertyGridProperty* pValueList=pPropCombo->GetSubItem(tagComboTextPadding-tagCombo);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);
	//dropboxsize
	SIZE size=pCombo->GetDropBoxSize();
	pValueList=pPropCombo->GetSubItem(tagComboDropBoxSize-tagCombo);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)size.cy);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)size.cx);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)size.cy);
	pPropCombo->Show(TRUE,FALSE);
}

void CUIProperties::ShowActiveXProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CActiveXUI* pActiveX=static_cast<CActiveXUI*>(pControl->GetInterface(_T("ActiveX")));
	ASSERT(pActiveX);

	CMFCPropertyGridProperty* pPropActiveX=m_wndPropList.FindItemByData(classActiveX,FALSE);
	ASSERT(pPropActiveX);

	CLSID clsid=pActiveX->GetClisd();
	TCHAR strCLSID[48];
	StringFromGUID2(clsid,strCLSID,48);
	//clsid
	pPropActiveX->GetSubItem(tagClsid-tagActiveX)->SetValue((_variant_t)strCLSID);
	pPropActiveX->GetSubItem(tagClsid-tagActiveX)->SetOriginalValue((_variant_t)strCLSID);

	//delaycreate
	pPropActiveX->GetSubItem(tagDelayCreate-tagActiveX)->SetValue((_variant_t)pActiveX->IsDelayCreate());
	pPropActiveX->GetSubItem(tagDelayCreate-tagActiveX)->SetOriginalValue((_variant_t)pActiveX->IsDelayCreate());

	//modulename
	pPropActiveX->GetSubItem(tagModuleName-tagActiveX)->SetValue((_variant_t)pActiveX->GetModuleName());
	pPropActiveX->GetSubItem(tagModuleName-tagActiveX)->SetOriginalValue((_variant_t)pActiveX->GetModuleName());

	pPropActiveX->Show(TRUE,FALSE);
}

void CUIProperties::ShowContainerProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CContainerUI* pContainer=static_cast<CContainerUI*>(pControl->GetInterface(_T("Container")));
	ASSERT(pContainer);

	CMFCPropertyGridProperty* pPropContainer=m_wndPropList.FindItemByData(classContainer,FALSE);
	ASSERT(pPropContainer);

	//inset
	RECT rect=pContainer->GetInset();
	CMFCPropertyGridProperty* pValueList=pPropContainer->GetSubItem(tagInset-tagContainer);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)rect.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)rect.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)rect.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)rect.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)rect.bottom);
	//childpadding
	pPropContainer->GetSubItem(tagChildPadding-tagContainer)->SetValue((_variant_t)(LONG)pContainer->GetChildPadding());
	pPropContainer->GetSubItem(tagChildPadding-tagContainer)->SetOriginalValue((_variant_t)(LONG)pContainer->GetChildPadding());
	//mousechild
	pPropContainer->GetSubItem(tagMouseChild-tagContainer)->SetValue((_variant_t)pContainer->IsMouseChildEnabled());
	pPropContainer->GetSubItem(tagMouseChild-tagContainer)->SetOriginalValue((_variant_t)pContainer->IsMouseChildEnabled());
	//hscrollbar
	pPropContainer->GetSubItem(tagHScrollBar-tagContainer)->SetValue((_variant_t)(pContainer->GetHorizontalScrollBar()==NULL?false:true));
	pPropContainer->GetSubItem(tagHScrollBar-tagContainer)->SetOriginalValue((_variant_t)(pContainer->GetHorizontalScrollBar()==NULL?false:true));
	//vscrollbar
	pPropContainer->GetSubItem(tagVScrollBar - tagContainer)->SetValue((_variant_t)(pContainer->GetVerticalScrollBar() == NULL ? false : true));
	pPropContainer->GetSubItem(tagVScrollBar - tagContainer)->SetOriginalValue((_variant_t)(pContainer->GetVerticalScrollBar() == NULL ? false : true));

	pPropContainer->Show(TRUE,FALSE);
}

void CUIProperties::ShowHorizontalLayoutProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CHorizontalLayoutUI* pHorizontalLayout=static_cast<CHorizontalLayoutUI*>(pControl->GetInterface(_T("HorizontalLayout")));
	ASSERT(pHorizontalLayout);

	CMFCPropertyGridProperty* pPropHorizontalLayout=m_wndPropList.FindItemByData(classHorizontalLayout,FALSE);
	ASSERT(pPropHorizontalLayout);

	//sepwidth
	pPropHorizontalLayout->GetSubItem(tagSepWidth-tagHorizontalLayout)->SetValue((_variant_t)(LONG)pHorizontalLayout->GetSepWidth());
	pPropHorizontalLayout->GetSubItem(tagSepWidth-tagHorizontalLayout)->SetOriginalValue((_variant_t)(LONG)pHorizontalLayout->GetSepWidth());
	//sepimm
	pPropHorizontalLayout->GetSubItem(tagHLSepImm-tagHorizontalLayout)->SetValue((_variant_t)pHorizontalLayout->IsSepImmMode());
	pPropHorizontalLayout->GetSubItem(tagHLSepImm-tagHorizontalLayout)->SetOriginalValue((_variant_t)pHorizontalLayout->IsSepImmMode());

	pPropHorizontalLayout->Show(TRUE,FALSE);
}

void CUIProperties::ShowVerticalLayoutProperty(CControlUI* pControl)
{

	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CVerticalLayoutUI* pVerticalLayoutUI=static_cast<CVerticalLayoutUI*>(pControl->GetInterface(DUI_CTR_VERTICALLAYOUT));
	ASSERT(pVerticalLayoutUI);

	CMFCPropertyGridProperty* pPropVerticalLayout=m_wndPropList.FindItemByData(classVerticalLayout,FALSE);
	ASSERT(pPropVerticalLayout);

	//sepheight
	pPropVerticalLayout->GetSubItem(tagSepHeight-tagVerticalLayout)->SetValue((_variant_t)(LONG)pVerticalLayoutUI->GetSepHeight());
	pPropVerticalLayout->GetSubItem(tagSepHeight-tagVerticalLayout)->SetOriginalValue((_variant_t)(LONG)pVerticalLayoutUI->GetHeight());
	//sepimm
	pPropVerticalLayout->GetSubItem(tagVLSepImm-tagVerticalLayout)->SetValue((_variant_t)pVerticalLayoutUI->IsSepImmMode());
	pPropVerticalLayout->GetSubItem(tagVLSepImm-tagVerticalLayout)->SetOriginalValue((_variant_t)pVerticalLayoutUI->IsSepImmMode());

	pPropVerticalLayout->Show(TRUE,FALSE);

}

void CUIProperties::ShowTileLayoutProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CTileLayoutUI* pTileLayout=static_cast<CTileLayoutUI*>(pControl->GetInterface(_T("TileLayout")));
	ASSERT(pTileLayout);

	CMFCPropertyGridProperty* pPropTileLayout=m_wndPropList.FindItemByData(classTileLayout,FALSE);
	ASSERT(pPropTileLayout);

	//columns 
	pPropTileLayout->GetSubItem(tagColumns-tagTileLayout)->SetValue((_variant_t)(LONG)pTileLayout->GetColumns());
	pPropTileLayout->GetSubItem(tagColumns-tagTileLayout)->SetOriginalValue((_variant_t)(LONG)pTileLayout->GetColumns());

	pPropTileLayout->Show(TRUE,FALSE);
}

void CUIProperties::ShowChildLayoutProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CChildLayoutUI* pChildLayout=static_cast<CChildLayoutUI*>(pControl->GetInterface(_T("ChildLayout")));
	ASSERT(pChildLayout);

	CMFCPropertyGridProperty* pPropTileLayout=m_wndPropList.FindItemByData(classChildLayout,FALSE);
	ASSERT(pPropTileLayout);

	//columns 
	pPropTileLayout->GetSubItem(tagCLXMLFile-tagChildLayout)->SetValue(pChildLayout->GetChildLayoutXML().GetData());
	pPropTileLayout->GetSubItem(tagCLXMLFile-tagChildLayout)->SetOriginalValue(pChildLayout->GetChildLayoutXML().GetData());

	pPropTileLayout->Show(TRUE,FALSE);


}

CMFCPropertyGridProperty* CUIProperties::FindPropByData(DWORD_PTR dwData, BOOL bSearchSubProps/* = TRUE*/) const
{
	return m_wndPropList.FindItemByData(dwData, bSearchSubProps);
}

CString CUIProperties::FormatOrigProperty(CMFCPropertyGridProperty* pProp)
{
	ASSERT(pProp);

	COleVariant var = pProp->GetOriginalValue();
	CString strOrigVal;

	int nCount = pProp->GetSubItemsCount();
	if(nCount > 0)
	{
		for (int i=0; i<nCount; i++)
		{
			CMFCPropertyGridProperty* pSubProp = pProp->GetSubItem(i);
			ASSERT_VALID(pSubProp);

			strOrigVal += FormatOrigProperty(pSubProp);

			if (i != nCount-1)
			{
				strOrigVal += _T(',');
				strOrigVal += _T(' ');
			}
		}

		return strOrigVal;
	}

	switch (var.vt)
	{
	case VT_BSTR:
		strOrigVal = var.bstrVal;
		break;

	case VT_I2:
		strOrigVal.Format(_T("%d"), (short)var.iVal);
		break;

	case VT_I4:
	case VT_INT:
		strOrigVal.Format(_T("%ld"), (long)var.lVal);
		break;

	case VT_UI1:
		if ((BYTE)var.bVal != 0)
		{
			strOrigVal.Format(_T("%c"), (TCHAR)(BYTE)var.bVal);
		}
		break;

	case VT_UI2:
		strOrigVal.Format(_T("%u"), var.uiVal);
		break;

	case VT_UINT:
	case VT_UI4:
		strOrigVal.Format(_T("%u"), var.ulVal);
		break;

	case VT_R4:
		strOrigVal.Format(_T("%f"), (float)var.fltVal);
		break;

	case VT_R8:
		strOrigVal.Format(_T("%lf"), (double)var.dblVal);
		break;

	case VT_BOOL:
		strOrigVal = var.boolVal == VARIANT_TRUE ? _T("True") : _T("False");
		break;

	default:
		// Unsupported type
		strOrigVal = _T("*** error ***");
	}

	return strOrigVal;
}

void CUIProperties::ShowListProperty( CControlUI* pControl )
{
	ShowContainerProperty(pControl);
	ShowItemProperty(pControl);

	ASSERT(pControl);
	CListUI* pList=static_cast<CListUI*>(pControl->GetInterface(_T("List")));
	ASSERT(pList);

	CMFCPropertyGridProperty* pPropList=m_wndPropList.FindItemByData(classList,FALSE);
	ASSERT(pPropList);

	//Header
	pPropList->GetSubItem(tagListHeader-tagList)->SetValue((_variant_t)pList->GetHeader()->IsVisible());
	pPropList->GetSubItem(tagListHeader-tagList)->SetOriginalValue((_variant_t)pList->GetHeader()->IsVisible());

	pPropList->GetSubItem(tagListHeaderBKImage-tagList)->SetValue((_variant_t)pList->GetHeader()->GetBkImage());
	pPropList->GetSubItem(tagListHeaderBKImage-tagList)->SetOriginalValue((_variant_t)pList->GetHeader()->GetBkImage());

	pPropList->Show(TRUE,FALSE);
}

void CUIProperties::ShowListHeadProper(CControlUI* pControl)
{
	ShowHorizontalLayoutProperty(pControl);
}

void CUIProperties::ShowItemProperty(CControlUI* pControl)
{
	ASSERT(pControl);
	IListOwnerUI* pList = static_cast<IListOwnerUI*>(pControl->GetInterface(_T("IListOwner")));
	ASSERT(pList);

	CMFCPropertyGridProperty* pPropItem = m_wndPropList.FindItemByData(classItem, FALSE);
	ASSERT(pPropItem);

	//itembkimage
	TListInfoUI* pListInfo = pList->GetListInfo();
	pPropItem->GetSubItem(tagItemBkImage - tagItem)->SetValue((_variant_t)pListInfo->sBkImage);
	pPropItem->GetSubItem(tagItemBkImage - tagItem)->SetOriginalValue((_variant_t)pListInfo->sBkImage);
	//itemhotimage
	pPropItem->GetSubItem(tagItemHotImage - tagItem)->SetValue((_variant_t)pListInfo->sHotImage);
	pPropItem->GetSubItem(tagItemHotImage - tagItem)->SetOriginalValue((_variant_t)pListInfo->sHotImage);
	//itemselectedimage
	pPropItem->GetSubItem(tagItemSelectedImage - tagItem)->SetValue((_variant_t)pListInfo->sSelectedImage);
	pPropItem->GetSubItem(tagItemSelectedImage - tagItem)->SetOriginalValue((_variant_t)pListInfo->sSelectedImage);
	//itemdisabledimage
	pPropItem->GetSubItem(tagItemDisabledImage - tagItem)->SetValue((_variant_t)pListInfo->sDisabledImage);
	pPropItem->GetSubItem(tagItemDisabledImage - tagItem)->SetOriginalValue((_variant_t)pListInfo->sDisabledImage);
	//itemtextpadding
	CMFCPropertyGridProperty*  pValueList = pPropItem->GetSubItem(tagItemTextPadding - tagItem);
	pValueList->GetSubItem(0)->SetValue((_variant_t)(LONG)pListInfo->rcTextPadding.left);
	pValueList->GetSubItem(1)->SetValue((_variant_t)(LONG)pListInfo->rcTextPadding.top);
	pValueList->GetSubItem(2)->SetValue((_variant_t)(LONG)pListInfo->rcTextPadding.right);
	pValueList->GetSubItem(3)->SetValue((_variant_t)(LONG)pListInfo->rcTextPadding.bottom);
	pValueList->GetSubItem(0)->SetOriginalValue((_variant_t)(LONG)pListInfo->rcTextPadding.left);
	pValueList->GetSubItem(1)->SetOriginalValue((_variant_t)(LONG)pListInfo->rcTextPadding.top);
	pValueList->GetSubItem(2)->SetOriginalValue((_variant_t)(LONG)pListInfo->rcTextPadding.right);
	pValueList->GetSubItem(3)->SetOriginalValue((_variant_t)(LONG)pListInfo->rcTextPadding.bottom);
	//itemalign
	CString strStyle;
		
	if (pListInfo->m_uItemTextStyle&DT_CENTER)
	{
		strStyle = _T("Center");
	}
	else if (pListInfo->m_uItemTextStyle&DT_RIGHT)
	{
		strStyle = _T("Right");
	}
	else//DT_LEFT
	{
		strStyle = _T("Left");
	}
	pPropItem->GetSubItem(tagItemAlign-tagItem)->SetValue((_variant_t)strStyle);
	pPropItem->GetSubItem(tagItemAlign-tagItem)->SetOriginalValue((_variant_t)strStyle);
	//itemtextcolor
	DWORD dwColor = pListInfo->dwTextColor;
	DWORD dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemTextColor - tagItem))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemTextColor - tagItem))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);
	//itembkcolor
	dwColor = pListInfo->dwBkColor;
	dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemBkColor - tagItem))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemBkColor - tagItem))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);
	//itemselectedtextcolor
	dwColor = pListInfo->dwSelectedTextColor;
	dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemSelectedTextColor - tagItem))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemSelectedTextColor - tagItem))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);
	//itemselectedbkcolor
	dwColor = pListInfo->dwSelectedBkColor;
	dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemSelectedBkColor - tagItem))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemSelectedBkColor - tagItem))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);
	//itemhottextcolor
	dwColor = pListInfo->dwHotTextColor;
	dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemHotTextColor - tagItem))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemHotTextColor - tagItem))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);
	//itemhotbkcolor
	dwColor = pListInfo->dwHotBkColor;
	dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemHotBkColor - tagItem))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemHotBkColor - tagItem))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);
	//itemdisabledtextcolor
	dwColor = pListInfo->dwDisabledTextColor;
	dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemDisabledTextColor - tagItem))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemDisabledTextColor - tagItem))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);
	//itemdisabledbkcolor
	dwColor = pListInfo->dwDisabledBkColor;
	dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemDisabledBkColor - tagItem))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemDisabledBkColor - tagItem))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);
	//itemlinecolor
	dwColor = pListInfo->dwLineColor;
	dwARGBColor = ARGB(HIBYTE(HIWORD(dwColor)), static_cast<BYTE>(GetBValue(dwColor)), static_cast<BYTE>(GetGValue(dwColor)), static_cast<BYTE>(GetRValue(dwColor)));

	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemLineColor - tagItem))->SetColor((_variant_t)(LONG)dwARGBColor);
	static_cast<CMFCPropertyGridColor32Property*>(pPropItem->GetSubItem(tagItemLineColor - tagItem))->SetOriginalValue((_variant_t)(LONG)dwARGBColor);
	//itemfont
	pPropItem->GetSubItem(tagItemFont-tagItem)->SetValue((_variant_t)(LONG)pListInfo->nFont);
	pPropItem->GetSubItem(tagItemFont-tagItem)->SetOriginalValue((_variant_t)(LONG)pListInfo->nFont);
	//itemshowhtml
	pPropItem->GetSubItem(tagMultiExpanding-tagItem)->SetValue((_variant_t)pListInfo->bMultiExpandable);
	pPropItem->GetSubItem(tagMultiExpanding-tagItem)->SetOriginalValue((_variant_t)pListInfo->bMultiExpandable);

	pPropItem->Show(TRUE,FALSE);
}

void CUIProperties::ShowScrollBarProperty( CControlUI* pControl )
{
	//大部分时候只在默认属性中设置全局滚动条属性
	//默认不显示基础控件属性
	//ShowControlProperty(pControl);

	ASSERT(pControl);
	CScrollBarUI* pScrollBar=static_cast<CScrollBarUI*>(pControl->GetInterface(_T("ScrollBar")));
	ASSERT(pScrollBar);

	CMFCPropertyGridProperty* pPropItem=m_wndPropList.FindItemByData(classScrollBar,FALSE);
	ASSERT(pPropItem);

	//tagScrollBarRange
	pPropItem->GetSubItem(tagScrollBarRange-tagScrollBar)->SetValue((_variant_t)(LONG)pScrollBar->GetScrollRange());
	pPropItem->GetSubItem(tagScrollBarRange-tagScrollBar)->SetOriginalValue((_variant_t)(LONG)pScrollBar->GetScrollRange());
	//tagScrollBarValue
	pPropItem->GetSubItem(tagScrollBarValue-tagScrollBar)->SetValue((_variant_t)(LONG)pScrollBar->GetScrollRange());
	pPropItem->GetSubItem(tagScrollBarValue-tagScrollBar)->SetOriginalValue((_variant_t)(LONG)pScrollBar->GetScrollRange());
	//tagScrollBarLineSize
	pPropItem->GetSubItem(tagScrollBarLineSize-tagScrollBar)->SetValue((_variant_t)(LONG)pScrollBar->GetScrollRange());
	pPropItem->GetSubItem(tagScrollBarLineSize-tagScrollBar)->SetOriginalValue((_variant_t)(LONG)pScrollBar->GetScrollRange());
	// tagScrollBarShowButton1
	pPropItem->GetSubItem(tagScrollBarShowButton1-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetShowButton1());
	pPropItem->GetSubItem(tagScrollBarShowButton1-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetShowButton1());
	//	tagScrollBarShowButton2
	pPropItem->GetSubItem(tagScrollBarShowButton2-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetShowButton2());
	pPropItem->GetSubItem(tagScrollBarShowButton2-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetShowButton2());

	//	tagScrollBarButton1NormalImage
	pPropItem->GetSubItem(tagScrollBarButton1NormalImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton1NormalImage());
	pPropItem->GetSubItem(tagScrollBarButton1NormalImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton1NormalImage());
	//	tagScrollBarButton1HotImage
	pPropItem->GetSubItem(tagScrollBarButton1HotImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton1HotImage());
	pPropItem->GetSubItem(tagScrollBarButton1HotImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton1HotImage());
	//	tagScrollBarButton1PushedImage
	pPropItem->GetSubItem(tagScrollBarButton1PushedImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton1PushedImage());
	pPropItem->GetSubItem(tagScrollBarButton1PushedImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton1PushedImage());
	//	tagScrollBarButton1DisabledImage
	pPropItem->GetSubItem(tagScrollBarButton1DisabledImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton1DisabledImage());
	pPropItem->GetSubItem(tagScrollBarButton1DisabledImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton1DisabledImage());

	//	tagScrollBarButton2NormalImage
	pPropItem->GetSubItem(tagScrollBarButton2NormalImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton2NormalImage());
	pPropItem->GetSubItem(tagScrollBarButton2NormalImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton2NormalImage());
	//	tagScrollBarButton2HotImage
	pPropItem->GetSubItem(tagScrollBarButton2HotImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton2HotImage());
	pPropItem->GetSubItem(tagScrollBarButton2HotImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton2HotImage());
	//	tagScrollBarButton2PushedImage
	pPropItem->GetSubItem(tagScrollBarButton2PushedImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton2PushedImage());
	pPropItem->GetSubItem(tagScrollBarButton2PushedImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton2PushedImage());
	//	tagScrollBarButton2DisabledImage
	pPropItem->GetSubItem(tagScrollBarButton2DisabledImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetButton2DisabledImage());
	pPropItem->GetSubItem(tagScrollBarButton2DisabledImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton2DisabledImage());

	//	tagScrollBarThumbNormalImage
	pPropItem->GetSubItem(tagScrollBarThumbNormalImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetThumbNormalImage());
	pPropItem->GetSubItem(tagScrollBarThumbNormalImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetThumbNormalImage());
	//	tagScrollBarThumbHotImage
	pPropItem->GetSubItem(tagScrollBarThumbHotImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetThumbHotImage());
	pPropItem->GetSubItem(tagScrollBarThumbHotImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetThumbHotImage());
	//	tagScrollBarThumbPushedImage
	pPropItem->GetSubItem(tagScrollBarThumbPushedImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetThumbPushedImage());
	pPropItem->GetSubItem(tagScrollBarThumbPushedImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetThumbPushedImage());
	//	tagScrollBarThumbDisabledImage
	pPropItem->GetSubItem(tagScrollBarThumbDisabledImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetThumbDisabledImage());
	pPropItem->GetSubItem(tagScrollBarThumbDisabledImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetThumbDisabledImage());

	//	tagScrollBarRailNormalImage
	pPropItem->GetSubItem(tagScrollBarRailNormalImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetRailNormalImage());
	pPropItem->GetSubItem(tagScrollBarRailNormalImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetRailNormalImage());
	//	tagScrollBarRailHotImage
	pPropItem->GetSubItem(tagScrollBarRailHotImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetRailHotImage());
	pPropItem->GetSubItem(tagScrollBarRailHotImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetButton1HotImage());
	//	tagScrollBarRailPushedImage
	pPropItem->GetSubItem(tagScrollBarRailPushedImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetRailPushedImage());
	pPropItem->GetSubItem(tagScrollBarRailPushedImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetRailPushedImage());
	//	tagScrollBarRailDisabledImage
	pPropItem->GetSubItem(tagScrollBarRailDisabledImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetRailDisabledImage());
	pPropItem->GetSubItem(tagScrollBarRailDisabledImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetRailDisabledImage());

	//	tagScrollBarBKNormalImage
	pPropItem->GetSubItem(tagScrollBarBKNormalImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetBkNormalImage());
	pPropItem->GetSubItem(tagScrollBarBKNormalImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetBkNormalImage());
	//	tagScrollBarBKHotImage
	pPropItem->GetSubItem(tagScrollBarBKHotImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetBkHotImage());
	pPropItem->GetSubItem(tagScrollBarBKHotImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetBkHotImage());
	//	tagScrollBarBKPushedImage
	pPropItem->GetSubItem(tagScrollBarBKPushedImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetBkPushedImage());
	pPropItem->GetSubItem(tagScrollBarBKPushedImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetBkPushedImage());
	//	tagScrollBarBKDisabledImage
	pPropItem->GetSubItem(tagScrollBarBKDisabledImage-tagScrollBar)->SetValue((_variant_t)pScrollBar->GetBkDisabledImage());
	pPropItem->GetSubItem(tagScrollBarBKDisabledImage-tagScrollBar)->SetOriginalValue((_variant_t)pScrollBar->GetBkDisabledImage());

	pPropItem->Show(TRUE,FALSE);
}
void CUIProperties::ShowTabLayoutPropery( CControlUI* pControl )
{
	ShowContainerProperty(pControl);

	ASSERT(pControl);
	CTabLayoutUI* pTabLayout=static_cast<CTabLayoutUI*>(pControl->GetInterface(_T("TabLayout")));
	ASSERT(pTabLayout);

	CMFCPropertyGridProperty* pPropItem=m_wndPropList.FindItemByData(classTabLayout,FALSE);
	ASSERT(pPropItem);

	//	SelectedID
	pPropItem->GetSubItem(tagSelectedID-tagTabLayout)->SetValue((_variant_t)(LONG)pTabLayout->GetCurSel());
	pPropItem->GetSubItem(tagSelectedID-tagTabLayout)->SetOriginalValue((_variant_t)(LONG)pTabLayout->GetCurSel());

	pPropItem->Show(TRUE,FALSE);
}

void CUIProperties::ShowListHeaderItemPropery( CControlUI* pControl )
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CListHeaderItemUI* pListHeaderItem=static_cast<CListHeaderItemUI*>(pControl->GetInterface(_T("ListHeaderItem")));
	ASSERT(pListHeaderItem);

	CMFCPropertyGridProperty* pPropItem = m_wndPropList.FindItemByData(classListHeaderItem, FALSE);
	ASSERT(pPropItem);

	//	dragable
	pPropItem->GetSubItem(tagDragable - tagListHeaderItem)->SetValue((_variant_t)pListHeaderItem->IsDragable( ));
	pPropItem->GetSubItem(tagDragable - tagListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->IsDragable( ));
	//	sepwidth
	pPropItem->GetSubItem(tagListHeaderItemSepWidth - tagListHeaderItem)->SetValue((_variant_t)(LONG)pListHeaderItem->GetSepWidth( ));
	pPropItem->GetSubItem(tagListHeaderItemSepWidth - tagListHeaderItem)->SetOriginalValue((_variant_t)(LONG)pListHeaderItem->GetSepWidth( ));
	//	normalimage
	pPropItem->GetSubItem(tagListHeaderItemNormalImage - tagListHeaderItem)->SetValue((_variant_t)pListHeaderItem->GetNormalImage( ));
	pPropItem->GetSubItem(tagListHeaderItemNormalImage - tagListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->GetNormalImage( ));
	//	hotimage
	pPropItem->GetSubItem(tagListHeaderItemHotImage - tagListHeaderItem)->SetValue((_variant_t)pListHeaderItem->GetHotImage( ));
	pPropItem->GetSubItem(tagListHeaderItemHotImage - tagListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->GetHotImage( ));
	//	pushedimage
	pPropItem->GetSubItem(tagListHeaderItemPushedImage - tagListHeaderItem)->SetValue((_variant_t)pListHeaderItem->GetPushedImage( ));
	pPropItem->GetSubItem(tagListHeaderItemPushedImage - tagListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->GetPushedImage( ));
	//	focusedimage
	pPropItem->GetSubItem(tagListHeaderItemFocusedImage - tagListHeaderItem)->SetValue((_variant_t)pListHeaderItem->GetFocusedImage( ));
	pPropItem->GetSubItem(tagListHeaderItemFocusedImage - tagListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->GetFocusedImage( ));
	//	sepimage
	pPropItem->GetSubItem(tagSepImage - tagListHeaderItem)->SetValue((_variant_t)pListHeaderItem->GetSepImage( ));
	pPropItem->GetSubItem(tagSepImage - tagListHeaderItem)->SetOriginalValue((_variant_t)pListHeaderItem->GetSepImage( ));

	pPropItem->Show(TRUE,FALSE);
}

void CUIProperties::ShowWebBrowserPropery( CControlUI* pControl )
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CWebBrowserUI* pWebBrowser=static_cast<CWebBrowserUI*>(pControl->GetInterface(_T("WebBrowser")));
	ASSERT(pWebBrowser);

	CMFCPropertyGridProperty* pPropItem=m_wndPropList.FindItemByData(classWebBrowser,FALSE);
	ASSERT(pPropItem);

	//	homepage
	pPropItem->GetSubItem(tagWebBrowserHomePage-tagWebBrowser)->SetValue((_variant_t)pWebBrowser->GetHomePage());
	pPropItem->GetSubItem(tagWebBrowserHomePage-tagWebBrowser)->SetOriginalValue((_variant_t)pWebBrowser->GetHomePage());

	//	autonavi
	pPropItem->GetSubItem(tagWebBrowserAutoNavi-tagWebBrowser)->SetValue((_variant_t)pWebBrowser->IsAutoNavigation());
	pPropItem->GetSubItem(tagWebBrowserAutoNavi-tagWebBrowser)->SetOriginalValue((_variant_t)pWebBrowser->IsAutoNavigation());

	pPropItem->Show(TRUE,FALSE);
}

void CUIProperties::ShowColorPaletteProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CColorPaletteUI* pColorPaletteUI=static_cast<CColorPaletteUI*>(pControl->GetInterface(DUI_CTR_COLORPALETTE));
	ASSERT(pColorPaletteUI);


	CMFCPropertyGridProperty* pPropItem=m_wndPropList.FindItemByData(classColorPalette,FALSE);
	ASSERT(pPropItem);

	//palletheight
	pPropItem->GetSubItem(tagCPPalletheight-tagColorPalette)->SetValue((_variant_t)(LONG)pColorPaletteUI->GetPalletHeight());
	pPropItem->GetSubItem(tagCPPalletheight-tagColorPalette)->SetOriginalValue((_variant_t)(LONG)pColorPaletteUI->GetPalletHeight());

	//barheight
	pPropItem->GetSubItem(tagCPBarHeight-tagColorPalette)->SetValue((_variant_t)(LONG)pColorPaletteUI->GetBarHeight());
	pPropItem->GetSubItem(tagCPBarHeight-tagColorPalette)->SetOriginalValue((_variant_t)(LONG)pColorPaletteUI->GetBarHeight());

	//thumbimage
	pPropItem->GetSubItem(tagCPThumbImage-tagColorPalette)->SetValue((_variant_t)pColorPaletteUI->GetThumbImage());
	pPropItem->GetSubItem(tagCPThumbImage-tagColorPalette)->SetOriginalValue((_variant_t)pColorPaletteUI->GetThumbImage());

	pPropItem->Show(TRUE,FALSE);

}


void CUIProperties::ShowListElementProperty(CControlUI* pControl)
{
	ShowControlProperty(pControl);

	ASSERT(pControl);
	CListElementUI* pListElementUI = static_cast<CListElementUI*>(pControl->GetInterface(DUI_CTR_LISTELEMENT));
	ASSERT(pListElementUI);


	CMFCPropertyGridProperty* pPropItem = m_wndPropList.FindItemByData(classListElement, FALSE);
	ASSERT(pPropItem);

	//seleted
	pPropItem->GetSubItem(tagLESelect - tagListElement)->SetValue((_variant_t)pListElementUI->IsSelected());
	pPropItem->GetSubItem(tagLESelect - tagListElement)->SetOriginalValue((_variant_t)pListElementUI->IsSelected());

	pPropItem->Show(TRUE, FALSE);

}

void CUIProperties::ShowListContainerElementProperty(CControlUI* pControl)
{
	ShowContainerProperty(pControl);
}

void CUIProperties::ShowTreeViewProperty(CControlUI* pControl)
{
	ShowListProperty(pControl);
	//TODO :待补充属性

}
void CUIProperties::ShowTreeNodeProperty(CControlUI* pControl)
{
	ShowListContainerElementProperty(pControl);
	//TODO :待补充属性
}


void CUIProperties::ShowDateProperty(CControlUI* pControl)
{

}

void CUIProperties::ShowFadeButtonProperty(CControlUI* pControl)
{
	ShowButtonProperty(pControl);
}