#pragma once

#include <exception>
#include <string>

namespace vor {

class VorException: public std::exception {
  public:
    VorException(void)  : message("Unspecified exception for"
                                " voroL5 program") {}
    explicit VorException(const char *msg) : message(msg) {}
    explicit VorException(const std::string & msg) : message(msg) {}
    virtual ~VorException() throw () {}
    virtual const char* what() const throw() {
       return message.c_str(); 
    }
  protected:
    std::string message;
};

class ManagerError : public VorException {
public:
  ManagerError(void)  : VorException("Unspecified exception for"
                                " sofem Manager") {}
  ManagerError(const std::string &msg) : VorException(msg) {} 
};

class ParseError : public VorException {
public:
  ParseError() : VorException("Unspecified parse error") {}
  ParseError(const std::string &explanation, const std::string &culprit) {
    message = explanation + " : " + culprit; 
  }
};

} // namespace vor
