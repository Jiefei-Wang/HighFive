#include <string>
#include <vector>
#include <Rcpp.h>
#include "Travel/Travel.h"
#include "utils.h"
#include "H5_table_reader.h"
#include "H5_utils.h"
using namespace Rcpp;

size_t h5_table_col_read_func(const Travel_altrep_info *altrep_info, void *buffer,
                              size_t offset, size_t length)
{
    H5_table_reader &reader = *(H5_table_reader *)altrep_info->private_data;
    size_t read_length = reader.read(altrep_info->type, buffer, offset, length);
    return read_length;
}

SEXP C_make_h5_altrep_table_col(H5_table_info &table_info, int field_index)
{
    PROTECT_GUARD guard;
    H5_table_reader *h5_reader = new H5_table_reader(table_info, field_index);
    SEXP extPtr = guard.protect(Travel_shared_ptr<H5_table_reader>(h5_reader));
    Travel_altrep_info altrep_info;
    altrep_info.length = h5_reader->get_length();
    altrep_info.type = h5_reader->get_suggested_type();
    altrep_info.operations.get_region = h5_table_col_read_func;
    altrep_info.private_data = h5_reader;
    altrep_info.protected_data = extPtr;
    SEXP x = guard.protect(Travel_make_altrep(altrep_info));
    return x;
}

bool supports_type(std::string type)
{
    if (type == "INTEGER")
    {
        return true;
    }
    if (type == "FLOAT")
    {
        return true;
    }
    return false;
}

// [[Rcpp::export]]
SEXP C_make_h5_altrep_table(String file_name, String table_name)
{
    H5_table_info table_info(file_name, table_name);
    std::vector<bool> supported_cols;
    supported_cols.resize(table_info.n_field);
    size_t n_supported_cols = 0;
    for (size_t i = 0; i < table_info.n_field; i++)
    {
        if (supports_type(table_info.field_info.elt_types[i]))
        {
            n_supported_cols++;
            supported_cols[i] = true;
        }
        else
        {
            std::string type_name = get_H5_type_class(table_info.field_info.elt_H5_types[i]);
            Rf_warning("Unsupported data type <%s> in the table", type_name.c_str());
            supported_cols[i] = false;
        }
    }
    Rcpp::List res(n_supported_cols);
    size_t list_index = 0;
    for (size_t i = 0; i < table_info.n_field; i++)
    {
        if (supports_type(table_info.field_info.elt_types[i]))
        {
            res[i] = C_make_h5_altrep_table_col(table_info,i);
            list_index++;
        }
    }
    return res;
}