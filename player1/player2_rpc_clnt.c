/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "player2_rpc.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

enum clnt_stat 
launchballp2_1(void *argp, int *clnt_res, CLIENT *clnt)
{
        return (clnt_call(clnt, LAUNCHBALLP2,
                (xdrproc_t) xdr_void, (caddr_t) argp,
                (xdrproc_t) xdr_int, (caddr_t) clnt_res,
                TIMEOUT));
}

enum clnt_stat 
getpaddle2_1(int *argp, int *clnt_res, CLIENT *clnt)
{
        return (clnt_call(clnt, GETPADDLE2,
                (xdrproc_t) xdr_int, (caddr_t) argp,
                (xdrproc_t) xdr_int, (caddr_t) clnt_res,
                TIMEOUT));
}

