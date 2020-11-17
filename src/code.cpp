#include <string>
#include <Rcpp.h>
#include "H5Cpp.h"
#include "Travel/Travel.h"
#define UTILS_ENABLE_R
#include "utils.h"

using namespace Rcpp;
using namespace H5;

const H5std_string FILE_NAME("h5tutr_dset.h5");
const H5std_string DATASET_NAME("dset");
const int DIM0 = 4; // dataset dimensions
const int DIM1 = 6;
const int RANK = 2;

//[[Rcpp::export]]
void test()
{
    // Data initialization.
    int i, j;
    int data[DIM0][DIM1]; // buffer for data to write

    for (j = 0; j < DIM0; j++)
        for (i = 0; i < DIM1; i++)
            data[j][i] = i * 6 + j + 1;

    // Turn off the auto-printing when failure occurs so that we can
    // handle the errors appropriately
    Exception::dontPrint();

    // Open an existing file and dataset.
    H5File file(FILE_NAME, H5F_ACC_TRUNC);
    hsize_t dims[2]; // dataset dimensions
    dims[0] = DIM0;
    dims[1] = DIM1;
    DataSpace space(RANK, dims);
    DataSet dataset = file.createDataSet(DATASET_NAME, PredType::NATIVE_INT, space);

    // Write the data to the dataset using default memory space, file
    // space, and transfer properties.
    dataset.write(data, PredType::NATIVE_INT);
}

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
/*
Create a fake file in the mounted filesystem
*/
size_t read_int_arithmetic_sequence(const Travel_altrep_info *altrep_info, void *buffer, size_t offset, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        ((int *)buffer)[i] = offset + i;
    }
    return length;
}
// [[Rcpp::export]]
SEXP C_make_arithmetic_sequence_altrep(double n)
{
    Travel_altrep_info altrep_info;
    altrep_info.length = n;
    altrep_info.type = INTSXP;
    altrep_info.operations.get_region = read_int_arithmetic_sequence;
    SEXP x = Travel_make_altrep(altrep_info);
    return x;
}

struct H5_data
{
    H5_data(H5std_string file_name, H5std_string dataset_name)
    {
        try
        {
            file = H5File(file_name, H5F_ACC_RDONLY);
            dataset = file.openDataSet(dataset_name);
            dataspace = dataset.getSpace();
            n_dims = dataspace.getSimpleExtentNdims();
            dims = new hsize_t[n_dims];
            sub_required_length = new hsize_t[n_dims];
            sub_read_length = new hsize_t[n_dims];
            sub_start_offset = new hsize_t[n_dims];
            sub_end_offset = new hsize_t[n_dims];
            length = dataspace.getSimpleExtentNpoints();
            dataspace.getSimpleExtentDims(dims);
        }
        catch (Exception &error)
        {
            Rf_error(error.getCDetailMsg());
        }
    }
    ~H5_data()
    {
        if (dims != nullptr)
        {
            delete[] dims;
        }
        if (sub_start_offset != nullptr)
        {
            delete[] sub_start_offset;
        }
        if (sub_end_offset != nullptr)
        {
            delete[] sub_end_offset;
        }
        if (sub_required_length != nullptr)
        {
            delete[] sub_required_length;
        }
        if (sub_read_length != nullptr)
        {
            delete[] sub_read_length;
        }
    }
    H5File file;
    DataSet dataset;
    DataSpace dataspace;
    hsize_t length;
    hsize_t n_dims;
    hsize_t *dims;
    hsize_t *sub_start_offset = nullptr;
    hsize_t *sub_end_offset = nullptr;
    hsize_t *sub_required_length = nullptr;
    hsize_t *sub_read_length = nullptr;
};
void read_by_type(int type, DataSet &dataset, DataSpace &dataspace, DataSpace &memspace, void *buffer)
{
    switch (type)
    {
    case INTSXP:
        dataset.read(buffer, PredType::NATIVE_INT, memspace, dataspace);
        break;
    case REALSXP:
        dataset.read(buffer, PredType::NATIVE_DOUBLE, memspace, dataspace);
        break;
    default:
        break;
    }
}

//Get the n-dimensional offset of the subset from the 1-dimensional offset
void compute_n_dim_offset(hsize_t n_dims, hsize_t *dims, hsize_t *n_offset, hsize_t offset)
{
    hsize_t length_rest = offset;
    for (hsize_t i = 0; i < n_dims; i++)
    {
        n_offset[i] = length_rest % dims[i];
        length_rest = length_rest / dims[i];
    }
    //If length_rest is not zero, it means we will
    //read the entire data
    if(length_rest==1){
        n_offset[n_dims-1] = dims[n_dims-1];
    }
}

hsize_t get_max_read_dimension(hsize_t n_dims, hsize_t* start_off, hsize_t* end_off){
    for (hsize_t i = 0; i < n_dims; i++)
    {
        hsize_t j = n_dims - i - 1;
        if (start_off[j] != end_off[j])
        {
            return j;
        }
    }
    return 0;
}

