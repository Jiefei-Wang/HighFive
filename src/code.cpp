#include <string>
#include <Rcpp.h>
#include "H5Cpp.h"
#include "utils.h"
#include "H5_vector_reader.h"

using namespace Rcpp;
using namespace H5;

//[[Rcpp::export]]
SEXP read_h5_vector(int type, String file_name, String dataset_name, size_t offset, size_t length)
{
    H5std_string h5_file_name(file_name.get_cstring());
    H5std_string h5_dataset_name(dataset_name.get_cstring());
    H5File file(h5_file_name, H5F_ACC_RDONLY);
    DataSet dataset = file.openDataSet(h5_dataset_name);

    try
    {
        // Specify size and shape of subset to write.
        hsize_t h5_offset = offset;
        hsize_t h5_size = length;
        // Define Memory Dataspace. Get file dataspace and select
        // a subset from the file dataspace.
        DataSpace memspace(1, &h5_size, NULL);

        DataSpace dataspace = dataset.getSpace();
        dataspace.selectHyperslab(H5S_SELECT_SET, &h5_size, &h5_offset);

        PROTECT_GUARD guard;
        SEXP x = guard.protect(Rf_allocVector(type, length));
        switch (type)
        {
        case INTSXP:
            dataset.read(DATAPTR(x), PredType::NATIVE_INT, memspace, dataspace);
            break;
        case REALSXP:
            dataset.read(DATAPTR(x), PredType::NATIVE_DOUBLE, memspace, dataspace);
            break;
        default:
            break;
        }
        return x;
    }
    catch (Exception &error)
    {
        Rf_error(error.getCDetailMsg());
    }
}

// [[Rcpp::export]]
SEXP C_read_h5_altrep(int type, String file_name, String dataset_name, size_t offset, size_t length, bool trans = false)
{
    PROTECT_GUARD guard;
    H5_vector_reader h5_reader(file_name, dataset_name);
    h5_reader.set_transpose(trans);
    h5_reader.set_exception(true);
    SEXP x = guard.protect(Rf_allocVector(type, length));
    h5_reader.read(type, DATAPTR(x), offset, length);
    return x;
}