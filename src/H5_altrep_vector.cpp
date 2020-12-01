#include <string>
#include <Rcpp.h>
#include "R_ext/Altrep.h"
#include "Travel/Travel.h"
#include "utils.h"
#include "H5_dataset_reader.h"
using namespace Rcpp;

size_t h5_read_func(const Travel_altrep_info *altrep_info, void *buffer,
                    size_t offset, size_t length)
{
    H5_dataset_reader &reader = *(H5_dataset_reader *)altrep_info->private_data;
    size_t read_length = reader.read(altrep_info->type, buffer, offset, length);
    return read_length;
}

// [[Rcpp::export]]
SEXP C_make_h5_altrep_vector(String file_name, String dataset_name, int type = 0, bool transpose = false, SEXP attributes = R_NilValue,
                             bool bit64conversion = false)
{
    PROTECT_GUARD guard;
    H5_dataset_reader *h5_reader = new H5_dataset_reader(file_name, dataset_name);
    h5_reader->set_transpose(transpose);
    SEXP extPtr = guard.protect(Travel_shared_ptr<H5_dataset_reader>(h5_reader));
    Travel_altrep_info altrep_info;
    altrep_info.length = h5_reader->get_length();
    altrep_info.type = (type == 0 ? h5_reader->get_suggested_type(bit64conversion) : type);
    altrep_info.operations.get_region = h5_read_func;
    altrep_info.private_data = h5_reader;
    altrep_info.protected_data = extPtr;
    SEXP x = guard.protect(Travel_make_altrep(altrep_info));
    if (attributes != R_NilValue)
    {
        SET_ATTRIB(x, attributes);
    }
    return x;
}

Rboolean string_dataset_altrep_Inspect(SEXP x, int pre, int deep, int pvec,
                                       void (*inspect_subtree)(SEXP, int, int, int))
{
    Rprintf("H5 String dataset altrep object\n");
    return TRUE;
}

R_xlen_t string_dataset_altrep_length(SEXP x)
{
    H5_dataset_reader *h5_reader = (H5_dataset_reader *)R_ExternalPtrAddr(R_altrep_data2(x));
    return h5_reader->get_length();
}

void *string_dataset_altrep_dataptr(SEXP x, Rboolean writeable)
{
    if(R_altrep_data1(x)!=R_NilValue){
        return DATAPTR(R_altrep_data1(x));
    }
    PROTECT_GUARD guard;
    SEXP stringVec =guard.protect(Rf_allocVector(STRSXP, XLENGTH(x)));
    for(size_t i=0;i<(size_t) XLENGTH(stringVec);i++){
        SET_STRING_ELT(stringVec, i, guard.protect(STRING_ELT(x,i)));
    }
    R_set_altrep_data1(x, stringVec);
    return DATAPTR(stringVec);
}
const void *string_dataset_altrep_dataptr_or_null(SEXP x)
{
    return NULL;
}

SEXP string_dataset_altrep_elt(SEXP x, R_xlen_t i)
{
    if(R_altrep_data1(x)!=R_NilValue){
        return STRING_ELT(R_altrep_data1(x),i);
    }else{ 
        H5_dataset_reader *h5_reader = (H5_dataset_reader *)R_ExternalPtrAddr(R_altrep_data2(x));
        return Rf_mkChar(h5_reader->read_str(i).c_str());
    }
}

R_altrep_class_t H5_altrep_dataset_string_class;
//[[Rcpp::init]]
void init_string_class(DllInfo *dll)
{
    char class_name[] = "H5_altrep_dataset_string";
    R_altrep_class_t &altrep_class = H5_altrep_dataset_string_class;
    altrep_class = R_make_altstring_class(class_name, PACKAGE_NAME, dll);
    R_set_altrep_Inspect_method(altrep_class, string_dataset_altrep_Inspect);
    R_set_altrep_Length_method(altrep_class, string_dataset_altrep_length);
    R_set_altvec_Dataptr_method(altrep_class, string_dataset_altrep_dataptr);
    R_set_altvec_Dataptr_or_null_method(altrep_class, string_dataset_altrep_dataptr_or_null);
    R_set_altstring_Elt_method(altrep_class, string_dataset_altrep_elt);
}

// [[Rcpp::export]]
SEXP C_make_h5_string_altrep_vector(String file_name, String dataset_name, bool transpose = false, SEXP attributes = R_NilValue)
{
    PROTECT_GUARD guard;
    H5_dataset_reader *h5_reader = new H5_dataset_reader(file_name, dataset_name);
    h5_reader->set_transpose(transpose);
    SEXP extPtr = guard.protect(Travel_shared_ptr<H5_dataset_reader>(h5_reader));
    if (h5_reader->get_data_type() != H5T_STRING)
    {
        Rf_error("The dataset is not of string type\n");
    }
    SEXP res = guard.protect(R_new_altrep(H5_altrep_dataset_string_class, R_NilValue, extPtr));
    if (attributes != R_NilValue)
    {
        SET_ATTRIB(res, attributes);
    }
    return res;
}