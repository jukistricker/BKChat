#pragma once
#include "afxwin.h"

class CMyCheckbox : public CButton
{
public:
    CMyCheckbox() {}
    virtual ~CMyCheckbox() {}

    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void PreSubclassWindow();
};
