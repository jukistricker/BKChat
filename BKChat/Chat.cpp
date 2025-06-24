#include "pch.h"
#include "Chat.h"

Chat::Chat(const CString& user) : user(user) {}

void Chat::AddMessage(const Message& msg) {
    messages.push_back(msg);
}
