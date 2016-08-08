#include "stdafx.h"
#include "WKEWebkit.h"

#pragma comment(lib,"lib/wke.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"msimg32.lib")


#define TICKTIMERID 50

CWKEWebkitUI::CWKEWebkitUI():pBuffer_(NULL), nSize_(0)
{
    pWebView_=NULL;
}

CWKEWebkitUI::~CWKEWebkitUI()
{
    pWebView_->destroy();
    if ( pBuffer_ )
    {
        free(pBuffer_);
        pBuffer_ = NULL;
    }

    wkeShutdown();
}

LPCTSTR CWKEWebkitUI::GetClass() const
{
    return _T("WKEWebkitUI");
}
LPVOID CWKEWebkitUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, DUI_CTR_WKEWEBKIT) == 0 )
    {
        return static_cast<CWKEWebkitUI*>(this);
    }
    
    return CControlUI::GetInterface(pstrName);
}

void CWKEWebkitUI::DoInit()
{
    //TODO 多次初始化问题
    wkeInit();
    pWebView_ = wkeCreateWebView();
    pWebView_->setTransparent(false);
    CDuiRect rect(m_rcItem);
    pWebView_->resize(rect.GetWidth(),rect.GetHeight());

    m_pManager->SetTimer(this,TICKTIMERID,50);
 
}

