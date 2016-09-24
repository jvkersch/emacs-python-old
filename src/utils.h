#ifndef UTILS_H
#define UTILS_H

class Error
{
public:
    Error(const std::string &msg) : message(msg) {}
    operator std::string() const { return message; }

private:
    std::string message;
};

#endif // UTILS_H
