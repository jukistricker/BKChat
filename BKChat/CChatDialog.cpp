// CChatDialog.cpp : implementation file
//

#include "pch.h"
#include "BKChat.h"
#include "afxdialogex.h"
#include "CChatDialog.h"


// CChatDialog dialog

IMPLEMENT_DYNAMIC(CChatDialog, CDialogEx)

CChatDialog::~CChatDialog()
{
    // Cleanup message controls (vector)
    for (auto* item : m_messageControls) {
        if (item) {
            item->DestroyWindow();
            delete item;
        }
    }
    m_messageControls.clear();

    // Cleanup chat object
    if (m_chat) {
        delete m_chat;
        m_chat = nullptr;
    }

    // Note: m_targetAvatar được quản lý bởi CHomeDialog, không delete ở đây
}

CChatDialog::CChatDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CHAT_DIALOG, pParent),
    m_chat(nullptr),
    m_targetAvatar(nullptr)  // ✅ Khởi tạo avatar = nullptr
{
}

void CChatDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_INPUT_MSG, edt_inp_msg);
    DDX_Control(pDX, IDC_BUTTON_ICON, btn_icon);
    DDX_Control(pDX, IDC_BUTTON_IMG, btn_img);
    DDX_Control(pDX, IDC_BUTTON_FILE, btn_file);
}


BEGIN_MESSAGE_MAP(CChatDialog, CDialogEx)
    ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_USER + 1, &CChatDialog::OnRefreshMessages)

    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()

    ON_EN_CHANGE(IDC_EDIT_INPUT_MSG, &CChatDialog::OnEnChangeEditInputMsg)
    ON_BN_CLICKED(IDC_BUTTON_ICON, &CChatDialog::OnBnClickedButtonIcon)
    ON_BN_CLICKED(IDC_BUTTON_IMG, &CChatDialog::OnBnClickedButtonImg)
    ON_BN_CLICKED(IDC_BUTTON_FILE, &CChatDialog::OnBnClickedButtonFile)
    ON_BN_CLICKED(IDC_BUTTON6, &CChatDialog::OnBnClickedButton6)
END_MESSAGE_MAP()


void CChatDialog::SetTargetUser(const CString& username)
{
    m_targetUser = username;
    SetWindowText(username);

    // Xóa chat cũ nếu có
    if (m_chat) {
        delete m_chat;
        m_chat = nullptr;
    }

    // Tạo chat mới
    m_chat = new Chat(username);

    // Demo dữ liệu giả lập
    m_chat->AddMessage(Message(_T("Me"), _T("Xin chào!"), COleDateTime::GetCurrentTime(), true));
    m_chat->AddMessage(Message(_T("Me"), _T("Hedonist Roots\nUntil recently, the prevailing view assumed lorem ipsum was born as a nonsense text.“It’s not Latin, though it looks like it, and it actually says nothing, ” Before & After magazine answered a curious reader, “Its ‘words’ loosely approximate the frequency with which letters occur in English, which is why at a glance it looks pretty real.”\nAs Cicero would put it, “Um, not so fast.”\nThe placeholder text, beginning with the line “Lorem ipsum dolor sit amet, consectetur adipiscing elit”, looks like Latin because in its youth, centuries ago, it was Latin."), COleDateTime::GetCurrentTime(), true));
    m_chat->AddMessage(Message(username, _T("Bạn khỏe không?"), COleDateTime::GetCurrentTime(), true));
    m_chat->AddMessage(Message(username, _T("Chào bạn!"), COleDateTime::GetCurrentTime(), false));
    m_chat->AddMessage(Message(username, _T("Hedonist Roots\nUntil recently, the prevailing view assumed lorem ipsum was born as a nonsense text.“It’s not Latin, though it looks like it, and it actually says nothing, ” Before & After magazine answered a curious reader, “Its ‘words’ loosely approximate the frequency with which letters occur in English, which is why at a glance it looks pretty real.”\nAs Cicero would put it, “Um, not so fast.”\nThe placeholder text, beginning with the line “Lorem ipsum dolor sit amet, consectetur adipiscing elit”, looks like Latin because in its youth, centuries ago, it was Latin."), COleDateTime::GetCurrentTime(), false));
    m_chat->AddMessage(Message(username, _T("Tôi khỏe"), COleDateTime::GetCurrentTime(), false));
    m_chat->AddMessage(Message(username, _T("Bạn khỏe không?"), COleDateTime::GetCurrentTime(), false));

    // Debug: Kiểm tra avatar có được set chưa
    if (m_targetAvatar) {
        TRACE(_T("Target avatar is set for user: %s\n"), username);
    }
    else {
        TRACE(_T("Warning: No avatar set for user: %s\n"), username);
    }

    TRACE(_T("Chat created with %d messages\n"), (int)m_chat->messages.size());

    RefreshMessages();
}

