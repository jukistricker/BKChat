#pragma once
#include "afxbutton.h"

class MyCustomButton : public CMFCButton
{
    DECLARE_DYNAMIC(MyCustomButton)
public:
    MyCustomButton() {}
    virtual ~MyCustomButton() {}
    afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    


protected:
    DECLARE_MESSAGE_MAP()
    
};
