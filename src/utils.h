#include <stdint.h>

#ifdef Rcpp_hpp
#ifndef UTILS_ENABLE_R
#define UTILS_ENABLE_R
#endif
#endif

#ifdef UTILS_ENABLE_R
#ifndef R_INTERNALS_H_
#include "Rcpp.h"
#endif
size_t get_object_size(SEXP x);
class PROTECT_GUARD
{
private:
  int protect_num = 0;

public:
  PROTECT_GUARD() {}
  ~PROTECT_GUARD()
  {
    if (protect_num != 0)
      UNPROTECT(protect_num);
  }
  SEXP protect(SEXP x)
  {
    protect_num++;
    return PROTECT(x);
  }
};
#endif

uint8_t get_type_size(int type);



#include <memory>
class Unique_buffer
{
  size_t size = 0;
  std::unique_ptr<char[]> ptr;
public:
  void reserve(size_t reserve_size)
  {
    if (reserve_size > size)
    {
      ptr.reset(new char[reserve_size]);
      size = reserve_size;
    }
  }
  void release()
  {
    if (size > 1024 * 1024)
    {
      ptr.reset(nullptr);
      size = 0;
    }
  }
  char* get(){
    return ptr.get();
  }
};