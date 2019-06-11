#ifndef CYGONCE_NET_USOCKETCLI_H
#define CYGONCE_NET_USOCKETCLI_H
/* =================================================================
 *
 *      usocket_cli.h
 *
 *      A simple user socket client
 *
 * =================================================================
 */

#if defined(__ECOS)
#include <pkgconf/system.h>
#include <pkgconf/isoinfra.h>
#include <cyg/hal/hal_tables.h>
#endif
#include <stdio.h>

#define CYG_USOCKETCLI_STATUS_CLIENT_CONNECT           0   // client connect.
#define CYG_USOCKETCLI_STATUS_CLIENT_REQUEST           1   // client has request comimg in
#define CYG_USOCKETCLI_STATUS_CLIENT_DISCONNECT        2   // client disconnect.
#define CYG_USOCKETCLI_UDP_STATUS_CLIENT_REQUEST       3   // udp client has request comimg in

#if defined(__ECOS)
#define DUMMY_NULL
#else
#ifndef __externC
#ifdef __cplusplus
# define __externC extern "C"
#else
# define __externC extern
#endif
#endif

#define bool int
#define false 0
#define true 1

#define cyg_bool int
#define cyg_addrword_t long*
#define CYG_ASSERT(cond,text) if(!(cond)){ \
                                  printf("%s\n", text);\
                                  exit(-1);\
                              }
#define cyg_flag_init(pflag) {*(pflag) = 0;}
#define cyg_flag_maskbits(pflag,bit) {*(pflag) &= (bit);}
#define cyg_flag_setbits(pflag,bit) {*(pflag) |= (bit);}
#define cyg_flag_wait(pflag,bit,mode) while(!(*(pflag) & (bit))){sleep(1);}
#define cyg_flag_destroy(pflag) {*(pflag) = 0;}
#define DUMMY_NULL NULL
#endif

typedef int usocket_cli_recv(char* addr, int size);
typedef void usocket_cli_notify(int status,int parm);

#define USOCKETCLI_IP_LEN          (16)

typedef struct
{
    usocket_cli_notify         *notify;        // notify the status
    usocket_cli_recv           *recv;
    char                       svrIP[USOCKETCLI_IP_LEN];   // server IP addr
    int                        portNum;        // socket port number
    int                        sockbufSize;    // socket buffer size
    int                        timeout;
    int                        connect_socket;
} usocket_cli_obj;


__externC void usocket_cli_install(usocket_cli_obj*  pusocket_cli_obj,usocket_cli_obj*  pObj); // install some callback function & config some settings

__externC void usocket_cli_uninstall(usocket_cli_obj*  pusocket_cli_obj);

__externC void usocket_cli_start(void);     // open usocket

__externC void usocket_cli_stop(void);    // close usocket

__externC int usocket_cli_send(usocket_cli_obj* pusocket_cli_obj,char* addr, int* size);  // install some callback function & config some settings

__externC usocket_cli_obj *usocket_cli_get_newobj(void);

__externC int usocket_cli_connect(usocket_cli_obj *pusocket_cli_obj);

__externC void usocket_cli_disconnect(usocket_cli_obj *pusocket_cli_obj);


/* ----------------------------------------------------------------- */
#endif /* CYGONCE_NET_USOCKETCLI_H                                  */
/* end of usocket_cli.h                                                   */

