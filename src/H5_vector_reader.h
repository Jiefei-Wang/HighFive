#ifndef HEADER_H5_VECTOR_READER
#define HEADER_H5_VECTOR_READER
#include "H5Cpp.h"
#include <vector>

class H5_vector_reader
{
public:
    H5_vector_reader(H5std_string file_name, H5std_string dataset_name);
    ~H5_vector_reader();
    void set_transpose(bool value);
    size_t read(int type, void *buffer, size_t offset, size_t length);
    void set_exception(bool value);
    hsize_t get_length();
private:
    H5::H5File file;
    H5::DataSet dataset;
    H5::DataSpace dataspace;
    hsize_t length;
    hsize_t n_dims;
    std::vector<hsize_t> dims;
    std::vector<hsize_t> sub_start_offset;
    std::vector<hsize_t> sub_end_offset;
    std::vector<hsize_t> sub_read_length;
    std::vector<hsize_t> sub_transposed_start_offset;
    std::vector<hsize_t> sub_transposed_end_offset;
    bool transposed = false;
    bool throw_exception = false;
    size_t read_native(int type, void *buffer, size_t offset, size_t length);
    size_t read_transposed(int type, void *buffer, size_t offset, size_t length);
};
#endif
