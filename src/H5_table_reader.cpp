#include "hdf5_hl.h"
#include "H5_table_info.h"
#include "H5_table_reader.h"
#include "H5_utils.h"
#include "utils.h"

size_t H5_table_reader::read(int R_type, void *buffer, size_t offset, size_t length)
{
    void *internal_buffer = nullptr;
    size_t &H5_elt_size = table_info.field_info.elt_sizes[field_index];
    size_t R_elt_size = get_R_type_size(R_type);
    if (H5_elt_size > R_elt_size)
    {
        internal_buffer = new char[H5_elt_size * length];
    }
    else
    {
        internal_buffer = buffer;
    }
    H5TBread_fields_index(table_info.file.getId(),
                          table_info.table_name.c_str(),
                          1, &field_index, 
                          offset, length, 
                          H5_elt_size,
                          0,
                          &H5_elt_size,
                          internal_buffer);
    H5::DataType &type = table_info.field_info.elt_H5_types[field_index];
    hid_t H5_type = get_H5_type_id(R_type);
    type.convert(H5_type, length, internal_buffer, NULL);
    if (internal_buffer != buffer)
    {
        memcpy(buffer, internal_buffer, R_elt_size * length);
        delete[] (char*)internal_buffer;
    }
    return length;
}
