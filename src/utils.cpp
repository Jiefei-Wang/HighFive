#include <stdint.h>
#include <Rcpp.h>
uint8_t get_R_type_size(int type)
{
	uint8_t elt_size = 0;
	switch (type)
	{
	case INTSXP:
		elt_size = sizeof(int);
		break;
	case LGLSXP:
		elt_size = sizeof(int);
		break;
	case REALSXP:
		elt_size = sizeof(double);
		break;
	case RAWSXP:
		elt_size = 1;
		break;
	default:
		assert(!"Unknown type");
	}
	return elt_size;
}