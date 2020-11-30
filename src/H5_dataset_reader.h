#ifndef HEADER_H5_VECTOR_READER
#define HEADER_H5_VECTOR_READER
#include "H5Cpp.h"
#include <vector>

class H5_dataset_reader
{
public:
    H5_dataset_reader(H5std_string file_name, H5std_string dataset_name);
    void set_transpose(bool value);
    size_t read(int type, void *buffer, size_t offset, size_t length);
    void set_exception(bool value);
    hsize_t get_length();
    int get_suggested_type();
    hsize_t get_n_dims();
    hsize_t get_dim(size_t i);
private:
    H5::H5File file;
    H5::DataSet dataset;
    H5::DataSpace dataspace;
    hsize_t total_length;
    hsize_t n_dims;
    std::vector<hsize_t> dims;
    std::vector<hsize_t> sub_start_offset;
    std::vector<hsize_t> sub_end_offset;
    std::vector<hsize_t> sub_read_length;
    std::vector<hsize_t> sub_transposed_start_offset;
    std::vector<hsize_t> sub_transposed_end_offset;
    bool transposed = false;
    bool throw_exception = false;
    void select_dataspace(size_t offset, size_t length);
    size_t read_native(int type, void *buffer, size_t offset, size_t length);
    size_t read_transposed(int type, void *buffer, size_t offset, size_t length);
};
#endif
