h5Dataset<-function(file, name, bit64conversion = TRUE,
                    transpose = FALSE, raw = FALSE){
    H5_type <- C_get_H5_type(file, name)
    if(H5_type=="INTEGER"||H5_type=="FLOAT"){
        x <- h5Atomic(file, name, bit64conversion = bit64conversion,
                      transpose = transpose, raw = raw)
        return(x)
    }
    if(H5_type == "COMPOUND"){
        x <- h5List(file, name, bit64conversion = bit64conversion,
                      raw = raw)
        return(x)
    }
    stop("Unsupported data type: <", H5_type, ">")
}


h5Atomic<- function(file, name, bit64conversion = TRUE,
                    transpose = FALSE, raw = FALSE){
    type_index <- 0
    if(raw){
        data_attributes <- NULL
    }else{
        dims <- get_dims(file_name = file,
                         dataset_name = name)
        if(transpose){
            dim <- rev(dim)
        }
        data_attributes <- pairlist(dim = as.integer(dims))
    }
    
    x <- C_make_h5_altrep_vector(file_name = file,
                                 dataset_name = name, 
                                 type = type_index,
                                 transpose = transpose,
                                 attributes = data_attributes,
                                 bit64conversion = bit64conversion)
    x
}



h5List <- function(file, name, bit64conversion = TRUE, raw = FALSE){
    x <- C_make_h5_altrep_table(file_name=file,
                                dataset_name=name,
                                bit64conversion=bit64conversion)
    if(!raw){
        x <- as.data.frame(x)
    }
    x
}