void CWKEWebkitUI::DoEvent(TEventUI& event)
{

    bool handled = true;
    if (!pWebView_)
    {
        return CControlUI::DoEvent(event);
    }
    if (event.Type == UIEVENT_TIMER)
    {
        if (TICKTIMERID==event.wParam)
        {
            CControlUI* pParent = GetParent();
            if ( pParent->IsVisible() && pWebView_->isDirty() )
            {
                HDC hDC = m_pManager->GetPaintDC();
                PaintBkImage(hDC);
            }
        }
    }

    else if (event.Type == UIEVENT_BUTTONDOWN)
    {
        HWND hwnd=m_pManager->GetPaintWindow();
        ::SetFocus(hwnd);
        ::SetCapture(hwnd);

       // pWebView_->focus();
        int x = GET_X_LPARAM(event.lParam);
        int y = GET_Y_LPARAM(event.lParam);

        unsigned int flags = 0;

        if (event.wParam & MK_CONTROL)
            flags |= WKE_CONTROL;
        if (event.wParam & MK_SHIFT)
            flags |= WKE_SHIFT;

        if (event.wParam & MK_LBUTTON)
            flags |= WKE_LBUTTON;
        if (event.wParam & MK_MBUTTON)
            flags |= WKE_MBUTTON;
        if (event.wParam & MK_RBUTTON)
            flags |= WKE_RBUTTON;


        x = event.ptMouse.x-m_rcPaint.left;
        y =  event.ptMouse.y-m_rcPaint.top;

        handled = pWebView_->mouseEvent(WM_LBUTTONDOWN, x, y, flags);
    }else if (event.Type == UIEVENT_BUTTONUP)
    {
       
        unsigned int flags = 0;

        if (event.wParam & MK_CONTROL)
            flags |= WKE_CONTROL;
        if (event.wParam & MK_SHIFT)
            flags |= WKE_SHIFT;

        if (event.wParam & MK_LBUTTON)
            flags |= WKE_LBUTTON;
        if (event.wParam & MK_MBUTTON)
            flags |= WKE_MBUTTON;
        if (event.wParam & MK_RBUTTON)
            flags |= WKE_RBUTTON;

        int x = event.ptMouse.x-m_rcPaint.left;
        int y =  event.ptMouse.y-m_rcPaint.top;

        handled = pWebView_->mouseEvent(WM_LBUTTONUP, x, y, flags);

    }else if (event.Type == UIEVENT_MOUSEMOVE)
    {
        
        unsigned int flags = 0;

        if (event.wParam & MK_CONTROL)
            flags |= WKE_CONTROL;
        if (event.wParam & MK_SHIFT)
            flags |= WKE_SHIFT;

        if (event.wParam & MK_LBUTTON)
            flags |= WKE_LBUTTON;
        if (event.wParam & MK_MBUTTON)
            flags |= WKE_MBUTTON;
        if (event.wParam & MK_RBUTTON)
            flags |= WKE_RBUTTON;

        int x = event.ptMouse.x-m_rcPaint.left;
        int y =  event.ptMouse.y-m_rcPaint.top;
        handled = pWebView_->mouseEvent(WM_MOUSEMOVE, x, y, flags);

    }else if (event.Type == UIEVENT_RBUTTONDOWN)
    {
      
        unsigned int flags = 0;

        if (event.wParam & MK_CONTROL)
            flags |= WKE_CONTROL;
        if (event.wParam & MK_SHIFT)
            flags |= WKE_SHIFT;

        if (event.wParam & MK_LBUTTON)
            flags |= WKE_LBUTTON;
        if (event.wParam & MK_MBUTTON)
            flags |= WKE_MBUTTON;
        if (event.wParam & MK_RBUTTON)
            flags |= WKE_RBUTTON;
      
        int x = event.ptMouse.x-m_rcPaint.left;
        int y =  event.ptMouse.y-m_rcPaint.top;

        handled = pWebView_->mouseEvent(WM_RBUTTONDOWN, x, y, flags);


    }
    else if (event.Type == UIEVENT_RBUTTONUP)
    {

        ::ReleaseCapture();
        unsigned int flags = 0;

        if (event.wParam & MK_CONTROL)
            flags |= WKE_CONTROL;
        if (event.wParam & MK_SHIFT)
            flags |= WKE_SHIFT;

        if (event.wParam & MK_LBUTTON)
            flags |= WKE_LBUTTON;
        if (event.wParam & MK_MBUTTON)
            flags |= WKE_MBUTTON;
        if (event.wParam & MK_RBUTTON)
            flags |= WKE_RBUTTON;
       
        int x = event.ptMouse.x-m_rcPaint.left;
        int y =  event.ptMouse.y-m_rcPaint.top;

        handled = pWebView_->mouseEvent(WM_RBUTTONUP, x, y, flags);

    }
    else if (event.Type == UIEVENT_SCROLLWHEEL)
    {
        POINT pt ;
        pt.x = event.ptMouse.x;
        pt.y = event.ptMouse.y;
        pt.x -= m_rcPaint.left;
        pt.y -= m_rcPaint.top;
        ScreenToClient(m_pManager->GetPaintWindow(), &pt);

        int delta = GET_WHEEL_DELTA_WPARAM(event.wParam);
        unsigned int flags = 0;
        //fwKeys = GET_KEYSTATE_WPARAM(wParam);
        if (event.wParam & MK_CONTROL)
            flags |= WKE_CONTROL;
        if (event.wParam & MK_SHIFT)
            flags |= WKE_SHIFT;

        if (event.wParam & MK_LBUTTON)
            flags |= WKE_LBUTTON;
        if (event.wParam & MK_MBUTTON)
            flags |= WKE_MBUTTON;
        if (event.wParam & MK_RBUTTON)
            flags |= WKE_RBUTTON;


        handled = pWebView_->mouseWheel(pt.x,pt.y, delta, flags);
    }else if (event.Type == UIEVENT_KEYDOWN)
    {
        unsigned int virtualKeyCode = event.wParam;
        unsigned int flags = 0;
        if (HIWORD(event.lParam) & KF_REPEAT)
            flags |= WKE_REPEAT;
        if (HIWORD(event.lParam) & KF_EXTENDED)
            flags |= WKE_EXTENDED;

        handled = pWebView_->keyDown(virtualKeyCode, flags, false);
        if (event.wParam==VK_F5)
        {
            Refresh();
        }

    }else if (event.Type == UIEVENT_KEYUP)
    {
        unsigned int virtualKeyCode = event.wParam;
        unsigned int flags = 0;
        if (HIWORD(event.lParam) & KF_REPEAT)
            flags |= WKE_REPEAT;
        if (HIWORD(event.lParam) & KF_EXTENDED)
            flags |= WKE_EXTENDED;

        handled = pWebView_->keyUp(virtualKeyCode, flags, false);
    }else if (event.Type == UIEVENT_CHAR)
    {
        unsigned int nCharCode = event.chKey;
        unsigned int flags = 0;
        if (HIWORD(event.lParam) & KF_REPEAT)
            flags |= WKE_REPEAT;
        if (HIWORD(event.lParam) & KF_EXTENDED)
            flags |= WKE_EXTENDED;

    
        handled = pWebView_->keyPress(nCharCode, flags, false);
        if (handled)
        {
            return;
        }

    }
    else if ((event.Type == UIEVENT_CONTEXTMENU))
    {


        unsigned int flags = 0;
        if (event.wParam & MK_CONTROL)
            flags |= WKE_CONTROL;
        if (event.wParam & MK_SHIFT)
            flags |= WKE_SHIFT;
        if (event.wParam & MK_LBUTTON)
            flags |= WKE_LBUTTON;
        if (event.wParam & MK_MBUTTON)
            flags |= WKE_MBUTTON;
        if (event.wParam & MK_RBUTTON)
            flags |= WKE_RBUTTON;
        bool handled = pWebView_->contextMenuEvent(event.ptMouse.x, event.ptMouse.y, flags);
        if ( handled )
            return ;
    }
    else if (event.Type == UIEVENT_IME_STARTCOMPOSITION)
    {
        wkeRect caret = pWebView_->getCaret();

        CANDIDATEFORM form;
        form.dwIndex = 0;
        form.dwStyle = CFS_EXCLUDE;
        form.ptCurrentPos.x = caret.x ;
        form.ptCurrentPos.y = caret.y + caret.h;
        form.rcArea.top = caret.y + m_rcPaint.top;
        form.rcArea.bottom = caret.y + caret.h +m_rcPaint.top;
        form.rcArea.left = caret.x +m_rcPaint.left;
        form.rcArea.right = caret.x + caret.w +m_rcPaint.left;

        HIMC hIMC = ImmGetContext(m_pManager->GetPaintWindow());
        ImmSetCandidateWindow(hIMC, &form);
        ImmReleaseContext(m_pManager->GetPaintWindow(), hIMC);
    }
    else if (event.Type ==UIEVENT_SETFOCUS)
    {
        pWebView_->focus(); 
    }
    else if (event.Type == UIEVENT_KILLFOCUS)
    {
        pWebView_->unfocus();   
    }
    else if(event.Type== UIEVENT_WINDOWSIZE)
    {
        pWebView_->resize(GET_X_LPARAM(event.lParam), GET_Y_LPARAM(event.lParam));
    }
    if (!handled)
    {
      //  CControlUI::DoEvent(event);
    }

}


