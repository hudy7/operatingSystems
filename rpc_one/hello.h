/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _HELLO_H_RPCGEN
#define _HELLO_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define DISPLAY_PRG 0x0000001
#define DISPLAY_VER 1

#if defined(__STDC__) || defined(__cplusplus)
#define print_hello 1
extern  int * print_hello_1(void *, CLIENT *);
extern  int * print_hello_1_svc(void *, struct svc_req *);
extern int display_prg_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define print_hello 1
extern  int * print_hello_1();
extern  int * print_hello_1_svc();
extern int display_prg_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_HELLO_H_RPCGEN */
