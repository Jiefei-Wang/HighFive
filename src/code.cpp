#include <Rcpp.h>
using namespace Rcpp;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 *  This example illustrates how to write to and read from an existing
 *  dataset. It is used in the HDF5 Tutorial.
 */

#include <iostream>
#include <string>
#include "H5Cpp.h"
using namespace H5;

const H5std_string FILE_NAME("h5tutr_dset.h5");
const H5std_string DATASET_NAME("dset");
const int DIM0 = 4; // dataset dimensions
const int DIM1 = 6;
const int RANK = 2;

//[[Rcpp::export]]
void test()
{
    // Data initialization.

    int i, j;
    int data[DIM0][DIM1]; // buffer for data to write

    for (j = 0; j < DIM0; j++)
        for (i = 0; i < DIM1; i++)
            data[j][i] = i * 6 + j + 1;

    // Turn off the auto-printing when failure occurs so that we can
    // handle the errors appropriately
    Exception::dontPrint();

    // Open an existing file and dataset.
    H5File file(FILE_NAME, H5F_ACC_TRUNC);
    hsize_t dims[2]; // dataset dimensions
    dims[0] = DIM0;
    dims[1] = DIM1;
    DataSpace space(RANK, dims);
    DataSet dataset = file.createDataSet(DATASET_NAME, PredType::NATIVE_INT,space);

    // Write the data to the dataset using default memory space, file
    // space, and transfer properties.
    dataset.write(data, PredType::NATIVE_INT);
}