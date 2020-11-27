#ifndef HEADER_H5_TABLE_INFO
#define HEADER_H5_TABLE_INFO

#include <memory>
#include <map>
#include <Rcpp.h>
#include "H5Cpp.h"

struct H5_table_field_info{
    //If is_compound = false, element type size and offset are length 1 vector
    bool is_compound;
    std::vector<H5::DataType> elt_H5_types; 
    std::vector<std::string> elt_types;
    std::vector<size_t> elt_sizes;
    std::vector<size_t> elt_offsets;
    std::map<size_t, H5_table_field_info> compound_list;
};


class H5_table_info
{
    static const size_t field_name_max_len = 255;
public:
    H5::H5File file;
    H5::DataSet dataset;
    H5std_string table_name;
    size_t compound_size;
    hsize_t n_field;
    hsize_t n_record;
    H5_table_field_info field_info;
    std::vector<std::string> field_names;
public:
    H5_table_info(H5std_string file_name, H5std_string table_name);
};


#endif