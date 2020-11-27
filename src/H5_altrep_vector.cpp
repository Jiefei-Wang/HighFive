#include <string>
#include <Rcpp.h>
#include "Travel/Travel.h"
#include "utils.h"
#include "H5_vector_reader.h"
using namespace Rcpp;



size_t h5_read_func(const Travel_altrep_info *altrep_info, void *buffer,
                    size_t offset, size_t length)
{
    H5_vector_reader &reader = *(H5_vector_reader *)altrep_info->private_data;
    size_t read_length = reader.read(altrep_info->type, buffer, offset, length);
    return read_length;
}

// [[Rcpp::export]]
SEXP C_make_h5_altrep_vector(String file_name, String dataset_name, int type = 0, bool transpose = false, SEXP attributes = R_NilValue)
{
    PROTECT_GUARD guard;
    H5_vector_reader *h5_reader = new H5_vector_reader(file_name, dataset_name);
    h5_reader->set_transpose(transpose);
    SEXP extPtr = guard.protect(Travel_shared_ptr<H5_vector_reader>(h5_reader));
    Travel_altrep_info altrep_info;
    altrep_info.length = h5_reader->get_length();
    altrep_info.type = type == 0 ? h5_reader->get_suggested_type() : type;
    altrep_info.operations.get_region = h5_read_func;
    altrep_info.private_data = h5_reader;
    altrep_info.protected_data = extPtr;
    SEXP x = guard.protect(Travel_make_altrep(altrep_info));
    if(attributes!= R_NilValue){
        SET_ATTRIB(x, attributes);
    }
    return x;
}