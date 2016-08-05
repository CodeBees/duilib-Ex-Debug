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

        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);
        virtual void DoInit();
        virtual void DoEvent(TEventUI& event);
        void SetPos(RECT rc);
        void PaintBkImage(HDC hDC);


    public:
        virtual    void onBufUpdated (const HDC hdc,int x, int y, int cx, int cy);
        ////网页标题、地址改变的回调  
        void SetClientHandler(const wkeClientHandler* handler);
        void LoadURL( wstring strValue);
        void LoadFile( wstring strValue);
        void GoBack();
        void GoForward();
        jsValue RunJS(const wchar_t* script);
    protected:
        wkeWebView pWebView_;
        BYTE*      pBuffer_;
        int        nSize_;

    };
}


#endif