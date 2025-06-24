#include "pch.h"
#include "Message.h"

// Implementation của constructor cũ (để tương thích với code hiện có)
Message::Message(const CString& sender, const CString& content, const COleDateTime& time, bool isSenderMe)
    : sender(sender), content(content), timestamp(time), isSenderMe(isSenderMe) 
{
}