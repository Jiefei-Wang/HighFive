#ifndef HEADER_H5_TYPE_INFO
#define HEADER_H5_TYPE_INFO
#include <vector>
#include "H5Cpp.h"

class H5_type_info;

struct Compound_info{
    H5::CompType type;
    std::vector<H5_type_info> elt_types;
    std::vector<size_t> elt_offsets;
    std::vector<H5std_string> elt_names;
    Compound_info(H5::CompType& type);
};

struct Simple_info{
    H5::DataType type;
    Simple_info(H5::DataType& type);
};

class H5_type_info{
private:
    H5T_class_t type_class;
    size_t type_size;
    void* type_object_ptr = nullptr;
public:
    H5_type_info(){}
    H5_type_info(H5::DataSet &dataset);
    H5_type_info(H5::CompType& type);
    H5_type_info(H5::DataType& type);
    H5_type_info(const H5_type_info &t);
    H5_type_info& operator = (const H5_type_info &t); 
    ~H5_type_info();
    H5T_class_t get_type_class() const;
    size_t get_type_size() const;
    Compound_info& get_compound_info() const;
    Simple_info& get_simple_info() const;
private:
    void init(H5::DataSet &dataset);
    void init(H5::CompType &type);
    void init(H5::DataType &type);
    void copy(const H5_type_info &t);
};

#endif