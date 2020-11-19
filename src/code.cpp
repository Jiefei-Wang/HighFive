#include <string>
#include <Rcpp.h>
#include "H5Cpp.h"
#include "utils.h"
#include "H5_vector_reader.h"

using namespace Rcpp;
using namespace H5;

// [[Rcpp::export]]
SEXP C_read_h5_altrep(String file_name, String dataset_name, size_t offset, size_t length, bool trans = false, int type = 0)
{
    PROTECT_GUARD guard;
    H5_vector_reader h5_reader(file_name, dataset_name);
    int R_type = type == 0 ? h5_reader.get_suggested_type() : type;
    h5_reader.set_transpose(trans);
    h5_reader.set_exception(true);
    SEXP x = guard.protect(Rf_allocVector(R_type, length));
    h5_reader.read(R_type, DATAPTR(x), offset, length);
    return x;
}

// [[Rcpp::export]]
int get_type_index(String type_name)
{
    if (type_name == "raw")
    {
        return RAWSXP;
    }
    if (type_name == "integer")
    {
        return INTSXP;
    }
    if (type_name == "numeric" || type_name == "real" || type_name == "double")
    {
        return REALSXP;
    }
    if (type_name == "logical")
    {
        return LGLSXP;
    }
    Rf_error("Unknown type name <%s>", type_name.get_cstring());
    return 0;
}

// [[Rcpp::export]]
SEXP get_dims(String file_name, String dataset_name)
{
    H5_vector_reader h5_reader(file_name, dataset_name);
    PROTECT_GUARD guard;
    size_t n_dims = h5_reader.get_n_dims();
    SEXP x = guard.protect(Rf_allocVector(REALSXP, n_dims));
    for (size_t i = 0; i < n_dims; i++)
    {
        SET_REAL_ELT(x, n_dims-i-1, h5_reader.get_dim(i));
    }
    return x;
}