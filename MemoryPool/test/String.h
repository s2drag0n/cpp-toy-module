#pragma once

#include "../MemoryPool.h"
#include <strstream>

class String {
  private:
    char *start;
    size_t length;
    MemoryPool *mp;

  public:
    const char *data() const;
    const size_t size() const;

    String(MemoryPool *mp, const char *str = "");
    ~String();

    String &operator=(const char *str);

    String &operator=(const String &str);

    String &operator+=(const char *str);

    String &operator+=(const String &str);

    friend std::ostrstream &operator<<(std::ostrstream &os, const String &str);
};
