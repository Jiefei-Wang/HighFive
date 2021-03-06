# Generated by using Rcpp::compileAttributes() -> do not edit by hand
# Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

C_make_h5_altrep_table <- function(file_name, dataset_name, bit64conversion = FALSE) {
    .Call(`_HighFive_C_make_h5_altrep_table`, file_name, dataset_name, bit64conversion)
}

C_make_h5_altrep_vector <- function(file_name, dataset_name, type = 0L, transpose = FALSE, attributes = NULL, bit64conversion = FALSE) {
    .Call(`_HighFive_C_make_h5_altrep_vector`, file_name, dataset_name, type, transpose, attributes, bit64conversion)
}

C_make_h5_vector_string_altrep <- function(file_name, dataset_name, transpose = FALSE, attributes = NULL) {
    .Call(`_HighFive_C_make_h5_vector_string_altrep`, file_name, dataset_name, transpose, attributes)
}

C_H5open <- function(file_name, read_only = TRUE) {
    .Call(`_HighFive_C_H5open`, file_name, read_only)
}

C_H5Dopen <- function(file, dataset_name) {
    .Call(`_HighFive_C_H5Dopen`, file, dataset_name)
}

C_read_dataset_test <- function(file_name, dataset_name, offset, length, trans = FALSE, type = 0L) {
    .Call(`_HighFive_C_read_dataset_test`, file_name, dataset_name, offset, length, trans, type)
}

get_type_index <- function(type_name) {
    .Call(`_HighFive_get_type_index`, type_name)
}

get_dims <- function(file_name, dataset_name) {
    .Call(`_HighFive_get_dims`, file_name, dataset_name)
}

C_get_H5_type <- function(file_name, dataset_name) {
    .Call(`_HighFive_C_get_H5_type`, file_name, dataset_name)
}

test1 <- function(file_name, table_name) {
    invisible(.Call(`_HighFive_test1`, file_name, table_name))
}

C_read_h5_string <- function(file_name, dataset_name, offset) {
    .Call(`_HighFive_C_read_h5_string`, file_name, dataset_name, offset)
}

read_table_test <- function(file_name, dataset_name, bit64conversion = FALSE) {
    .Call(`_HighFive_read_table_test`, file_name, dataset_name, bit64conversion)
}

