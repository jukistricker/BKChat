#pragma once
#include "Message.h"

class CMessageItem : public CWnd
{
public:
    CMessageItem();
    CMessageItem(CWnd* pParent, const Message& message, const CRect& rect, bool isSenderMe, bool showAvatar = true);
    virtual ~CMessageItem();

    BOOL Create(CWnd* pParent, const Message& message, const CRect& rect, bool isSenderMe, bool showAvatar = true);
    void UpdateMessage(const Message& message);
    void SetColors(COLORREF bgColor, COLORREF textColor);
    void SetAvatar(HBITMAP hBitmap);
    void MoveAvatarTo(const CRect& rect);

    void SetMessage(const Message& message);
    void SetSenderMe(bool isSenderMe);
    void SetShowAvatar(bool showAvatar);
    

    // Tính toán kích thước dynamic
    CSize CalculateRequiredSize(CDC* pDC, int maxWidth);

protected:
    virtual void OnPaint();
    virtual BOOL OnEraseBkgnd(CDC* pDC);

    DECLARE_MESSAGE_MAP()

private:
    Message m_message;
    bool m_isSenderMe;
    HBITMAP m_avatarBitmap;
    bool m_showAvatar = true;

    // Colors theo design Jetpack
    COLORREF m_senderBubbleColor = RGB(67, 127, 236);    // #437fec - xanh dương
    COLORREF m_receiverBubbleColor = RGB(218, 218, 218); // #dadada - xám nhạt
    COLORREF m_senderTextColor = RGB(255, 255, 255);     // Trắng
    COLORREF m_receiverTextColor = RGB(0, 0, 0);         // Đen

    // Dimensions
    static const int BUBBLE_RADIUS = 10;
    static const int AVATAR_SIZE = 42;
    static const int MARGIN = 10;
    static const int BUBBLE_PADDING_H = 15;
    static const int BUBBLE_PADDING_V = 8;
    static const int SPACING = 8;

    // Helper methods
    void DrawRoundedBubble(CDC* pDC, const CRect& rect, COLORREF color);
    void DrawAvatar(CDC* pDC, const CRect& rect);
    CRect CalculateBubbleRect(const CRect& clientRect, const CSize& textSize);
    CRect CalculateAvatarRect(const CRect& clientRect);
    CSize MeasureText(CDC* pDC, const CString& text, int maxWidth);
};