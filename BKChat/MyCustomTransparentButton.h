#pragma once
#include "afxbutton.h"

class MyCustomTransparentButton : public CMFCButton
{
    DECLARE_DYNAMIC(MyCustomTransparentButton)
public:
    MyCustomTransparentButton() {}
    virtual ~MyCustomTransparentButton() {}
    afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);



protected:
    DECLARE_MESSAGE_MAP()

};
