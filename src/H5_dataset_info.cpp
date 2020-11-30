#include "H5_dataset_info.h"
#include "Rcpp.h"

using namespace H5;

H5_dataset_info::H5_dataset_info(H5std_string file_name, H5std_string table_name) : table_name(table_name)
{
    try
    {
        file = H5File(file_name, H5F_ACC_RDONLY);
        dataset = file.openDataSet(table_name);
        type_info = H5_type_info(dataset);
        DataSpace dataspace = dataset.getSpace();
        total_length = dataspace.getSimpleExtentNpoints();
        n_dims = dataspace.getSimpleExtentNdims();
        dims.resize(n_dims);
        dataspace.getSimpleExtentDims(dims.data());
    }
    catch (Exception &error)
    {
        //std::string msg = error.getMajorString()+"\n"+error.getMinorString()+"\n"+error.getCDetailMsg();
        throw Rcpp::exception(error.getCDetailMsg());
    }
}

