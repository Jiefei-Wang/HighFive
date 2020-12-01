#include <string>
#include <Rcpp.h>
#include "H5Cpp.h"
#include "utils.h"

using namespace Rcpp;
using namespace H5;

H5::PredType get_R_H5_type_id(int R_type)
{
    switch (R_type)
    {
    case INTSXP:
        return H5::PredType::NATIVE_INT;
    case LGLSXP:
        return H5::PredType::NATIVE_INT;
    case REALSXP:
        return H5::PredType::NATIVE_DOUBLE;
    case RAWSXP:
        return H5::PredType::NATIVE_INT8;
    default:
        assert(!"Unknown type");
    }
    return H5::PredType::NATIVE_INT8;
}

int get_H5_R_suggested_type(H5T_class_t &H5_type, size_t type_size, bool bit64conversion)
{
    if (H5_type == H5T_INTEGER)
    {
        if (type_size > sizeof(int) && bit64conversion)
        {
            return REALSXP;
        }
        else
        {
            return INTSXP;
        }
    }
    if (H5_type == H5T_FLOAT)
    {
        return REALSXP;
    }
    Rf_error("unknown type");
    return 0;
}