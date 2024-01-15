#include "STDString.h"
#include <cstring>

const char *STDString::data() const { return start; }
const size_t STDString::size() const { return length; }

STDString::STDString(const char *str) {
    length = strlen(str);
    start = new char[length];
    memcpy(start, str, length);
}

STDString::~STDString() { delete[] start; }

STDString &STDString::operator=(const char *str) {
    delete[] start;
    length = strlen(str);
    start = new char[length];
    memcpy(start, str, length);

    return *this;
}

STDString &STDString::operator=(const STDString &str) {
    delete[] start;
    length = str.size();
    start = new char[length];
    memcpy(start, str.start, length);

    return *this;
}

STDString &STDString::operator+=(const char *str) {
    char *new_start = new char[length + strlen(str)];
    memcpy(new_start, start, length);
    memcpy(new_start + length, str, strlen(str));
    length += strlen(str);
    delete[] start;
    start = new_start;
    return *this;
}

STDString &STDString::operator+=(const STDString &str) {
    char *new_start = new char[length + str.size()];
    memcpy(new_start, start, length);
    memcpy(new_start + length, str.data(), str.size());
    length += str.size();
    delete[] start;
    start = new_start;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const STDString &str) {
    os << str.data();
    os.flush();
    return os;
}
