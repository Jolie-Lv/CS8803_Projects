#include "minifyjpeg.h"
#include "magickminify.h"
#include <string.h>

/* Implement the needed server-side functions here */
minifyjpeg_res *minify_jpeg_proc_1_svc(jpeg_in src_jpeg, struct svc_req *req){
    static minifyjpeg_res res;
    void *src_val = src_jpeg.src_jpeg_val.src_jpeg_val_val;
    ssize_t src_len = src_jpeg.src_jpeg_val.src_jpeg_val_len;
    void *dst_val;
    
    //printf("Server started... waiting for request...\n");
    
    magickminify_init();
    
    
    dst_val = magickminify(src_val, src_len, (ssize_t*)src_jpeg.dst_len);
    
    if (dst_val == (void*)NULL) {
        printf("magickminify() returned NULL");
        exit(1);
    }

    //printf("modified picture length is %d.\n", *src_jpeg.dst_len);
    
    res.minifyjpeg_res_u.minified_jpeg_val.minified_jpeg_val_val = dst_val;
    res.minifyjpeg_res_u.minified_jpeg_val.minified_jpeg_val_len = (u_int)*src_jpeg.dst_len;
    
    if (res.minifyjpeg_res_u.minified_jpeg_val.minified_jpeg_val_val == (char*)NULL) {
        printf("result jgep val is still NULL, didn't get data from dst_val\n");
        exit(1);
    }
    
    return &res;
}

int minify_jpeg_prog_1_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result){
    magickminify_cleanup();
    (void) xdr_free(xdr_result, result);
    return 0;
}
