#include <Rcpp.h>
#include "H5Cpp.h"
#include "utils.h"
#include "H5_vector_reader.h"
#include "hdf5_hl.h"

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
        SET_REAL_ELT(x, n_dims - i - 1, h5_reader.get_dim(i));
    }
    return x;
}

// [[Rcpp::export]]
SEXP test(String file_name, String table_name)
{
    H5File file = H5File(file_name, H5F_ACC_RDONLY);
    hsize_t n_field;
    hsize_t n_record;
    herr_t error = H5TBget_table_info(file.getId(), table_name.get_cstring(), &n_field, &n_record);
    char **names_out = new char*[n_field];
    for (hsize_t i = 0; i < n_field; i++)
    {
        names_out[i] = new char[255];
    }
    std::vector<size_t> field_sizes;
    field_sizes.resize(n_field);
    std::vector<size_t> offset_out;
    offset_out.resize(n_field);
    size_t type_size;
    H5TBget_field_info(file.getId(), table_name.get_cstring(),
                       names_out, field_sizes.data(), offset_out.data(), &type_size);
    CharacterVector names(n_field);
    for (hsize_t i = 0; i < n_field; i++)
    {
        names[i]=names_out[i];
    }
    for (hsize_t i = 0; i < n_field; i++)
    {
        delete[] names_out[i];
    }
    delete[] names_out;
    return names;
}