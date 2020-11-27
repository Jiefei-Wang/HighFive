#include <string>
#include <Rcpp.h>
#include "H5Cpp.h"
#include "utils.h"


using namespace Rcpp;
using namespace H5;



std::string get_H5_type_class(H5::DataType& data_type){
    switch (data_type.getClass())
    {
    case H5T_NO_CLASS:
        return "NO_CLASS";
    case H5T_INTEGER:
        return "INTEGER";
    case H5T_FLOAT:
        return "FLOAT";
    case H5T_TIME:
        return "TIME";
    case H5T_STRING:
        return "STRING";
    case H5T_BITFIELD:
        return "BITFIELD";
    case H5T_OPAQUE:
        return "OPAQUE";
    case H5T_COMPOUND:
        return "COMPOUND";
    case H5T_REFERENCE:
        return "REFERENCE";
    case H5T_ENUM:
        return "ENUM";
    case H5T_VLEN:
        return "VLEN";
    case H5T_ARRAY:
        return "ARRAY";
    case H5T_NCLASSES:
        return "NCLASSES";
    default:
        return "UNKNOWN";
    }
}

H5::PredType get_H5_type_id(int R_type){
    hid_t H5_type = 0;
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


int get_H5_R_suggested_type(H5T_class_t& H5_type)
{
    if (H5_type == H5T_INTEGER)
    {
        return INTSXP;
    }
    if(H5_type == H5T_FLOAT){
        return REALSXP;
    }
    Rf_error("unknown type");
    return 0;
}