void CChatDialog::RefreshMessages()
{
    TRACE("CChatDialog::RefreshMessages called\n");

    if (!m_chat) return;

    // Xoá các item cũ
    for (auto* item : m_messageControls) {
        if (item) {
            item->DestroyWindow();
            delete item;
        }
    }
    m_messageControls.clear();

    // Lấy kích thước client hiện tại
    CRect dialogRect;
    GetClientRect(&dialogRect);
    m_clientHeight = dialogRect.Height();

    int dialogWidth = dialogRect.Width();
    int margin = 10;
    int spacing = 10;

    TRACE(_T("Dialog width: %d, height: %d\n"), dialogWidth, dialogRect.Height());

    // Tạo DC để đo text
    CClientDC dc(this);

    int y = 20; // Vị trí bắt đầu từ top

    for (size_t i = 0; i < m_chat->messages.size(); ++i) {
        const auto& msg = m_chat->messages[i];
        bool isSenderMe = (msg.sender == _T("Me"));
        bool isSameSender = (i > 0 && msg.sender == m_chat->messages[i - 1].sender);
        bool showAvatar = !isSenderMe && !isSameSender;

        CMessageItem* item = new CMessageItem();
        item->SetMessage(msg);
        item->SetSenderMe(isSenderMe);
        item->SetShowAvatar(showAvatar);

        CSize requiredSize = item->CalculateRequiredSize(&dc, dialogWidth - margin * 2);

        // ✅ Tính toán vị trí với scroll offset
        int itemTop = y - m_scrollPos;
        int itemBottom = itemTop + requiredSize.cy;

        CRect rect(margin, itemTop, dialogWidth - margin, itemBottom);

        TRACE(_T("Message %zu: y=%d, scrollPos=%d, itemTop=%d\n"), i, y, m_scrollPos, itemTop);

        if (item->Create(this, msg, rect, isSenderMe, showAvatar)) {
            if (showAvatar && !isSenderMe && m_targetAvatar) {
                item->SetAvatar(m_targetAvatar);
            }

            m_messageControls.push_back(item);

            int customSpacing = isSameSender ? 3 : spacing;
            y += requiredSize.cy + customSpacing;
        }
        else {
            TRACE(_T("Failed to create message item %zu\n"), i);
            delete item;
        }
    }

    // ✅ Cập nhật thông tin scroll
    m_totalContentHeight = y + 20; // Thêm margin cuối
    UpdateScrollInfo();

    Invalidate();
    UpdateWindow();
    TRACE("CChatDialog::RefreshMessages finished, total height: %d\n", m_totalContentHeight);
}

// ✅ Cập nhật thông tin scroll bar
void CChatDialog::UpdateScrollInfo()
{
    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;
    si.nMin = 0;

    if (m_totalContentHeight > m_clientHeight) {
        // Cần scroll
        m_isScrollEnabled = true;
        si.nMax = m_totalContentHeight - 1;
        si.nPage = m_clientHeight;
        si.nPos = m_scrollPos;

        // Đảm bảo scroll position hợp lệ
        int maxScrollPos = GetMaxScrollPos();
        if (m_scrollPos > maxScrollPos) {
            m_scrollPos = maxScrollPos;
            si.nPos = m_scrollPos;
        }
    }
    else {
        // Không cần scroll
        m_isScrollEnabled = false;
        si.nMax = 0;
        si.nPage = 0;
        si.nPos = 0;
        m_scrollPos = 0;
    }

    SetScrollInfo(SB_VERT, &si, TRUE);

    TRACE(_T("UpdateScrollInfo: total=%d, client=%d, scrollPos=%d, enabled=%d\n"),
        m_totalContentHeight, m_clientHeight, m_scrollPos, m_isScrollEnabled);
}

