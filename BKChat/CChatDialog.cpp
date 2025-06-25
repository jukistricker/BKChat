// CChatDialog.cpp : implementation file
//

#include "pch.h"
#include "BKChat.h"
#include "afxdialogex.h"
#include "CChatDialog.h"

// ✅ Define constants at the top of the file
#define SCROLL_LINE_SIZE 20
#define SCROLL_PAGE_SIZE 100

// CChatDialog dialog

IMPLEMENT_DYNAMIC(CChatDialog, CDialogEx)

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
	DDX_Control(pDX, IDC_BUTTON_SEND, btn_send);
}


BEGIN_MESSAGE_MAP(CChatDialog, CDialogEx)
    ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_USER + 1, &CChatDialog::OnRefreshMessages)

    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
    ON_WM_PAINT()

    ON_EN_CHANGE(IDC_EDIT_INPUT_MSG, &CChatDialog::OnEnChangeEditInputMsg)
    ON_BN_CLICKED(IDC_BUTTON_ICON, &CChatDialog::OnBnClickedButtonIcon)
    ON_BN_CLICKED(IDC_BUTTON_IMG, &CChatDialog::OnBnClickedButtonImg)
    ON_BN_CLICKED(IDC_BUTTON_FILE, &CChatDialog::OnBnClickedButtonFile)
    ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatDialog::OnBnClickedButtonSend)
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
    m_chat->AddMessage(Message(username, _T("Tôi khỏe"), COleDateTime::GetCurrentTime(), false));
    m_chat->AddMessage(Message(_T("Me"), _T("Xin chào!"), COleDateTime::GetCurrentTime(), true));
    m_chat->AddMessage(Message(_T("Me"), _T("Hedonist Roots\nUntil recently, the prevailing view assumed lorem ipsum was born as a nonsense text.“It’s not Latin, though it looks like it, and it actually says nothing, ” Before & After magazine answered a curious reader, “Its ‘words’ loosely approximate the frequency with which letters occur in English, which is why at a glance it looks pretty real.”\nAs Cicero would put it, “Um, not so fast.”\nThe placeholder text, beginning with the line “Lorem ipsum dolor sit amet, consectetur adipiscing elit”, looks like Latin because in its youth, centuries ago, it was Latin."), COleDateTime::GetCurrentTime(), true));
    m_chat->AddMessage(Message(username, _T("Bạn khỏe không?"), COleDateTime::GetCurrentTime(), true));
    m_chat->AddMessage(Message(username, _T("Chào bạn!"), COleDateTime::GetCurrentTime(), false));
    m_chat->AddMessage(Message(username, _T("Hedonist Roots\nUntil recently, the prevailing view assumed lorem ipsum was born as a nonsense text.“It’s not Latin, though it looks like it, and it actually says nothing, ” Before & After magazine answered a curious reader, “Its ‘words’ loosely approximate the frequency with which letters occur in English, which is why at a glance it looks pretty real.”\nAs Cicero would put it, “Um, not so fast.”\nThe placeholder text, beginning with the line “Lorem ipsum dolor sit amet, consectetur adipiscing elit”, looks like Latin because in its youth, centuries ago, it was Latin."), COleDateTime::GetCurrentTime(), false));
   
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

    // ✅ Xoá các item cũ
    for (auto& info : m_messageInfos) {
        if (info.item) {
            info.item->DestroyWindow();
            delete info.item;
        }
    }
    m_messageInfos.clear();

    // Cũng clear m_messageControls nếu vẫn còn sử dụng
    m_messageControls.clear();

    // Tính toán vùng chat (trừ input area)
    CRect dialogRect;
    GetClientRect(&dialogRect);
    m_clientHeight = dialogRect.Height();
    m_chatAreaHeight = m_clientHeight - INPUT_AREA_HEIGHT;

    int dialogWidth = dialogRect.Width();
    int margin = 10;
    int spacing = 10;

    TRACE(_T("RefreshMessages: dialogWidth=%d, clientHeight=%d, chatAreaHeight=%d\n"),
        dialogWidth, m_clientHeight, m_chatAreaHeight);

    // Tạo DC để đo text
    CClientDC dc(this);

    int y = 20; // Vị trí bắt đầu từ top (tọa độ logic)

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

        // ✅ Tạo rect với tọa độ logic
        CRect logicalRect(margin, y, dialogWidth - margin, y + requiredSize.cy);

        TRACE(_T("Message %zu: logical y=%d, rect=(%d,%d,%d,%d), size=%dx%d\n"),
            i, y, logicalRect.left, logicalRect.top, logicalRect.right, logicalRect.bottom,
            requiredSize.cx, requiredSize.cy);

        // ✅ Tạo control với rect tạm thời
        if (item->Create(this, msg, logicalRect, isSenderMe, showAvatar)) {
            if (showAvatar && !isSenderMe && m_targetAvatar) {
                item->SetAvatar(m_targetAvatar);
            }

            // ✅ Lưu thông tin item và vị trí logic
            m_messageInfos.emplace_back(item, logicalRect);
            m_messageControls.push_back(item); // Giữ để tương thích với code khác

            int customSpacing = isSameSender ? 3 : spacing;
            y += requiredSize.cy + customSpacing;
        }
        else {
            TRACE(_T("Failed to create message item %zu\n"), i);
            delete item;
        }
    }

    // Cập nhật thông tin scroll
    m_totalContentHeight = y + 20; // Thêm margin cuối
    UpdateScrollInfo();

    // ✅ Áp dụng scroll position để hiển thị đúng vùng
    ApplyScrollPosition();

    Invalidate();
    UpdateWindow();
    TRACE("RefreshMessages finished: totalHeight=%d, chatAreaHeight=%d, visibleMessages=%zu\n",
        m_totalContentHeight, m_chatAreaHeight, m_messageInfos.size());
}