size_t read_h5_vector(const Travel_altrep_info *altrep_info, void *buffer, size_t offset, size_t length)
{
    H5_data &data = *(H5_data *)altrep_info->private_data;
    DataSpace &dataspace = data.dataspace;
    hsize_t h5_size = length;
    DataSpace memspace(1, &h5_size, NULL);
    //Get the starting offset in the dataset
    compute_n_dim_offset(data.n_dims, data.dims, data.sub_start_offset, offset);
    compute_n_dim_offset(data.n_dims, data.dims, data.sub_end_offset, offset + length);
    size_t max_read_dim = get_max_read_dimension(data.n_dims,data.sub_start_offset,data.sub_end_offset);
    //Initialize the subset size
    for (size_t i = 0; i < data.n_dims; i++)
    {
        data.sub_read_length[i] = 1;
    }
    //We must read the marginal data first
    //The marginal data is the data that cannot be read by block
    size_t buffer_read_length = 0;
    uint8_t unit_size = get_type_size(altrep_info->type);
    char *buffer_ptr = (char *)buffer;
    for (size_t i = 0; i < max_read_dim; i++)
    {
        //If the current dimension index is not 0
        //We do the partial reading and make it zero.
        if (data.sub_start_offset[i] != 0)
        {
            data.sub_read_length[i] = data.dims[i] - data.sub_start_offset[i];
            dataspace.selectHyperslab(H5S_SELECT_SET, data.sub_read_length, data.sub_start_offset);
            memspace= DataSpace(data.n_dims, data.sub_read_length, NULL);
            read_by_type(altrep_info->type, data.dataset, dataspace, memspace, buffer_ptr + unit_size * buffer_read_length);
            buffer_read_length += dataspace.getSelectHyperNblocks();
            data.sub_start_offset[i] = 0;
            //Move the offset in the higher dimension forward by 1, check if it is out-of-bound
            ++data.sub_start_offset[i + 1];
            for (size_t j = i + 1; j < max_read_dim; j++)
            {
                if (data.sub_start_offset[j] == data.dims[j])
                {
                    data.sub_start_offset[j] = 0;
                    ++data.sub_start_offset[j + 1];
                }
                else
                {
                    break;
                }
            }
        }
        data.sub_read_length[i] = data.dims[i];
    }
    //Update the required length
    size_t new_max_read_dim = 0;
    if (max_read_dim != 0)
    {
        new_max_read_dim = get_max_read_dimension(data.n_dims,data.sub_start_offset,data.sub_end_offset);
        //Reset the read length for the dimensions that are not required
        for (size_t i = new_max_read_dim + 1; i <= max_read_dim; i++)
        {
            data.sub_read_length[i] = 1;
        }
    }
    for (size_t i = 0; i <= new_max_read_dim; i++)
    {
        size_t j = new_max_read_dim - i;
        if (data.sub_end_offset[j] != data.sub_start_offset[j])
        {
            data.sub_read_length[j] = data.sub_end_offset[j] - data.sub_start_offset[j];
            dataspace.selectHyperslab(H5S_SELECT_SET, data.sub_read_length, data.sub_start_offset);
            memspace= DataSpace(data.n_dims, data.sub_read_length, NULL);
            read_by_type(altrep_info->type, data.dataset, dataspace, memspace, buffer_ptr + unit_size * buffer_read_length);
            buffer_read_length += dataspace.getSelectHyperNblocks();
            data.sub_read_length[j] = 1;
            data.sub_start_offset[j] = data.sub_end_offset[j];
        }
    }
    return buffer_read_length;
}

// [[Rcpp::export]]
SEXP C_make_h5_altrep(int type, String file_name, String dataset_name)
{
    PROTECT_GUARD guard;

    H5_data *h5_data = new H5_data(file_name, dataset_name);
    SEXP extPtr = guard.protect(Travel_shared_ptr<H5_data>(h5_data));
    Travel_altrep_info altrep_info;
    altrep_info.length = h5_data->length;
    altrep_info.type = type;
    altrep_info.operations.get_region = read_h5_vector;
    altrep_info.private_data = h5_data;
    altrep_info.protected_data = extPtr;
    SEXP x = Travel_make_altrep(altrep_info);
    return x;
}

// [[Rcpp::export]]
SEXP C_read_h5_altrep(int type, String file_name, String dataset_name, size_t offset, size_t length)
{
    PROTECT_GUARD guard;
    H5_data *h5_data = new H5_data(file_name, dataset_name);
    SEXP extPtr = guard.protect(Travel_shared_ptr<H5_data>(h5_data));
    Travel_altrep_info altrep_info;
    altrep_info.length = h5_data->length;
    altrep_info.type = type;
    altrep_info.operations.get_region = read_h5_vector;
    altrep_info.private_data = h5_data;
    altrep_info.protected_data = extPtr;
    SEXP x = guard.protect(Rf_allocVector(type, length));
    try
    {
        read_h5_vector(&altrep_info, DATAPTR(x), offset, length);
    }
    catch (Exception &error)
    {
        Rf_error(error.getCDetailMsg());
    }
    return x;
}