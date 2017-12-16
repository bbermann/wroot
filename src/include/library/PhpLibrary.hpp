#ifndef PHPLIBRARY_H
#define PHPLIBRARY_H

#define BUFFER_SIZE 1024

#include "CustomLibrary.hpp"
#include "../helper/UrlRewriter.hpp"

class PhpLibrary : public CustomLibrary
{
  public:
    PhpLibrary();
    virtual ~PhpLibrary();
    virtual String toString() override;

  protected:
    String getFullPath();
    String getEnvironment();

    UrlRewriter urlRewriter;
};

#endif // PHPLIBRARY_H
