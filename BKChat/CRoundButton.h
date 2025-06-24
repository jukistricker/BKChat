#pragma once
class CRoundButton : public CButton
{
public:
    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
    BOOL m_bHover;

protected:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    DECLARE_MESSAGE_MAP()
};
