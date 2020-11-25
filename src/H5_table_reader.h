#ifndef HEADER_H5_TABLE_READER
#define HEADER_H5_TABLE_READER

#include <memory>
#include <vector>
#include "H5Cpp.h"
#include "utils.h"
#include "H5_table_info.h"
#include "hdf5_hl.h"

class H5_table_reader
{
    H5_table_info table_info;
    std::vector<int> field_index;
public:
    H5_table_reader(H5_table_info table_info, std::vector<int> field_index) : table_info(table_info),
                                                                                               field_index(field_index){};
    size_t read(int type, void *buffer, size_t offset, size_t length);
};

#endif