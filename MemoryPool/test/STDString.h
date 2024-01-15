#pragma once

#include <strstream>

class STDString {
  private:
    char *start;
    size_t length;

  public:
    const char *data() const;
    const size_t size() const;

    STDString(const char *str = "");
    ~STDString();

    STDString &operator=(const char *str);

    STDString &operator=(const STDString &str);

    STDString &operator+=(const char *str);

    STDString &operator+=(const STDString &str);

    friend std::ostrstream &operator<<(std::ostrstream &os,
                                       const STDString &str);
};
