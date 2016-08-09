#pragma  once 

#ifndef UIWKEWEBKIT_H
#define UIWKEWEBKIT_H
#include "stdafx.h"
#include "3rd/wke.h"

namespace DuiLib
{  
    class CWKEWebkitUI : public CControlUI
    {
    public:
        CWKEWebkitUI();
        ~CWKEWebkitUI();

        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);
        virtual void DoInit();
        virtual void DoEvent(TEventUI& event);
        virtual void SetPos(RECT rc);
        virtual void PaintBkImage(HDC hDC);
        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);


    public:
        virtual    void onBufUpdated (const HDC hdc,int x, int y, int cx, int cy);
        ////网页标题、地址改变的回调  
        void SetClientHandler(const wkeClientHandler* handler);
        void LoadURL( wstring strValue);
        void LoadFile( wstring strValue);
        void GoBack();
        void GoForward();
        wstring RunJS(const wchar_t* script);

        void    StopLoad();
        void    Refresh();
    public:
        wkeWebView GetWebView();
        const wstring& GetUrl() const;
    protected:
        wkeWebView pWebView_;
        BYTE*      pBuffer_;
        int        nSize_;
        wstring    strUrl_;

    };
}


#endif