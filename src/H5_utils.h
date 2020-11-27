#ifndef HEADER_H5_UTILS
#define HEADER_H5_UTILS


#include <Rcpp.h>
#include "H5Cpp.h"

std::string get_H5_type_class(H5::DataType& data_type);
H5::PredType get_H5_type_id(int R_type);
int get_H5_R_suggested_type(H5T_class_t& H5_type);


#endif