void CChatDialog::ApplyScrollPosition()
{
    for (auto& info : m_messageInfos) {
        if (info.item && info.item->GetSafeHwnd()) {
            // ✅ Tính vị trí hiển thị từ vị trí logic ban đầu
            CRect displayRect = info.logicalRect;
            displayRect.OffsetRect(0, -m_scrollPos);  // Áp dụng scroll offset

            // ✅ Kiểm tra xem item có nằm trong vùng chat không
            bool isVisible = (displayRect.bottom > 0 && displayRect.top < m_chatAreaHeight);

            // ✅ Clip rect để không vào input area
            CRect clippedRect = displayRect;
            if (clippedRect.bottom > m_chatAreaHeight) {
                clippedRect.bottom = m_chatAreaHeight;
            }
            if (clippedRect.top < 0) {
                clippedRect.top = 0;
            }

            if (isVisible && clippedRect.Height() > 0) {
                info.item->MoveWindow(clippedRect, TRUE);
                info.item->ShowWindow(SW_SHOW);

                TRACE(_T("Item shown: logical=(%d,%d,%d,%d) -> display=(%d,%d,%d,%d)\n"),
                    info.logicalRect.left, info.logicalRect.top, info.logicalRect.right, info.logicalRect.bottom,
                    clippedRect.left, clippedRect.top, clippedRect.right, clippedRect.bottom);
            }
            else {
                info.item->ShowWindow(SW_HIDE);
                TRACE(_T("Item hidden: outside visible area\n"));
            }
        }
    }
}


// ✅ Cập nhật thông tin scroll bar
void CChatDialog::UpdateScrollInfo()
{
    // Tính toán chiều cao vùng chat (trừ đi input area)
    CRect clientRect;
    GetClientRect(&clientRect);
    m_chatAreaHeight = clientRect.Height() - INPUT_AREA_HEIGHT;

    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;
    si.nMin = 0;

    if (m_totalContentHeight > m_chatAreaHeight) {
        // Cần scroll
        m_isScrollEnabled = true;
        si.nMax = m_totalContentHeight - 1;
        si.nPage = m_chatAreaHeight;  // ✅ Sử dụng chat area height thay vì client height
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

    TRACE(_T("UpdateScrollInfo: total=%d, chatArea=%d, client=%d, scrollPos=%d, enabled=%d\n"),
        m_totalContentHeight, m_chatAreaHeight, clientRect.Height(), m_scrollPos, m_isScrollEnabled);
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
        m_chatAreaHeight = cy - INPUT_AREA_HEIGHT;

        // ✅ FIXED: Reset scroll position nếu không còn cần scroll
        if (m_totalContentHeight <= m_chatAreaHeight) {
            m_scrollPos = 0;
        }
        else {
            // Đảm bảo scroll position không vượt quá giới hạn
            m_scrollPos = min(m_scrollPos, GetMaxScrollPos());
        }

        UpdateScrollInfo();

        // ✅ FIXED: Refresh messages để tính toán lại layout
        if (m_chat && !m_messageControls.empty()) {
            PostMessage(WM_USER + 1, 0, 0); // Gọi RefreshMessages async
        }
    }
    ResizeControls();
}

