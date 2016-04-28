/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _MSGRPC_H_RPCGEN
#define _MSGRPC_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct data {
	int id;
	char msg[80];
};
typedef struct data data;

#define DISPLAY_PRG 0x20000001
#define DISPLAY_VER 1

#if defined(__STDC__) || defined(__cplusplus)
#define get 1
extern  int * get_1(int *, CLIENT *);
extern  int * get_1_svc(int *, struct svc_req *);
#define put 2
extern  int * put_1(struct data *, CLIENT *);
extern  int * put_1_svc(struct data *, struct svc_req *);
extern int display_prg_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define get 1
extern  int * get_1();
extern  int * get_1_svc();
#define put 2
extern  int * put_1();
extern  int * put_1_svc();
extern int display_prg_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_data (XDR *, data*);

#else /* K&R C */
extern bool_t xdr_data ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_MSGRPC_H_RPCGEN */
