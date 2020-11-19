h5Dataset<-function(file, name, type = c("auto", "integer", "numeric"), 
                    transpose = FALSE, raw = FALSE){
    type <- match.arg(type)
    if(type!="auto"){
        type_index <- get_type_index(type)
    }else{
        type_index <- 0
    }
    if(raw){
        data_attributes <- NULL
    }else{
        dims <- get_dims(file_name = file,
                         dataset_name = name)
        data_attributes <- pairlist(dim = as.integer(dims))
    }
    
    x <- C_make_h5_altrep(file_name = file,
                     dataset_name = name, 
                     type = type_index,
                     transpose = transpose,
                     attributes = data_attributes)
    x
}