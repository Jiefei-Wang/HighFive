#include "H5_type_info.h"
#include "Rcpp.h"

Compound_info::Compound_info(H5::CompType &type) : type(type)
{
    int n_numbers = type.getNmembers();
    elt_types.reserve(n_numbers);
    elt_offsets.reserve(n_numbers);
    elt_names.reserve(n_numbers);
    for (int i = 0; i < n_numbers; i++)
    {
        H5::DataType elt_type = type.getMemberDataType(i);
        elt_offsets.push_back(type.getMemberOffset(i));
        elt_names.push_back(type.getMemberName(i));
        if (elt_type.getClass() == H5T_COMPOUND)
        {
            H5::CompType elt_type=type.getMemberCompType(i);
            H5_type_info elt_type_info(elt_type);
            elt_types.push_back(elt_type_info);
        }
        else
        {
            H5_type_info elt_type_info(elt_type);
            elt_types.push_back(elt_type_info);
        }
    }
}

Simple_info::Simple_info(H5::DataType &type) : type(type)
{
}

H5T_class_t H5_type_info::get_type_class()
{
    return type_class;
}

H5_type_info::H5_type_info(H5::DataSet &dataset)
{
    init(dataset);
}

H5_type_info::H5_type_info(H5::DataType &type)
{
    init(type);
}

H5_type_info::H5_type_info(H5::CompType &type)
{
    init(type);
}

void H5_type_info::init(H5::DataSet &dataset)
{
    switch (dataset.getTypeClass())
    {
    case H5T_COMPOUND:
    {
        H5::CompType type = dataset.getCompType();
        init(type);
        break;
    }
    default:
        H5::DataType type = dataset.getDataType();
        init(type);
        break;
    }
}

void H5_type_info::init(H5::CompType &type)
{
    type_object_ptr = new Compound_info(type);
    type_size = type.getSize();
    type_class = type.getClass();
}

void H5_type_info::init(H5::DataType &type)
{

    type_object_ptr = new Simple_info(type);
    type_size = type.getSize();
    type_class = type.getClass();
}

H5_type_info::~H5_type_info()
{
    if (type_object_ptr != nullptr)
    {
        switch (type_class)
        {
        case H5T_COMPOUND:
            delete (Compound_info *)type_object_ptr;
            break;
        default:
            delete (Simple_info *)type_object_ptr;
            break;
        }
    }
}

size_t H5_type_info::get_type_size()
{
    return type_size;
}
Compound_info &H5_type_info::get_compound_info()
{
    if (get_type_class() != H5T_COMPOUND)
    {
        throw Rcpp::exception("The data is not of compound type!");
    }
    return *(Compound_info *)type_object_ptr;
}
Simple_info &H5_type_info::get_simple_info()
{
    if (get_type_class() == H5T_COMPOUND)
    {
        throw Rcpp::exception("The data is not of simple type!");
    }
    return *(Simple_info *)type_object_ptr;
}