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

hid_t get_H5_type_id(int R_type){
    hid_t H5_type = 0;
	switch (R_type)
	{
	case INTSXP:
		H5_type = H5T_NATIVE_INT;
		break;
	case LGLSXP:
		H5_type = H5T_NATIVE_INT;
		break;
	case REALSXP:
		H5_type = H5T_NATIVE_DOUBLE;
		break;
	case RAWSXP:
		H5_type = H5T_NATIVE_CHAR;
		break;
	default:
		assert(!"Unknown type");
	}
	return H5_type;
}