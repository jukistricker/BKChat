#pragma once
#include <afxwin.h>

class CRoundedAvatar : public CStatic
{
public:
    CRoundedAvatar();
    virtual ~CRoundedAvatar();

    void SetAvatar(HBITMAP hBitmap);

protected:
    HBITMAP m_hBitmap = NULL;

    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};
