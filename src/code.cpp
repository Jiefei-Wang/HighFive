#include <Rcpp.h>
#include "H5Cpp.h"
#include "utils.h"
#include "H5_dataset_reader.h"
#include "H5_table_reader.h"
#include "hdf5_hl.h"

using namespace Rcpp;
using namespace H5;

// [[Rcpp::export]]
SEXP C_read_h5_altrep(String file_name, String dataset_name, size_t offset, size_t length, bool trans = false, int type = 0)
{
    PROTECT_GUARD guard;
    H5_dataset_reader h5_reader(file_name, dataset_name);
    int R_type = type == 0 ? h5_reader.get_suggested_type(true) : type;
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
    H5_dataset_reader h5_reader(file_name, dataset_name);
    PROTECT_GUARD guard;
    size_t n_dims = h5_reader.get_n_dims();
    SEXP x = guard.protect(Rf_allocVector(REALSXP, n_dims));
    for (size_t i = 0; i < n_dims; i++)
    {
        SET_REAL_ELT(x, n_dims - i - 1, h5_reader.get_dim(i));
    }
    return x;
}

#include "H5_dataset_info.h"
//[[Rcpp::export]]
void test1(String file_name, String table_name)
{
    H5_dataset_info info(file_name, table_name);
    if (info.type_info.get_type_class() == H5T_COMPOUND)
    {
        Compound_info &cmp_type = info.type_info.get_compound_info();
        for (auto i : cmp_type.elt_names)
        {
            Rprintf("Type name:%s\n", i.c_str());
        }
    }
}

// [[Rcpp::export]]
Rcpp::String C_read_h5_string(String file_name, String dataset_name, size_t offset)
{
    H5_dataset_reader h5_reader(file_name, dataset_name);
    h5_reader.set_exception(true);
    return h5_reader.read_str(offset);
}

#include "H5_table_reader.h"
#include "H5_utils.h"
SEXP C_make_h5_table_string_altrep(H5_dataset_info& info, size_t i);
// [[Rcpp::export]]
SEXP test2(String file_name, String dataset_name,
           bool bit64conversion = false)
{
    PROTECT_GUARD guard;
    H5_dataset_info dataset_info(file_name, dataset_name);
    if (dataset_info.type_info.get_type_class() != H5T_COMPOUND)
    {
        Rf_error("The dataset type is <%s>, not the compound type",
                 get_H5_type_class_name(dataset_info.type_info.get_type_class()).c_str());
    }
    Compound_info &info = dataset_info.type_info.get_compound_info();
    //Count the number of supported fields
    size_t n_elt = 0;
    for (size_t i = 0; i < info.elt_numbers; i++)
    {
        if (info.elt_types[i].get_type_class() == H5T_INTEGER ||
            info.elt_types[i].get_type_class() == H5T_FLOAT ||
            info.elt_types[i].get_type_class() == H5T_STRING)
        {
            n_elt++;
        }
    }
    //Create list
    Rcpp::List x(n_elt);
    n_elt = 0;
    for (size_t i = 0; i < info.elt_numbers; i++)
    {
        if (info.elt_types[i].get_type_class() == H5T_INTEGER ||
            info.elt_types[i].get_type_class() == H5T_FLOAT)
        {
            H5_table_reader h5_reader(dataset_info, i);
            SEXP elt =
                guard.protect(Rf_allocVector(h5_reader.get_suggested_type(bit64conversion), h5_reader.get_length()));
            h5_reader.read(TYPEOF(elt), DATAPTR(elt), 0, XLENGTH(elt));
            x[n_elt] = elt;
            n_elt++;
            continue;
        }
        if (info.elt_types[i].get_type_class() == H5T_STRING)
        {
            SEXP elt = guard.protect(C_make_h5_table_string_altrep(dataset_info,i));
            x[n_elt] = elt;
            n_elt++;
            continue;
        }
        Rf_warning("Unsupported type <%s> in the dataset\n", get_H5_type_class_name(info.elt_types[i].get_type_class()).c_str());
    }
    return x;
}