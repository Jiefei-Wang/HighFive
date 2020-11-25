#include <memory>
#include <Rcpp.h>
#include "H5Cpp.h"
#include "hdf5_hl.h"
#include "H5_utils.h"
#include "H5_table_info.h"

using namespace Rcpp;
using namespace H5;

H5_table_field_info get_type_details(H5::CompType &type)
{
    int n_numbers = type.getNmembers();
    H5_table_field_info info;
    info.is_compound = true;
    info.elt_types.resize(n_numbers);
    info.elt_sizes.resize(n_numbers);
    info.elt_offsets.resize(n_numbers);
    for (int i = 0; i < n_numbers; i++)
    {
        DataType elt_type = type.getMemberDataType(i);
        std::string elt_class = get_H5_type_class(elt_type);
        info.elt_types[i] = elt_class;
        info.elt_sizes[i] = elt_type.getSize();
        info.elt_offsets[i] = type.getMemberOffset(i);
        if (elt_class == "COMPOUND"){
            CompType elt_compType = type.getMemberCompType(i);
            H5_table_field_info elt_type_info = get_type_details(elt_compType);
            info.compound_list.emplace(i,elt_type_info);
        }
    }
    return info;
}

H5_table_field_info get_type_details(H5::DataSet &dataSet)
{
    DataType dataSet_type = dataSet.getDataType();
    std::string type = get_H5_type_class(dataSet_type);
    if (type != "COMPOUND")
    {
        H5_table_field_info info;
        info.is_compound = false;
        info.elt_sizes[0] = dataSet_type.getSize();
        info.elt_offsets[0] = 0;
        info.elt_types[0] = type;
        return info;
    }
    else
    {
        CompType compound_type(dataSet);
        return get_type_details(compound_type);
    }
}

H5_table_info::H5_table_info(H5std_string file_name, H5std_string table_name) : table_name(table_name)
{
    try
    {
        file = H5File(file_name, H5F_ACC_RDONLY);
        dataset = file.openDataSet(table_name);
        field_info = get_type_details(dataset);
        H5TBget_table_info(file.getId(), table_name.c_str(), &n_field, &n_record);
        char **names_out = new char *[n_field];
        for (hsize_t i = 0; i < n_field; i++)
        {
            names_out[i] = new char[field_name_max_len];
        }
        size_t* field_sizes = new size_t[n_field];
        size_t* field_offsets = new size_t[n_field];
        size_t compound_element_size;
        H5TBget_field_info(file.getId(), table_name.c_str(),
                           names_out, field_sizes, field_offsets, &compound_element_size);
        field_names.reserve(n_field);
        for (hsize_t i = 0; i < n_field; i++)
        {
            field_names[i] = names_out[i];
        }
        for (hsize_t i = 0; i < n_field; i++)
        {
            delete[] names_out[i];
        }
        delete[] names_out;
        delete[] field_sizes;
        delete[] field_offsets;

    }
    catch (Exception &error)
    {
        throw Rcpp::exception(error.getCDetailMsg());
    }
}


//[[Rcpp::export]]
void print_table_fields(String file_name, String dataset_name){
    H5_table_info table_info(file_name,dataset_name);
    for(size_t i = 0;i<table_info.n_field;i++){
        Rprintf("Field name: %s\n", table_info.field_names[i].c_str());
        Rprintf("Field type: %s\n", table_info.field_info.elt_types[i].c_str());
    }
}