/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "minifyjpeg.h"

bool_t
xdr_jpeg_in (XDR *xdrs, jpeg_in *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->arg1, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_jpeg_out (XDR *xdrs, jpeg_out *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->res1, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}