// ✅ Xử lý scroll bar
void CChatDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (!m_isScrollEnabled) return;

    int newPos = m_scrollPos;
    int maxScrollPos = GetMaxScrollPos();

    switch (nSBCode) {
    case SB_LINEUP:
        newPos -= SCROLL_LINE_SIZE;
        break;
    case SB_LINEDOWN:
        newPos += SCROLL_LINE_SIZE;
        break;
    case SB_PAGEUP:
        newPos -= SCROLL_PAGE_SIZE;
        break;
    case SB_PAGEDOWN:
        newPos += SCROLL_PAGE_SIZE;
        break;
    case SB_THUMBTRACK:
    case SB_THUMBPOSITION:
        newPos = nPos;
        break;
    case SB_TOP:
        newPos = 0;
        break;
    case SB_BOTTOM:
        newPos = maxScrollPos;
        break;
    }

    ScrollToPosition(newPos);
    CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

// ✅ Xử lý mouse wheel
BOOL CChatDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (!m_isScrollEnabled) {
        return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
    }

    // zDelta > 0: scroll up, zDelta < 0: scroll down
    int scrollLines = abs(zDelta) / WHEEL_DELTA * 3; // 3 lines per wheel notch
    int newPos = m_scrollPos;

    if (zDelta > 0) {
        newPos -= scrollLines * SCROLL_LINE_SIZE; // Scroll up
    }
    else {
        newPos += scrollLines * SCROLL_LINE_SIZE; // Scroll down
    }

    ScrollToPosition(newPos);
    return TRUE;
}

// ✅ Xử lý resize
void CChatDialog::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (nType != SIZE_MINIMIZED) {
        m_clientHeight = cy;
        UpdateScrollInfo();

        // Refresh messages để cập nhật layout
        if (m_chat && !m_messageControls.empty()) {
            RefreshMessages();
        }
    }
}

// ✅ Helper methods
void CChatDialog::ScrollToPosition(int newPos)
{
    int maxScrollPos = GetMaxScrollPos();
    newPos = max(0, min(newPos, maxScrollPos));

    if (newPos != m_scrollPos) {
        int deltaY = m_scrollPos - newPos;
        m_scrollPos = newPos;

        // Cập nhật scroll bar
        SetScrollPos(SB_VERT, m_scrollPos, TRUE);

        // Di chuyển tất cả message controls
        for (auto* item : m_messageControls) {
            if (item && item->GetSafeHwnd()) {
                CRect rect;
                item->GetWindowRect(&rect);
                ScreenToClient(&rect);
                rect.OffsetRect(0, deltaY);
                item->MoveWindow(rect, TRUE);
            }
        }

        TRACE(_T("ScrollToPosition: %d (delta: %d)\n"), newPos, deltaY);
    }
}

void CChatDialog::ScrollToBottom()
{
    if (m_isScrollEnabled) {
        ScrollToPosition(GetMaxScrollPos());
    }
}

int CChatDialog::GetMaxScrollPos()
{
    return max(0, m_totalContentHeight - m_clientHeight);
}


HBRUSH CChatDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    UINT id = pWnd->GetDlgCtrlID();


    if (id == 2002) // Text label
    {
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }
    else if (id == 2001) // Nền label - ĐỔI THÀNH MÀU ĐỎ
    {
        static HBRUSH hRed = CreateSolidBrush(RGB(255, 0, 0)); // Màu đỏ
        return hRed;
    }

    return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CChatDialog::SetTargetAvatar(HBITMAP hAvatar)
{
    m_targetAvatar = hAvatar;
}

LRESULT CChatDialog::OnRefreshMessages(WPARAM wParam, LPARAM lParam)
{
    RefreshMessages();
    return 0;
}

