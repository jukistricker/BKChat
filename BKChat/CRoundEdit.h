#pragma once
class CRoundEdit : public CEdit
{
protected:
    afx_msg void OnNcPaint();
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp); // ✅ đúng kiểu

    DECLARE_MESSAGE_MAP()
    
};
