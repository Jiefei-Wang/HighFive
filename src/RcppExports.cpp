// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "HighFive_types.h"
#include <Rcpp.h>

using namespace Rcpp;

// C_make_h5_altrep_table
SEXP C_make_h5_altrep_table(String file_name, String table_name);
RcppExport SEXP _HighFive_C_make_h5_altrep_table(SEXP file_nameSEXP, SEXP table_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< String >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< String >::type table_name(table_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(C_make_h5_altrep_table(file_name, table_name));
    return rcpp_result_gen;
END_RCPP
}
// C_make_h5_altrep_vector
SEXP C_make_h5_altrep_vector(String file_name, String dataset_name, int type, bool transpose, SEXP attributes);
RcppExport SEXP _HighFive_C_make_h5_altrep_vector(SEXP file_nameSEXP, SEXP dataset_nameSEXP, SEXP typeSEXP, SEXP transposeSEXP, SEXP attributesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< String >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< String >::type dataset_name(dataset_nameSEXP);
    Rcpp::traits::input_parameter< int >::type type(typeSEXP);
    Rcpp::traits::input_parameter< bool >::type transpose(transposeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type attributes(attributesSEXP);
    rcpp_result_gen = Rcpp::wrap(C_make_h5_altrep_vector(file_name, dataset_name, type, transpose, attributes));
    return rcpp_result_gen;
END_RCPP
}
// C_H5open
Rcpp::XPtr<H5::H5File> C_H5open(String file_name, bool read_only);
RcppExport SEXP _HighFive_C_H5open(SEXP file_nameSEXP, SEXP read_onlySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< String >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< bool >::type read_only(read_onlySEXP);
    rcpp_result_gen = Rcpp::wrap(C_H5open(file_name, read_only));
    return rcpp_result_gen;
END_RCPP
}
// C_H5Dopen
Rcpp::XPtr<H5::DataSet> C_H5Dopen(Rcpp::XPtr<H5::H5File> file, String dataset_name);
RcppExport SEXP _HighFive_C_H5Dopen(SEXP fileSEXP, SEXP dataset_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::XPtr<H5::H5File> >::type file(fileSEXP);
    Rcpp::traits::input_parameter< String >::type dataset_name(dataset_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(C_H5Dopen(file, dataset_name));
    return rcpp_result_gen;
END_RCPP
}
// print_table_fields
void print_table_fields(String file_name, String dataset_name);
RcppExport SEXP _HighFive_print_table_fields(SEXP file_nameSEXP, SEXP dataset_nameSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< String >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< String >::type dataset_name(dataset_nameSEXP);
    print_table_fields(file_name, dataset_name);
    return R_NilValue;
END_RCPP
}
// C_read_h5_altrep
SEXP C_read_h5_altrep(String file_name, String dataset_name, size_t offset, size_t length, bool trans, int type);
RcppExport SEXP _HighFive_C_read_h5_altrep(SEXP file_nameSEXP, SEXP dataset_nameSEXP, SEXP offsetSEXP, SEXP lengthSEXP, SEXP transSEXP, SEXP typeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< String >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< String >::type dataset_name(dataset_nameSEXP);
    Rcpp::traits::input_parameter< size_t >::type offset(offsetSEXP);
    Rcpp::traits::input_parameter< size_t >::type length(lengthSEXP);
    Rcpp::traits::input_parameter< bool >::type trans(transSEXP);
    Rcpp::traits::input_parameter< int >::type type(typeSEXP);
    rcpp_result_gen = Rcpp::wrap(C_read_h5_altrep(file_name, dataset_name, offset, length, trans, type));
    return rcpp_result_gen;
END_RCPP
}
// get_type_index
int get_type_index(String type_name);
RcppExport SEXP _HighFive_get_type_index(SEXP type_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< String >::type type_name(type_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(get_type_index(type_name));
    return rcpp_result_gen;
END_RCPP
}
// get_dims
SEXP get_dims(String file_name, String dataset_name);
RcppExport SEXP _HighFive_get_dims(SEXP file_nameSEXP, SEXP dataset_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< String >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< String >::type dataset_name(dataset_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(get_dims(file_name, dataset_name));
    return rcpp_result_gen;
END_RCPP
}
// test
SEXP test(String file_name, String table_name);
RcppExport SEXP _HighFive_test(SEXP file_nameSEXP, SEXP table_nameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< String >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< String >::type table_name(table_nameSEXP);
    rcpp_result_gen = Rcpp::wrap(test(file_name, table_name));
    return rcpp_result_gen;
END_RCPP
}
// test_table_read
SEXP test_table_read(int type, String file_name, String table_name, int field_index);
RcppExport SEXP _HighFive_test_table_read(SEXP typeSEXP, SEXP file_nameSEXP, SEXP table_nameSEXP, SEXP field_indexSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type type(typeSEXP);
    Rcpp::traits::input_parameter< String >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< String >::type table_name(table_nameSEXP);
    Rcpp::traits::input_parameter< int >::type field_index(field_indexSEXP);
    rcpp_result_gen = Rcpp::wrap(test_table_read(type, file_name, table_name, field_index));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_HighFive_C_make_h5_altrep_table", (DL_FUNC) &_HighFive_C_make_h5_altrep_table, 2},
    {"_HighFive_C_make_h5_altrep_vector", (DL_FUNC) &_HighFive_C_make_h5_altrep_vector, 5},
    {"_HighFive_C_H5open", (DL_FUNC) &_HighFive_C_H5open, 2},
    {"_HighFive_C_H5Dopen", (DL_FUNC) &_HighFive_C_H5Dopen, 2},
    {"_HighFive_print_table_fields", (DL_FUNC) &_HighFive_print_table_fields, 2},
    {"_HighFive_C_read_h5_altrep", (DL_FUNC) &_HighFive_C_read_h5_altrep, 6},
    {"_HighFive_get_type_index", (DL_FUNC) &_HighFive_get_type_index, 1},
    {"_HighFive_get_dims", (DL_FUNC) &_HighFive_get_dims, 2},
    {"_HighFive_test", (DL_FUNC) &_HighFive_test, 2},
    {"_HighFive_test_table_read", (DL_FUNC) &_HighFive_test_table_read, 4},
    {NULL, NULL, 0}
};

RcppExport void R_init_HighFive(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
