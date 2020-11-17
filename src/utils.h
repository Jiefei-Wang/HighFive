#include <stdint.h>

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