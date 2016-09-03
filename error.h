#ifndef ERROR_H
#define ERROR_H

class Error {
public:
    Error(const std::string &msg) : message(msg) {}
    operator std::string() const { return message; }

private:
    std::string message;
};


#endif // ERROR_H
