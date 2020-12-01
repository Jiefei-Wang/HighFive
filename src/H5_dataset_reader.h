#ifndef HEADER_H5_VECTOR_READER
#define HEADER_H5_VECTOR_READER
#include "H5Cpp.h"
#include "H5_dataset_info.h"
#include <vector>

class H5_dataset_reader
{
public:
    H5_dataset_reader(H5std_string file_name, H5std_string dataset_name);
    void set_transpose(bool value);
    size_t read(int type, void *buffer, size_t offset, size_t length);
    std::string read_str(size_t offset);
    void set_exception(bool value);
    const size_t& get_length();
    int get_suggested_type(bool bit64conversion);
    H5T_class_t get_data_type();
    const hsize_t& get_n_dims();
    const hsize_t& get_dim(size_t i);
private:
    H5_dataset_info dataset_info;
    H5::DataSpace dataspace;
    std::vector<hsize_t> sub_start_offset;
    std::vector<hsize_t> sub_end_offset;
    std::vector<hsize_t> sub_read_length;
    std::vector<hsize_t> sub_transposed_start_offset;
    std::vector<hsize_t> sub_transposed_end_offset;
    bool transposed = false;
    bool throw_exception = false;
    void select_dataspace(size_t offset, size_t length, bool overlap_selection = false);
    size_t read_native(int type, void *buffer, size_t offset, size_t length);
    size_t read_transposed(int type, void *buffer, size_t offset, size_t length);
    void read_by_type(int type, H5::DataSpace &memspace, void *buffer);
};
#endif
