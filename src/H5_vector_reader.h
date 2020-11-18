#ifndef HEADER_H5_VECTOR_READER
#define HEADER_H5_VECTOR_READER
#include "H5Cpp.h"

class H5_vector_reader
{
public:
    H5_vector_reader(H5std_string file_name, H5std_string dataset_name);
    ~H5_vector_reader();
    void set_transpose(bool value);
    size_t read(int type, void *buffer, size_t offset, size_t length);
    hsize_t length;
    void set_exception(bool value);
private:
    H5::H5File file;
    H5::DataSet dataset;
    H5::DataSpace dataspace;
    hsize_t n_dims;
    hsize_t *dims;
    hsize_t *sub_start_offset = nullptr;
    hsize_t *sub_end_offset = nullptr;
    hsize_t *sub_read_length = nullptr;
    hsize_t *sub_transposed_start_offset = nullptr;
    hsize_t *sub_transposed_end_offset = nullptr;
    bool transposed = false;
    bool throw_exception = false;
    size_t read_native(int type, void *buffer, size_t offset, size_t length);
    size_t read_transposed(int type, void *buffer, size_t offset, size_t length);
};
#endif