// ✅ Helper methods
void CChatDialog::ScrollToPosition(int newPos)
{
    int maxScrollPos = GetMaxScrollPos();
    newPos = max(0, min(newPos, maxScrollPos));

    if (newPos != m_scrollPos) {
        m_scrollPos = newPos;

        // Cập nhật scroll bar
        SetScrollPos(SB_VERT, m_scrollPos, TRUE);

        // ✅ Chỉ cần gọi ApplyScrollPosition - không cần tính toán phức tạp
        ApplyScrollPosition();

        // ✅ Invalidate vùng chat để vẽ lại background
        CRect chatRect;
        GetClientRect(&chatRect);
        chatRect.bottom = m_chatAreaHeight;
        InvalidateRect(&chatRect);

        TRACE(_T("ScrollToPosition: %d, maxScroll: %d, chatAreaHeight: %d\n"),
            newPos, maxScrollPos, m_chatAreaHeight);
    }
}

// ✅ Thêm hàm vẽ background để phân biệt rõ chat area và input area
void CChatDialog::OnPaint()
{
    CPaintDC dc(this);

    CRect clientRect;
    GetClientRect(&clientRect);

    // ✅ Vẽ background cho chat area
    CRect chatAreaRect = clientRect;
    chatAreaRect.bottom = m_chatAreaHeight;
    dc.FillSolidRect(&chatAreaRect, RGB(240, 240, 240)); // Light gray cho chat area

    // ✅ Vẽ background cho input area
    CRect inputAreaRect = clientRect;
    inputAreaRect.top = m_chatAreaHeight;
    dc.FillSolidRect(&inputAreaRect, RGB(250, 250, 250)); // Slightly different color

    // ✅ Vẽ đường phân cách
    CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
    CPen* oldPen = dc.SelectObject(&pen);
    dc.MoveTo(0, m_chatAreaHeight);
    dc.LineTo(clientRect.Width(), m_chatAreaHeight);
    dc.SelectObject(oldPen);
}

void CChatDialog::ScrollToBottom()
{
    if (m_isScrollEnabled) {
        ScrollToPosition(GetMaxScrollPos());
    }
}

int CChatDialog::GetMaxScrollPos()
{
    return max(0, m_totalContentHeight - m_chatAreaHeight);  // ✅ Sử dụng chat area height
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

    // ✅ Khởi tạo các biến scroll
    m_totalContentHeight = 0;
    m_scrollPos = 0;
    m_clientHeight = 0;
    m_isScrollEnabled = false;

    // ✅ Tính toán kích thước ban đầu
    CRect clientRect;
    GetClientRect(&clientRect);
    m_clientHeight = clientRect.Height();
    m_chatAreaHeight = m_clientHeight - INPUT_AREA_HEIGHT;  // ✅ Tính chat area height ngay từ đầu

    TRACE(_T("OnInitDialog: clientHeight=%d, chatAreaHeight=%d\n"),
        m_clientHeight, m_chatAreaHeight);

    CreateInputArea();
    ResizeControls();

    // ✅ Chỉ gọi RefreshMessages sau khi đã setup xong layout
    if (m_chat) {
        RefreshMessages();
    }

    TRACE(_T("CChatDialog::OnInitDialog completed\n"));

    return TRUE;
}

void CChatDialog::CreateInputArea()
{
    // Style cho edit control
    edt_inp_msg.ModifyStyle(0, ES_MULTILINE | ES_WANTRETURN);
    edt_inp_msg.SetWindowText(L"Nhập tin nhắn...");

    // Style cho các nút
    btn_icon.SetWindowText(L"😊");
    btn_img.SetWindowText(L"📷");
    btn_file.SetWindowText(L"📎");
    btn_send.SetWindowText(L"➤");

    // Set font cho các control
    CFont font;
    font.CreatePointFont(90, L"Segoe UI");
    edt_inp_msg.SetFont(&font);
    btn_icon.SetFont(&font);
    btn_img.SetFont(&font);
    btn_file.SetFont(&font);
    btn_send.SetFont(&font);
    font.Detach();
}

