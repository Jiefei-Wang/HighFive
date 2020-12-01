#ifndef HEADER_H5_TABLE_READER
#define HEADER_H5_TABLE_READER

#include "utils.h"
#include "H5_dataset_info.h"

class H5_table_reader
{
    H5_dataset_info dataset_info;
    int field_index;
    size_t H5_type_size;
    H5::DataType H5_type;
    H5T_class_t H5_class;

public:
    H5_table_reader(H5std_string file_name, H5std_string table_name, int field_index);
    H5_table_reader(H5_dataset_info dataset_info, int field_index);
    size_t get_length();
    int get_suggested_type(bool bit64conversion);
    size_t get_data_size(size_t length);
    size_t read(int R_type, void *buffer, size_t offset, size_t length);

private:
    //The buffer passed in the function must have enough size to hold the data
    size_t read_raw(void *buffer, size_t offset, size_t length);
    size_t read_numeric(int R_type, void *buffer, size_t offset, size_t length);

    void init();
};

#endif