BOOL CChatDialog::OnInitDialog()
{
    ModifyStyle(0, WS_CAPTION | WS_SYSMENU | WS_VSCROLL);
    CDialogEx::OnInitDialog();
    CDialog::OnInitDialog();

    // ✅ Khởi tạo các biến scroll
    m_totalContentHeight = 0;
    m_scrollPos = 0;
    m_clientHeight = 0;
    m_isScrollEnabled = false;

    // ✅ Debug: Kiểm tra dialog đã được tạo chưa
    TRACE(_T("CChatDialog::OnInitDialog called\n"));

    return TRUE;
}

//void CChatDialog::AddMessageItem(const Message& message, bool isSenderMe)
//{
//    // Tính toán vị trí cho message item mới
//    CRect clientRect;
//    m_messageContainer.GetClientRect(&clientRect);
//
//    // Tính height cần thiết cho message
//    CDC* pDC = m_messageContainer.GetDC();
//    CMessageItem tempItem;
//    tempItem.UpdateMessage(message);
//    CSize requiredSize = tempItem.CalculateRequiredSize(pDC, clientRect.Width());
//    m_messageContainer.ReleaseDC(pDC);
//
//    // Tính vị trí Y cho item mới (stack từ trên xuống)
//    int yPos = 0;
//    for (int i = 0; i < m_messageItems.GetSize(); i++) {
//        CRect itemRect;
//        m_messageItems[i]->GetWindowRect(&itemRect);
//        m_messageContainer.ScreenToClient(&itemRect);
//        yPos = max(yPos, itemRect.bottom + 5); // 5px spacing
//    }
//
//    // Tạo message item
//    CMessageItem* pMessageItem = new CMessageItem();
//    CRect itemRect(0, yPos, clientRect.Width(), yPos + requiredSize.cy);
//
//    if (pMessageItem->Create(&m_messageContainer, message, itemRect, isSenderMe)) {
//        // ✅ Set avatar cho message của người nhận
//        if (!isSenderMe && m_targetAvatar) {
//            pMessageItem->SetAvatar(m_targetAvatar);
//        }
//
//        m_messageItems.Add(pMessageItem);
//
//        // Cập nhật scroll range
//        UpdateScrollRange();
//
//        // Scroll to bottom
//        ScrollToBottom();
//    }
//    else {
//        delete pMessageItem;
//    }
//}
//
//// 3. Cập nhật method gửi tin nhắn
//void CChatDialog::OnSendMessage()
//{
//    CString messageText;
//    m_editMessage.GetWindowText(messageText);
//
//    if (messageText.IsEmpty()) return;
//
//    // Tạo message object
//    Message message;
//    message.content = messageText;
//    message.timestamp = CTime::GetCurrentTime();
//    message.sender = _T("Me"); // hoặc current user
//
//    // Thêm message của mình (isSenderMe = true)
//    AddMessageItem(message, true);
//
//    // Clear input
//    m_editMessage.SetWindowText(_T(""));
//
//    // Giả lập reply từ người khác (cho demo)
//    SimulateReceivedMessage();
//}
//
//void CChatDialog::SimulateReceivedMessage()
//{
//    // Tạo message reply giả lập
//    Message replyMessage;
//    replyMessage.content = _T("Thanks for your message!");
//    replyMessage.timestamp = CTime::GetCurrentTime();
//    replyMessage.sender = m_targetUser;
//
//    // Thêm message của người khác (isSenderMe = false) - sẽ có avatar
//    AddMessageItem(replyMessage, false);
//}



void CChatDialog::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
}

void CChatDialog::OnEnChangeEdit1()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}

void CChatDialog::OnBnClickedButton3()
{
    // TODO: Add your control notification handler code here
}

void CChatDialog::OnEnChangeEditInputMsg()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}

void CChatDialog::OnBnClickedButtonIcon()
{
    // TODO: Add your control notification handler code here
}

void CChatDialog::OnBnClickedButtonImg()
{
    // TODO: Add your control notification handler code here
}

void CChatDialog::OnBnClickedButtonFile()
{
    // TODO: Add your control notification handler code here
}

void CChatDialog::OnBnClickedButton6()
{
    // TODO: Add your control notification handler code here
}
