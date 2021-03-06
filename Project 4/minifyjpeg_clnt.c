/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "minifyjpeg.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

minifyjpeg_res *
minify_jpeg_proc_1(jpeg_in arg1,  CLIENT *clnt)
{
	static minifyjpeg_res clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, MINIFY_JPEG_PROC,
		(xdrproc_t) xdr_jpeg_in, (caddr_t) &arg1,
		(xdrproc_t) xdr_minifyjpeg_res, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
