#include <Rcpp.h>
#include "H5Cpp.h"
#include "H5_vector_reader.h"
#define UTILS_ENABLE_R
#include "utils.h"
using namespace H5;

H5_vector_reader::H5_vector_reader(H5std_string file_name, H5std_string dataset_name)
{
    try
    {
        file = H5File(file_name, H5F_ACC_RDONLY);
        dataset = file.openDataSet(dataset_name);
        dataspace = dataset.getSpace();
        n_dims = dataspace.getSimpleExtentNdims();
        dims = new hsize_t[n_dims];
        sub_read_length = new hsize_t[n_dims];
        sub_start_offset = new hsize_t[n_dims];
        sub_end_offset = new hsize_t[n_dims];
        if (n_dims == 2)
        {
            sub_transposed_start_offset = new hsize_t[n_dims];
            sub_transposed_end_offset = new hsize_t[n_dims];
        }
        length = dataspace.getSimpleExtentNpoints();
        dataspace.getSimpleExtentDims(dims);
    }
    catch (Exception &error)
    {
        Rf_error(error.getCDetailMsg());
    }
}

H5_vector_reader::~H5_vector_reader()
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
    if (sub_transposed_start_offset != nullptr)
    {
        delete[] sub_transposed_start_offset;
    }
    if (sub_transposed_end_offset != nullptr)
    {
        delete[] sub_transposed_end_offset;
    }
    if (sub_read_length != nullptr)
    {
        delete[] sub_read_length;
    }
}

void H5_vector_reader::set_transpose(bool value)
{
    if (n_dims == 2)
    {
        transposed = value;
    }
    else
    {
        Rcpp::exception("The dimension must be 2");
    }
}
void H5_vector_reader::set_exception(bool value)
{
    throw_exception = value;
}

size_t H5_vector_reader::read(int type, void *buffer, size_t offset, size_t length)
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

/*


*/

static void read_by_type(int type, DataSet &dataset, DataSpace &dataspace, DataSpace &memspace, void *buffer)
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
static void compute_n_dim_offset(hsize_t n_dims, hsize_t *dims, hsize_t *n_offset, hsize_t offset, bool transpose)
{
    hsize_t length_rest = offset;
    for (hsize_t i = 0; i < n_dims; i++)
    {
        hsize_t j = transpose?i:(n_dims - 1 - i);
        n_offset[j] = length_rest % dims[j];
        length_rest = length_rest / dims[j];
    }
    //If length_rest is not zero, it means we will
    //read the entire data
    if (length_rest >= 1)
    {
        n_offset[0] = dims[0];
    }
}

static hsize_t get_min_read_dimension(hsize_t n_dims, hsize_t *start_off, hsize_t *end_off)
{
    for (hsize_t i = 0; i < n_dims; i++)
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
static void move_by_one(hsize_t *dims, hsize_t *index, hsize_t moved_dim_index)
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

size_t H5_vector_reader::read_native(int type, void *buffer, size_t offset, size_t length)
{
    DataSpace memspace;
    //Get the starting offset in the dataset
    compute_n_dim_offset(n_dims, dims, sub_start_offset, offset, false);
    compute_n_dim_offset(n_dims, dims, sub_end_offset, offset + length, false);
    size_t min_read_dim = get_min_read_dimension(n_dims, sub_start_offset, sub_end_offset);
    //Initialize the subset size
    for (size_t i = 0; i < n_dims; i++)
    {
        sub_read_length[i] = 1;
    }
    size_t buffer_read_length = 0; //We must read the marginal data first
    //The marginal data is the data that cannot be read by block
    uint8_t unit_size = get_type_size(type);
    char *buffer_ptr = (char *)buffer;
    for (size_t i = min_read_dim + 1; i < n_dims; i++)
    {
        hsize_t k = n_dims - i + min_read_dim;
        //If the current dimension index is not 0
        //We do the partial reading and make it zero.
        if (sub_start_offset[k] != 0)
        {
            sub_read_length[k] = dims[k] - sub_start_offset[k];
            dataspace.selectHyperslab(H5S_SELECT_SET, sub_read_length, sub_start_offset);
            memspace = DataSpace(n_dims, sub_read_length, NULL);
            read_by_type(type, dataset, dataspace, memspace, buffer_ptr + unit_size * buffer_read_length);
            buffer_read_length += dataspace.getSelectHyperNblocks();
            sub_start_offset[k] = 0;
            //Move the offset in the higher dimension forward by 1, check if it is out-of-bound
            move_by_one(dims, sub_start_offset, k - 1);
        }
        sub_read_length[k] = dims[k];
    }
    //Update the required length
    size_t new_min_read_dim = 0;
    if (min_read_dim != 0)
    {
        new_min_read_dim = get_min_read_dimension(n_dims, sub_start_offset, sub_end_offset);
        //Reset the read length for the dimensions that are not required
        for (size_t i = min_read_dim; i < new_min_read_dim; i++)
        {
            sub_read_length[i] = 1;
        }
    }
    for (size_t i = new_min_read_dim; i < n_dims; i++)
    {
        if (sub_end_offset[i] != sub_start_offset[i])
        {
            sub_read_length[i] = sub_end_offset[i] - sub_start_offset[i];
            dataspace.selectHyperslab(H5S_SELECT_SET, sub_read_length, sub_start_offset);
            memspace = DataSpace(n_dims, sub_read_length, NULL);
            read_by_type(type, dataset, dataspace, memspace, buffer_ptr + unit_size * buffer_read_length);
            buffer_read_length += dataspace.getSelectHyperNblocks();
            sub_read_length[i] = 1;
            sub_start_offset[i] = sub_end_offset[i];
        }
    }
    return buffer_read_length;
}


static Unique_buffer transpose_buffer;
size_t H5_vector_reader::read_transposed(int type, void *buffer, size_t offset, size_t length)
{
    uint8_t type_size = get_type_size(type);
    //Get the starting offset in the dataset
    compute_n_dim_offset(n_dims, dims, sub_transposed_start_offset, offset, true);
    compute_n_dim_offset(n_dims, dims, sub_transposed_end_offset, offset + length, true);
    hsize_t max_buffer_length = sub_transposed_end_offset[1]-sub_transposed_start_offset[1]+1;
    transpose_buffer.reserve(type_size * max_buffer_length);
    char* transpose_buffer_ptr = transpose_buffer.get();
    char* buffer_ptr = (char*) buffer;
    //read the data row by row
    for(hsize_t start_dim0 = 0;start_dim0<dims[0];start_dim0++){
        hsize_t start_dim1 = sub_transposed_start_offset[0] + (start_dim0 < sub_transposed_start_offset[0]);
        if(start_dim1<=sub_transposed_end_offset[1]){
            hsize_t required_length = sub_transposed_end_offset[1]-start_dim1+1;
            read_native(type, transpose_buffer_ptr, start_dim0*dims[1]+start_dim1, required_length);
            //move the row data to the buffer
            for(hsize_t j = 0;j<required_length;j++){
                buffer_ptr[type_size*(start_dim0+(start_dim1+j)*dims[0])]= transpose_buffer_ptr[type_size*j];
            }
        }
    }
    return length;
}