#include "Rcpp.h"
#include "hdf5_hl.h"
#include "H5_table_reader.h"
#include "H5_utils.h"
#include "utils.h"

H5_table_reader::H5_table_reader(H5std_string file_name,
                                 H5std_string table_name, int field_index) : dataset_info(file_name, table_name),
                                                                             field_index(field_index)
{
    init();
}
H5_table_reader::H5_table_reader(H5_dataset_info dataset_info, int field_index) : dataset_info(dataset_info),
                                                                                  field_index(field_index)
{
    init();
}

void H5_table_reader::init()
{
    if (dataset_info.type_info.get_type_class() != H5T_COMPOUND)
    {
        Rcpp::exception("The dataset is not a table!");
    }
    H5_class = dataset_info.type_info.get_compound_info().elt_types[field_index].get_type_class();
    if (H5_class != H5T_FLOAT &&
        H5_class != H5T_INTEGER &&
        H5_class != H5T_STRING)
    {
        Rcpp::exception("Unsupported table type!");
    }
    H5_type = dataset_info.type_info.get_compound_info().elt_types[field_index].get_simple_info().type;
    H5_type_size = dataset_info.type_info.get_compound_info().elt_types[field_index].get_type_size();
}
size_t H5_table_reader::get_data_size(size_t length)
{
    return length * H5_type_size;
}
size_t H5_table_reader::read(int R_type, void *buffer, size_t offset, size_t length)
{
    if (H5_class == H5T_STRING)
    {
        return read_raw(buffer, offset, length);
    }
    else
    {
        return read_numeric(R_type, buffer, offset, length);
    }
}

size_t H5_table_reader::read_numeric(int R_type, void *buffer, size_t offset, size_t length)
{
    //Allocate the internal buffer if the read data is larger than the input buffer
    static Unique_buffer read_buffer;
    size_t required_size = get_data_size(length);
    uint8_t R_type_size = get_R_type_size(R_type);
    size_t buffer_size = length * R_type_size;
    char *internal_ptr = nullptr;
    if (required_size > buffer_size)
    {
        read_buffer.reserve(required_size);
        internal_ptr = read_buffer.get();
    }
    else
    {
        internal_ptr = (char *)buffer;
    }
    //Read raw data
    read_raw(internal_ptr, offset, length);
    //Do the type conversion
    H5::PredType R_H5_type = get_R_H5_type_id(R_type);
    H5_type.convert(R_H5_type, length, internal_ptr, NULL);
    if (internal_ptr != buffer)
    {
        memcpy(buffer, internal_ptr, R_type_size * length);
        read_buffer.release();
    }
    return length;
}
size_t H5_table_reader::read_raw(void *buffer, size_t offset, size_t length)
{
    void *internal_buffer = nullptr;
    size_t H5_elt_size = get_data_size(1);
    H5TBread_fields_index(dataset_info.file.getId(),
                          dataset_info.table_name.c_str(),
                          1, &field_index,
                          offset, length,
                          H5_elt_size,
                          0,
                          &H5_elt_size,
                          internal_buffer);

    return length;
}
size_t H5_table_reader::get_length()
{
    return dataset_info.total_length;
}

int H5_table_reader::get_suggested_type(bool bit64conversion)
{
    return get_H5_R_suggested_type(H5_class,H5_type_size,bit64conversion);
}