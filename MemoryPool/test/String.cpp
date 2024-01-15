#include "String.h"
#include <cstring>

const char *String::data() const { return start; }

const size_t String::size() const { return length; }

String::String(MemoryPool *mp, const char *str) : mp(mp) {
    length = strlen(str);
    start = new (mp) char[length];
    memcpy(start, str, length);
}

String::~String() { mp->free(start); }

String &String::operator=(const char *str) {
    mp->free(start);
    length = strlen(str);
    start = new (mp) char[length];
    memcpy(start, str, length);
    return *this;
}

String &String::operator=(const String &str) {
    mp->free(start);
    length = str.size();
    start = new (mp) char[length];
    memcpy(start, str.data(), length);
    return *this;
}

String &String::operator+=(const char *str) {
    start =
        reinterpret_cast<char *>(mp->reallocate(start, length + strlen(str)));
    memcpy(start + length, str, strlen(str));
    length += strlen(str);
    return *this;
}

String &String::operator+=(const String &str) {
    start =
        reinterpret_cast<char *>(mp->reallocate(start, length + str.size()));
    memcpy(start + length, str.data(), str.size());
    length += str.size();
    return *this;
}

std::ostrstream &operator<<(std::ostrstream &os, const String &str) {
    os << str.start;
    os.flush();
    return os;
}
