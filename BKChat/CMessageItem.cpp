#include "pch.h"
#include "CMessageItem.h"
#include <algorithm>

BEGIN_MESSAGE_MAP(CMessageItem, CWnd)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CMessageItem::CMessageItem() : m_isSenderMe(false), m_avatarBitmap(nullptr)
{
}

CMessageItem::CMessageItem(CWnd* pParent, const Message& message, const CRect& rect, bool isSenderMe, bool showAvatar)
    : m_message(message), m_isSenderMe(isSenderMe), m_avatarBitmap(nullptr), m_showAvatar(showAvatar)
{
    Create(pParent, message, rect, isSenderMe, showAvatar);
}

CMessageItem::~CMessageItem()
{
    // Avatar bitmap được quản lý bởi parent, không delete ở đây
}

BOOL CMessageItem::Create(CWnd* pParent, const Message& message, const CRect& rect, bool isSenderMe, bool showAvatar)
{
    m_message = message;
    m_isSenderMe = isSenderMe;
    m_showAvatar = showAvatar;

    return CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, 0);
}

void CMessageItem::OnPaint()
{
    CPaintDC dc(this);
    CRect clientRect;
    GetClientRect(&clientRect);

    // Anti-aliasing cho text
    dc.SetTextColor(m_isSenderMe ? m_senderTextColor : m_receiverTextColor);
    dc.SetBkMode(TRANSPARENT);

    // Chọn font
    CFont font;
    font.CreateFont(
        16,                        // Height (tương đương 20sp trong Compose)
        0,                         // Width
        0,                         // Escapement
        0,                         // Orientation
        FW_NORMAL,                 // Weight
        FALSE,                     // Italic
        FALSE,                     // Underline
        FALSE,                     // StrikeOut
        DEFAULT_CHARSET,           // CharSet
        OUT_OUTLINE_PRECIS,        // OutPrecision
        CLIP_DEFAULT_PRECIS,       // ClipPrecision
        CLEARTYPE_QUALITY,         // Quality
        VARIABLE_PITCH | FF_SWISS, // PitchAndFamily
        _T("Segoe UI")             // Facename
    );
    CFont* pOldFont = dc.SelectObject(&font);

    // Đo kích thước text
    CSize textSize = MeasureText(&dc, m_message.content, clientRect.Width() * 0.6);

    // Tính toán vị trí bubble và avatar
    CRect bubbleRect = CalculateBubbleRect(clientRect, textSize);

    // Vẽ avatar (chỉ cho người nhận)
    if (!m_isSenderMe&& m_showAvatar) {
        CRect avatarRect = CalculateAvatarRect(clientRect);
        DrawAvatar(&dc, avatarRect);
    }

    // Vẽ bubble
    COLORREF bubbleColor = m_isSenderMe ? m_senderBubbleColor : m_receiverBubbleColor;
    DrawRoundedBubble(&dc, bubbleRect, bubbleColor);

    // Vẽ text trong bubble
    CRect textRect = bubbleRect;
    textRect.DeflateRect(BUBBLE_PADDING_H, BUBBLE_PADDING_V);

    dc.DrawText(m_message.content, &textRect,
        DT_WORDBREAK | DT_CENTER | DT_VCENTER);

    dc.SelectObject(pOldFont);
}

BOOL CMessageItem::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // Không xóa nền để tránh flicker
}

void CMessageItem::DrawRoundedBubble(CDC* pDC, const CRect& rect, COLORREF color)
{
    // Tạo brush và pen
    CBrush brush(color);
    CPen pen(PS_SOLID, 1, color);

    CBrush* pOldBrush = pDC->SelectObject(&brush);
    CPen* pOldPen = pDC->SelectObject(&pen);

    // Vẽ hình chữ nhật bo tròn
    pDC->RoundRect(rect, CPoint(BUBBLE_RADIUS, BUBBLE_RADIUS));

    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
}

void CMessageItem::DrawAvatar(CDC* pDC, const CRect& rect)
{
    if (!m_avatarBitmap) return;

    // Tạo memory DC để vẽ bitmap
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBitmap = memDC.SelectObject(CBitmap::FromHandle(m_avatarBitmap));

    // Lấy kích thước bitmap
    BITMAP bmpInfo;
    ::GetObject(m_avatarBitmap, sizeof(BITMAP), &bmpInfo);

    // Vẽ avatar với bo tròn (ellipse clipping)
    CRgn clipRgn;
    clipRgn.CreateEllipticRgnIndirect(&rect);
    pDC->SelectClipRgn(&clipRgn);

    // Scale và vẽ bitmap
    pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
        &memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);

    // Restore clipping
    pDC->SelectClipRgn(NULL);

    memDC.SelectObject(pOldBitmap);
}

CRect CMessageItem::CalculateBubbleRect(const CRect& clientRect, const CSize& textSize)
{
    int bubbleWidth = textSize.cx + (BUBBLE_PADDING_H * 2);
    int bubbleHeight = textSize.cy + (BUBBLE_PADDING_V * 2);

    // Giới hạn width tối đa 60% của container (như trong Compose)
    int maxBubbleWidth = (int)(clientRect.Width() * 0.6);
    bubbleWidth = min(bubbleWidth, maxBubbleWidth);

    CRect bubbleRect;

    if (m_isSenderMe) {
        // Căn phải cho tin nhắn của mình
        bubbleRect.left = clientRect.right - bubbleWidth - MARGIN;
        bubbleRect.right = clientRect.right - MARGIN;
    }
    else {
        // Căn trái cho tin nhắn của người khác (sau avatar)
        int leftOffset = AVATAR_SIZE + (MARGIN * 2);
        bubbleRect.left = clientRect.left + leftOffset;
        bubbleRect.right = bubbleRect.left + bubbleWidth;
    }

    bubbleRect.top = clientRect.top + MARGIN;
    bubbleRect.bottom = bubbleRect.top + bubbleHeight;

    return bubbleRect;
}

