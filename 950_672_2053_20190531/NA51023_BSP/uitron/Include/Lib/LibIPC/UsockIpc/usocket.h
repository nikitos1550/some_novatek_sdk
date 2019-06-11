#ifndef CYGONCE_NET_USOCKET_H
#define CYGONCE_NET_USOCKET_H
/* =================================================================
 *
 *      usocket.h
 *
 *      A simple user socket server
 *
 * =================================================================
 */

#if defined(__ECOS)
#include <pkgconf/system.h>
#include <pkgconf/isoinfra.h>
#include <cyg/hal/hal_tables.h>
#endif
#include <stdio.h>

#define CYG_USOCKET_STATUS_CLIENT_CONNECT           0   // client connect.
#define CYG_USOCKET_STATUS_CLIENT_REQUEST           1   // client has request comimg in
#define CYG_USOCKET_STATUS_CLIENT_DISCONNECT        2   // client disconnect.
#define CYG_USOCKET_UDP_STATUS_CLIENT_REQUEST       3   // udp client has request comimg in

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

typedef int usocket_recv(char* addr, int size);
typedef void usocket_notify(int status,int parm);
typedef int usocket_recv_multi(char* addr, int size, int obj_index);
typedef void usocket_notify_multi(int status,int parm, int obj_index);

typedef struct
{
    usocket_notify         *notify;        // notify the status
    usocket_recv           *recv;
    int                     portNum;        // socket port number
    int                     threadPriority; // socket thread priority
    int                     sockbufSize;// socket buffer size
    int                     client_socket;
} usocket_install_obj;


typedef struct
{
    usocket_notify_multi         *notify;        // notify the status
    usocket_recv_multi           *recv;
    int                     portNum;        // socket port number
    int                     threadPriority; // socket thread priority
    int                     sockbufSize;// socket buffer size
    int                     client_socket;
	int						occupy;     // 0:not using ,1:using
} usocket_install_obj_multi;

/////////////////////uscoket multi//////////////////////////////////

__externC int usocket_install_multi( usocket_install_obj_multi*  pObj); ///return obj_index, if return -1 means error.

__externC void usocket_uninstall_multi(int obj_index);

__externC void usocket_open_multi(int obj_index);

__externC void usocket_close_multi(int obj_index);

__externC int usocket_send_multi(char* buf, int* size, int obj_index);

__externC int usocket_get_obj_index(int port);

////////////////////////usocket/////////////////////////////////////

__externC void usocket_install( usocket_install_obj*  pObj);  // install some callback function & config some settings

__externC void usocket_uninstall(void);

__externC void usocket_open(void);     // open usocket

__externC void usocket_close(void);    // close usocket

__externC int usocket_send(char* addr, int* size);  // install some callback function & config some settings

///////////////////////////UDP////////////////////////////

__externC void usocket_udp_install( usocket_install_obj*  pObj);  // install some callback function & config some settings

__externC void usocket_udp_uninstall(void);

__externC void usocket_udp_open(void);     // open usocket

__externC void usocket_udp_close(void);    // close usocket

__externC int usocket_udp_send(char* addr, int* size);  // install some callback function & config some settings

__externC int usocket_udp_sendto(char* dest_ip, int dest_port,char* buf, int* size);

/* ----------------------------------------------------------------- */
#endif /* CYGONCE_NET_USOCKET_H                                  */
/* end of usocket.h                                                   */

