/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "as7.h"

bool_t
xdr_data (XDR *xdrs, data *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_int (xdrs, &objp->id))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->message, 2000,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}
