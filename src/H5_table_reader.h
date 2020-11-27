#ifndef HEADER_H5_TABLE_READER
#define HEADER_H5_TABLE_READER

#include "utils.h"
#include "H5_table_info.h"

class H5_table_reader
{
    H5_table_info table_info;
    int field_index;

public:
    H5_table_reader(H5std_string file_name, H5std_string table_name, int field_index);
    H5_table_reader(H5_table_info table_info, int field_index) : table_info(table_info),
                                                                 field_index(field_index){};
    size_t read(int type, void *buffer, size_t offset, size_t length);
    size_t get_length();
    int get_suggested_type();
};

#endif