CRect CMessageItem::CalculateAvatarRect(const CRect& clientRect)
{
    CRect avatarRect;
    avatarRect.left = clientRect.left + MARGIN;
    avatarRect.top = clientRect.top + MARGIN;
    avatarRect.right = avatarRect.left + AVATAR_SIZE;
    avatarRect.bottom = avatarRect.top + AVATAR_SIZE;

    return avatarRect;
}

CSize CMessageItem::CalculateRequiredSize(CDC* pDC, int maxWidth)
{
    // Setup font giống như trong OnPaint
    CFont font;
    font.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, _T("Segoe UI"));

    CFont* pOldFont = pDC->SelectObject(&font);

    // ✅ Tính toán width khả dụng dựa trên layout
    int availableWidth = maxWidth;

    // Nếu là tin nhắn của đối phương và có avatar
    if (!m_isSenderMe && m_showAvatar) {
        availableWidth -= (AVATAR_SIZE + MARGIN * 2); // Trừ chỗ cho avatar
    }

    // ✅ Giới hạn width tối đa 60% của available width
    int maxTextWidth = (int)(availableWidth * 0.6);

    // ✅ Đảm bảo maxTextWidth có giá trị hợp lý
    maxTextWidth = max(maxTextWidth, 100); // Tối thiểu 100px

    TRACE(_T("CalculateRequiredSize: maxWidth=%d, availableWidth=%d, maxTextWidth=%d\n"),
        maxWidth, availableWidth, maxTextWidth);

    // Đo text với width giới hạn
    CSize textSize = MeasureText(pDC, m_message.content, maxTextWidth);

    // Tính kích thước bubble
    int bubbleWidth = textSize.cx + (BUBBLE_PADDING_H * 2);
    int bubbleHeight = textSize.cy + (BUBBLE_PADDING_V * 2);

    // ✅ Đảm bảo bubble có kích thước tối thiểu
    bubbleWidth = max(bubbleWidth, 80);   // Minimum width
    bubbleHeight = max(bubbleHeight, 40); // Minimum height

    // Tính kích thước tổng
    int totalWidth = maxWidth;

    // ✅ Tính chiều cao dựa trên layout
    int totalHeight;

    if (!m_isSenderMe && m_showAvatar) {
        // Nếu có avatar, chiều cao = max(bubble, avatar) + margins
        totalHeight = max(bubbleHeight, AVATAR_SIZE) + (MARGIN * 2);
    }
    else {
        // Nếu không có avatar, chỉ cần chiều cao bubble + margins
        totalHeight = bubbleHeight + MARGIN + 5;
    }

    pDC->SelectObject(pOldFont);

    TRACE(_T("Final size: %dx%d (bubble: %dx%d, text: %dx%d)\n"),
        totalWidth, totalHeight, bubbleWidth, bubbleHeight, textSize.cx, textSize.cy);

    return CSize(totalWidth, totalHeight);
}

CSize CMessageItem::MeasureText(CDC* pDC, const CString& text, int maxWidth)
{
    if (text.IsEmpty()) {
        return CSize(0, 20); // Minimum height cho empty text
    }

    // ✅ Tạo rect để đo text với width giới hạn
    CRect measureRect(0, 0, maxWidth, 0);

    // ✅ Sử dụng DT_CALCRECT để tính toán kích thước chính xác
    int height = pDC->DrawText(text, &measureRect,
        DT_CALCRECT | DT_WORDBREAK | DT_LEFT | DT_TOP);

    // ✅ Debug: In ra kích thước đã tính
    TRACE(_T("MeasureText: '%s' -> %dx%d (maxWidth=%d)\n"),
        text.Left(30), measureRect.Width(), height, maxWidth);

    // ✅ Đảm bảo width không vượt quá maxWidth
    int actualWidth = min(measureRect.Width(), maxWidth);

    // ✅ Đảm bảo height tối thiểu
    int actualHeight = max(height, 20);

    return CSize(actualWidth, actualHeight);
}

void CMessageItem::UpdateMessage(const Message& message)
{
    m_message = message;
    Invalidate();
}

void CMessageItem::SetColors(COLORREF bgColor, COLORREF textColor)
{
    // Có thể customize màu nếu cần
    if (m_isSenderMe) {
        m_senderBubbleColor = bgColor;
        m_senderTextColor = textColor;
    }
    else {
        m_receiverBubbleColor = bgColor;
        m_receiverTextColor = textColor;
    }
    Invalidate();
}

void CMessageItem::SetAvatar(HBITMAP hBitmap)
{
    if (!m_isSenderMe) {
        m_avatarBitmap = hBitmap;
        Invalidate();
    }
}

void CMessageItem::MoveAvatarTo(const CRect& rect)
{
    // Không cần thiết nữa vì avatar được vẽ trực tiếp trong OnPaint
    // Giữ lại để tương thích với code cũ
}

void CMessageItem::SetMessage(const Message& message)
{
    m_message = message;
}

void CMessageItem::SetSenderMe(bool isSenderMe)
{
    m_isSenderMe = isSenderMe;
}

void CMessageItem::SetShowAvatar(bool showAvatar)
{
    m_showAvatar = showAvatar;
}
