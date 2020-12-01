#include <Rcpp.h>
#include "H5Cpp.h"
#include "H5_dataset_reader.h"
#define UTILS_ENABLE_R
#include "utils.h"
#include "H5_utils.h"

using namespace H5;

typedef std::vector<hsize_t> hsize_vec;

/*
========================================================
Utilities
========================================================
*/
//Get the n-dimensional offset of the subset from the 1-dimensional offset
static void compute_n_dim_offset(hsize_vec &dims, hsize_vec &n_offset, hsize_t offset, bool transpose)
{
    size_t n_dims = dims.size();
    hsize_t length_rest = offset;
    for (size_t i = 0; i < n_dims; i++)
    {
        size_t j = transpose ? i : (n_dims - 1 - i);
        n_offset[j] = length_rest % dims[j];
        length_rest = length_rest / dims[j];
    }
    //If length_rest is not zero, it means we will
    //read the entire data
    if (length_rest >= 1)
    {
        if (!transpose)
        {
            n_offset[0] = dims[0];
        }
        else
        {
            n_offset[n_dims - 1] = dims[n_dims - 1];
        }
    }
}
//Get the offset in the vector based on the 2-dimensional offset (x,y)
static size_t compute_offset(hsize_vec &dims, hsize_t x, hsize_t y, bool transpose)
{
    if (transpose)
    {
        return x + y * dims[0];
    }
    else
    {
        return x * dims[1] + y;
    }
}

static hsize_t get_min_read_dimension(hsize_vec &start_off, hsize_vec &end_off)
{
    size_t n_dims = start_off.size();
    for (size_t i = 0; i < n_dims; i++)
    {
        if (start_off[i] != end_off[i])
        {
            return i;
        }
    }
    return 0;
}
//Move the index by one unit in along a specific dimension
//If the index is out-off-bound, add 1 to the next dimension
static void move_by_one(hsize_vec &dims, hsize_vec &index, hsize_t moved_dim_index)
{
    ++index[moved_dim_index];
    for (size_t j = moved_dim_index; j != 0; j--)
    {
        if (index[j] == dims[j])
        {
            index[j] = 0;
            ++index[j - 1];
        }
        else
        {
            break;
        }
    }
}

/*
========================================================
Class member functions
========================================================
*/
H5_dataset_reader::H5_dataset_reader(H5std_string file_name, H5std_string dataset_name) : dataset_info(file_name, dataset_name)
{
    try
    {
        dataspace = dataset_info.dataset.getSpace();
        sub_read_length.resize(get_n_dims());
        sub_start_offset.resize(get_n_dims());
        sub_end_offset.resize(get_n_dims());
    }
    catch (Exception &error)
    {
        Rf_error(error.getCDetailMsg());
    }
}

void H5_dataset_reader::set_transpose(bool value)
{
    if (get_n_dims() == 2)
    {
        transposed = value;
        if (transposed)
        {
            sub_transposed_start_offset.resize(get_n_dims());
            sub_transposed_end_offset.resize(get_n_dims());
        }
    }
    else
    {
        Rcpp::exception("The dimension must be 2");
    }
}
void H5_dataset_reader::set_exception(bool value)
{
    throw_exception = value;
}

const hsize_t &H5_dataset_reader::get_length()
{
    return dataset_info.total_length;
}

int H5_dataset_reader::get_suggested_type()
{
    H5T_class_t data_type = dataset_info.type_info.get_type_class();
    return get_H5_R_suggested_type(data_type);
}
const hsize_t &H5_dataset_reader::get_n_dims()
{
    return dataset_info.n_dims;
}

const hsize_t &H5_dataset_reader::get_dim(size_t i)
{
    return dataset_info.dims.at(i);
}

size_t H5_dataset_reader::read(int type, void *buffer, size_t offset, size_t length)
{
    try
    {
        if (!transposed)
        {
            return read_native(type, buffer, offset, length);
        }
        else
        {
            return read_transposed(type, buffer, offset, length);
        }
    }
    catch (Exception &error)
    {
        if (throw_exception)
            throw Rcpp::exception(error.getCDetailMsg());
    }
    return 0;
}
std::string H5_dataset_reader::read_str(size_t offset)
{
    if (dataset_info.type_info.get_type_class() != H5T_STRING)
    {
        Rcpp::exception("The data type is not string");
    }
    H5std_string buf;
    select_dataspace(offset, 1);
    DataSpace memspace(H5S_SCALAR);
    StrType datatype(dataset_info.dataset);
    dataset_info.dataset.read(buf, datatype, memspace, dataspace);
    return buf;
}

void H5_dataset_reader::read_by_type(int type, DataSpace &memspace, void *buffer)
{
    switch (type)
    {
    case INTSXP:
        dataset_info.dataset.read(buffer, PredType::NATIVE_INT, memspace, dataspace);
        break;
    case REALSXP:
        dataset_info.dataset.read(buffer, PredType::NATIVE_DOUBLE, memspace, dataspace);
        break;
    case RAWSXP:
        dataset_info.dataset.read(buffer, PredType::NATIVE_INT8, memspace, dataspace);
        break;
    case LGLSXP:
        dataset_info.dataset.read(buffer, PredType::NATIVE_INT, memspace, dataspace);
        break;
    default:
        Rcpp::exception("unsupported data type in <read_by_type>");
        break;
    }
}

