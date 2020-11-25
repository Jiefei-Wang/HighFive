#include <Rcpp.h>
#include "H5Cpp.h"
#include "utils.h"
#include "hdf5_hl.h"
#include "H5_utils.h"

using namespace Rcpp;
using namespace H5;

//[[Rcpp::export]]
Rcpp::XPtr<H5::H5File> C_H5open(String file_name, bool read_only = true)
{
    unsigned int flag;
    if (read_only)
    {
        flag = H5F_ACC_RDONLY;
    }
    else
    {
        flag = H5F_ACC_RDWR;
    }
    Rcpp::XPtr<H5File> ptr(new H5File(file_name, flag), true);
    return ptr;
}
//[[Rcpp::export]]
Rcpp::XPtr<H5::DataSet> C_H5Dopen(Rcpp::XPtr<H5::H5File> file, String dataset_name)
{
    DataSet *dataset = new DataSet(file.get()->openDataSet(dataset_name));
    Rcpp::XPtr<DataSet> ptr(dataset, true);
    return ptr;
}
