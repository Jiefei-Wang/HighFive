#ifndef HEADER_H5_UTILS
#define HEADER_H5_UTILS


#include <Rcpp.h>
#include "H5Cpp.h"

std::string get_H5_type_class(H5::DataType& data_type);


#endif