//Select the dataspace by specifying the offset and length
void H5_dataset_reader::select_dataspace(size_t offset, size_t length, bool overlap_selection)
{
    if (!overlap_selection)
    {
        dataspace.selectNone();
    }
    if (length == 0)
    {
        return;
    }
    //Get the starting offset in the dataset
    compute_n_dim_offset(dataset_info.dims, sub_start_offset, offset, false);
    compute_n_dim_offset(dataset_info.dims, sub_end_offset, offset + length, false);
    size_t min_read_dim = get_min_read_dimension(sub_start_offset, sub_end_offset);
    //Initialize the subset size
    for (size_t i = 0; i < get_n_dims(); i++)
    {
        sub_read_length[i] = 1;
    }
    size_t buffer_read_length = 0;
    //We select the marginal data first
    //The marginal data is the data that cannot be read by block
    for (size_t i = min_read_dim + 1; i < get_n_dims(); i++)
    {
        hsize_t k = get_n_dims() - i + min_read_dim;
        //If the current dimension index is not 0
        //We do the partial reading and make it zero.
        if (sub_start_offset[k] != 0)
        {
            sub_read_length[k] = get_dim(k) - sub_start_offset[k];
            dataspace.selectHyperslab(H5S_SELECT_OR, sub_read_length.data(), sub_start_offset.data());
            sub_start_offset[k] = 0;
            //Move the offset in the higher dimension forward by 1, check if it is out-of-bound
            move_by_one(dataset_info.dims, sub_start_offset, k - 1);
        }
        sub_read_length[k] = get_dim(k);
    }
    //Update the required length
    size_t new_min_read_dim = 0;
    if (min_read_dim != 0)
    {
        new_min_read_dim = get_min_read_dimension(sub_start_offset, sub_end_offset);
        //Reset the read length for the dimensions that are not required
        for (size_t i = min_read_dim; i < new_min_read_dim; i++)
        {
            sub_read_length[i] = 1;
        }
    }
    for (size_t i = new_min_read_dim; i < get_n_dims(); i++)
    {
        if (sub_end_offset[i] != sub_start_offset[i])
        {
            sub_read_length[i] = sub_end_offset[i] - sub_start_offset[i];
            dataspace.selectHyperslab(H5S_SELECT_OR, sub_read_length.data(), sub_start_offset.data());
            buffer_read_length += dataspace.getSelectHyperNblocks();
            sub_read_length[i] = 1;
            sub_start_offset[i] = sub_end_offset[i];
        }
    }
}

size_t H5_dataset_reader::read_native(int type, void *buffer, size_t offset, size_t length)
{
    //Initial the memory space and the data space
    hsize_t mem_length = length;
    DataSpace memspace(1, &mem_length, NULL);
    select_dataspace(offset, length);
    read_by_type(type, memspace, buffer);
    return length;
}

static Unique_buffer transpose_buffer;
size_t H5_dataset_reader::read_transposed(int type, void *buffer, size_t offset, size_t length)
{
    select_dataspace(0, 0, false);
    uint8_t type_size = get_R_type_size(type);
    transpose_buffer.reserve(type_size * length);
    //Get the starting offset in the dataset
    compute_n_dim_offset(dataset_info.dims, sub_transposed_start_offset, offset, true);
    compute_n_dim_offset(dataset_info.dims, sub_transposed_end_offset, offset + length, true);
    char *transpose_buffer_ptr = transpose_buffer.get();
    char *buffer_ptr = (char *)buffer;
    //Select the data row by row
    for (hsize_t start_dim0 = 0; start_dim0 < get_dim(0); start_dim0++)
    {
        hsize_t start_dim1 = sub_transposed_start_offset[1] + (start_dim0 < sub_transposed_start_offset[0]);
        if (start_dim1 <= sub_transposed_end_offset[1])
        {
            hsize_t required_length = 1 + sub_transposed_end_offset[1] -
                                      (start_dim0 >= sub_transposed_end_offset[0]) - start_dim1;
            select_dataspace(
                compute_offset(dataset_info.dims, start_dim0, start_dim1, false),
                required_length, true);
        }
    }
    hsize_t mem_length = dataspace.getSelectNpoints();
    DataSpace memspace(1, &mem_length, NULL);
    read_by_type(type, memspace, transpose_buffer_ptr);
    for (hsize_t start_dim0 = 0; start_dim0 < get_dim(0); start_dim0++)
    {
        hsize_t start_dim1 = sub_transposed_start_offset[1] + (start_dim0 < sub_transposed_start_offset[0]);
        if (start_dim1 <= sub_transposed_end_offset[1])
        {
            hsize_t required_length = 1 + sub_transposed_end_offset[1] -
                                      (start_dim0 >= sub_transposed_end_offset[0]) - start_dim1;
            for (hsize_t j = 0; j < required_length; j++)
            {
                size_t row_major_length = compute_offset(dataset_info.dims, start_dim0, j, true);
                size_t col_major_length = compute_offset(dataset_info.dims, start_dim0, j, false);
                memcpy(buffer_ptr + type_size * col_major_length,
                       transpose_buffer_ptr + type_size * row_major_length,
                       type_size);
            }
        }
    }

    transpose_buffer.release();
    return length;
}