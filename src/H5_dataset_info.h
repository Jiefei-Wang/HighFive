#ifndef HEADER_H5_DATASET_INFO
#define HEADER_H5_DATASET_INFO
#include <vector>
#include "H5Cpp.h"
#include "H5_type_info.h"

class H5_dataset_info
{
public:
    H5::H5File file;
    H5::DataSet dataset;
    H5std_string table_name;
    size_t total_length;
    hsize_t n_dims;
    std::vector<hsize_t> dims;
    H5_type_info type_info;
public:
    H5_dataset_info(H5std_string file_name, H5std_string table_name);
};


#endif