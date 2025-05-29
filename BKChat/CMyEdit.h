// MyEdit.h
#pragma once
#include <afxwin.h>

class CMyEdit : public CEdit
{
public:
    CMyEdit();
    virtual ~CMyEdit();

protected:
    afx_msg void OnPaint();
    afx_msg void OnNcPaint();

    DECLARE_MESSAGE_MAP()
};
