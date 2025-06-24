#pragma once
#include <afxstr.h>
#include <ctime>
#include <afxdisp.h> // COleDateTime

class Message {
public:
    CString sender;
    CString content;
    COleDateTime timestamp;
    bool isSenderMe;

    // Constructor mặc định
    Message() : isSenderMe(false) {}

    // Constructor với tham số (giữ nguyên signature cũ để tương thích)
    Message(const CString& sender, const CString& content, const COleDateTime& time, bool isSenderMe);


    // Copy constructor
    Message(const Message& other)
        : sender(other.sender), content(other.content),
        timestamp(other.timestamp), isSenderMe(other.isSenderMe) {
    }

    // Assignment operator
    Message& operator=(const Message& other) {
        if (this != &other) {
            sender = other.sender;
            content = other.content;
            timestamp = other.timestamp;
            isSenderMe = other.isSenderMe;
        }
        return *this;
    }
};