void CWKEWebkitUI::SetPos(RECT rc)
{
    CControlUI::SetPos(rc);
    CDuiRect rect(m_rcItem);
    if (pWebView_)
    {
        pWebView_->resize(rect.GetWidth(),rect.GetHeight());
    }
}


void CWKEWebkitUI::PaintBkImage( HDC hDC )  
{  
    int nWidth = m_rcItem.right - m_rcItem.left;  
    int nHeight = m_rcItem.bottom - m_rcItem.top;  
    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);  
    HDC hMemDC = CreateCompatibleDC(hDC);  
    int nPitch = ((nWidth*32+31)>>5)<<2;  
    int nSize = nHeight*nPitch;  

    if ( NULL == pBuffer_ || nSize_<nSize )  
    {  
        if ( pBuffer_ )  
            free(pBuffer_);  
        pBuffer_ = (BYTE*)malloc(nSize);  
        nSize_ = nSize;  
    }  
    pWebView_->paint(pBuffer_, 0);  
    SetBitmapBits(hBitmap, nSize, pBuffer_);  
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBitmap);  
    BitBlt(hDC, m_rcItem.left, m_rcItem.top, nWidth, nHeight, hMemDC, 0, 0, SRCCOPY);  
    SelectObject(hMemDC, hOldBmp);  
    DeleteDC(hMemDC);  
    DeleteObject(hBitmap);  
}  



void CWKEWebkitUI::onBufUpdated (const HDC hdc,int x, int y, int cx, int cy)
{
    this->Invalidate();
}



void CWKEWebkitUI::SetClientHandler(const wkeClientHandler* handler)
{
    if (pWebView_)
    {
        pWebView_->setClientHandler(handler);
    }
}

void CWKEWebkitUI::LoadURL( wstring strValue)
{
    if (pWebView_)
    {
        pWebView_->loadURL(strValue.c_str());
    }
}

void CWKEWebkitUI::LoadFile( wstring strValue)
{
    if (pWebView_)
    {
        pWebView_->loadFile(strValue.c_str());
    }
}


void CWKEWebkitUI::GoBack()
{

    if (pWebView_)
    {
        if (pWebView_->canGoBack())
        {
            pWebView_->goBack();
        }
    }

}

void CWKEWebkitUI::GoForward()
{

    if (pWebView_)
    {
        if (pWebView_->canGoForward())
        {
            pWebView_->goForward();
        }
    }

}


jsValue  CWKEWebkitUI::RunJS(const wchar_t* script)
{
    if (pWebView_)
    {
       return pWebView_->runJS(script);
    }

    return -1;
}

const wstring& CWKEWebkitUI::GetUrl() const
{
    return strUrl_;
}

void CWKEWebkitUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
    if ( _tcscmp(pstrName, _T("url")) == 0 )
        strUrl_ = pstrValue;
    else
        CControlUI::SetAttribute(pstrName, pstrValue);
}

void CWKEWebkitUI::StopLoad()
{
    if ( pWebView_ )
        pWebView_->stopLoading();
}

void CWKEWebkitUI::Refresh()
{
    if ( pWebView_ )
    {
        pWebView_->reload();
    }
}

wkeWebView CWKEWebkitUI::GetWebView()
{
    return pWebView_;
}