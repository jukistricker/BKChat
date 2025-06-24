#pragma once
#include "Message.h"
#include <vector>

class Chat {
public:
    CString user;
    std::vector<Message> messages;

    Chat(const CString& user);
    void AddMessage(const Message& msg);
};