void CChatDialog::ResizeControls()
{
    if (!IsWindow(edt_inp_msg.m_hWnd))
        return;

    CRect clientRect;
    GetClientRect(&clientRect);

    const int BUTTON_SIZE = 40;
    const int MARGIN = 10;
    const int BUTTON_SPACING = 5;

    // ✅ Input area luôn cố định ở dưới cùng
    CRect inputAreaRect = clientRect;
    inputAreaRect.top = m_chatAreaHeight; // Bắt đầu từ sau chat area

    // Vị trí các nút trong input area
    CRect btnFileRect(
        inputAreaRect.left + MARGIN,
        inputAreaRect.top + (INPUT_AREA_HEIGHT - BUTTON_SIZE) / 2,
        inputAreaRect.left + MARGIN + BUTTON_SIZE,
        inputAreaRect.top + (INPUT_AREA_HEIGHT + BUTTON_SIZE) / 2
    );

    CRect btnImgRect = btnFileRect;
    btnImgRect.OffsetRect(BUTTON_SIZE + BUTTON_SPACING, 0);

    CRect btnIconRect = btnImgRect;
    btnIconRect.OffsetRect(BUTTON_SIZE + BUTTON_SPACING, 0);

    CRect btnSendRect(
        inputAreaRect.right - MARGIN - BUTTON_SIZE,
        inputAreaRect.top + (INPUT_AREA_HEIGHT - BUTTON_SIZE) / 2,
        inputAreaRect.right - MARGIN,
        inputAreaRect.top + (INPUT_AREA_HEIGHT + BUTTON_SIZE) / 2
    );

    // Edit control ở giữa input area
    CRect editRect(
        btnIconRect.right + BUTTON_SPACING,
        inputAreaRect.top + MARGIN,
        btnSendRect.left - BUTTON_SPACING,
        inputAreaRect.bottom - MARGIN
    );

    // ✅ Đảm bảo các control input luôn ở đúng vị trí
    btn_file.MoveWindow(&btnFileRect);
    btn_img.MoveWindow(&btnImgRect);
    btn_icon.MoveWindow(&btnIconRect);
    btn_send.MoveWindow(&btnSendRect);
    edt_inp_msg.MoveWindow(&editRect);

    // ✅ Đảm bảo input controls luôn hiển thị trên cùng
    btn_file.BringWindowToTop();
    btn_img.BringWindowToTop();
    btn_icon.BringWindowToTop();
    btn_send.BringWindowToTop();
    edt_inp_msg.BringWindowToTop();

    TRACE(_T("ResizeControls: inputArea=(%d,%d,%d,%d), chatAreaHeight=%d\n"),
        inputAreaRect.left, inputAreaRect.top, inputAreaRect.right, inputAreaRect.bottom,
        m_chatAreaHeight);
}

void CChatDialog::DebugMessagePositions()
{
    TRACE(_T("=== DEBUG MESSAGE POSITIONS ===\n"));
    TRACE(_T("ScrollPos: %d, ChatAreaHeight: %d, TotalHeight: %d\n"),
        m_scrollPos, m_chatAreaHeight, m_totalContentHeight);

    for (size_t i = 0; i < m_messageInfos.size(); ++i) {
        auto& info = m_messageInfos[i];
        if (info.item && info.item->GetSafeHwnd()) {
            CRect currentRect;
            info.item->GetWindowRect(&currentRect);
            ScreenToClient(&currentRect);

            bool isVisible = info.item->IsWindowVisible();
            TRACE(_T("Message %zu: logical=(%d,%d,%d,%d), current=(%d,%d,%d,%d), visible=%d\n"),
                i,
                info.logicalRect.left, info.logicalRect.top, info.logicalRect.right, info.logicalRect.bottom,
                currentRect.left, currentRect.top, currentRect.right, currentRect.bottom,
                isVisible);
        }
    }
    TRACE(_T("=== END DEBUG ===\n"));
}

// ✅ ADDITION: Method để đảm bảo message cuối cùng luôn hiển thị
void CChatDialog::EnsureLastMessageVisible()
{
    if (m_totalContentHeight > m_chatAreaHeight) {
        ScrollToPosition(GetMaxScrollPos());
    }
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

void CChatDialog::OnBnClickedButtonSend()
{
    // TODO: Add your control notification handler code here
}

CChatDialog::~CChatDialog()
{
    // ✅ Cleanup message controls (từ m_messageInfos)
    for (auto& info : m_messageInfos) {
        if (info.item) {
            info.item->DestroyWindow();
            delete info.item;
        }
    }
    m_messageInfos.clear();

    // Cleanup vector cũ (để tương thích)
    m_messageControls.clear();

    // Cleanup chat object
    if (m_chat) {
        delete m_chat;
        m_chat = nullptr;
    }
}