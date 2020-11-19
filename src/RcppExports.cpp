// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// C_make_h5_altrep
SEXP C_make_h5_altrep(String file_name, String dataset_name, int type, bool transpose, SEXP attributes);
RcppExport SEXP _HighFive_C_make_h5_altrep(SEXP file_nameSEXP, SEXP dataset_nameSEXP, SEXP typeSEXP, SEXP transposeSEXP, SEXP attributesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< String >::type file_name(file_nameSEXP);
    Rcpp::traits::input_parameter< String >::type dataset_name(dataset_nameSEXP);
    Rcpp::traits::input_parameter< int >::type type(typeSEXP);
    Rcpp::traits::input_parameter< bool >::type transpose(transposeSEXP);
    Rcpp::traits::input_parameter< SEXP >::type attributes(attributesSEXP);
    rcpp_result_gen = Rcpp::wrap(C_make_h5_altrep(file_name, dataset_name, type, transpose, attributes));
    return rcpp_result_gen;
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

static const R_CallMethodDef CallEntries[] = {
    {"_HighFive_C_make_h5_altrep", (DL_FUNC) &_HighFive_C_make_h5_altrep, 5},
    {"_HighFive_C_read_h5_altrep", (DL_FUNC) &_HighFive_C_read_h5_altrep, 6},
    {"_HighFive_get_type_index", (DL_FUNC) &_HighFive_get_type_index, 1},
    {"_HighFive_get_dims", (DL_FUNC) &_HighFive_get_dims, 2},
    {NULL, NULL, 0}
};

RcppExport void R_init_HighFive(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
