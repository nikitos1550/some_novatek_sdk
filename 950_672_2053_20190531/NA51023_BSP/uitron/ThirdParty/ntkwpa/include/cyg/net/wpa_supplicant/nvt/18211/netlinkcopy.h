#ifndef __LINUX_NETLINK_H
#define __LINUX_NETLINK_H

//#include <linux/socket.h> /* for sa_family_t */
//#include <linux/types.h>
#include "nlmsg_includes.h"
//#include "common.h"

#if 1
enum {
	IFLA_UNSPEC,
	IFLA_ADDRESS,
	IFLA_BROADCAST,
	IFLA_IFNAME,
	IFLA_MTU,
	IFLA_LINK,
	IFLA_QDISC,
	IFLA_STATS,
	IFLA_COST,
#define IFLA_COST IFLA_COST
	IFLA_PRIORITY,
#define IFLA_PRIORITY IFLA_PRIORITY
	IFLA_MASTER,
#define IFLA_MASTER IFLA_MASTER
	IFLA_WIRELESS,		/* Wireless Extension event - see wireless.h */
#define IFLA_WIRELESS IFLA_WIRELESS
	IFLA_PROTINFO,		/* Protocol specific information for a link */
#define IFLA_PROTINFO IFLA_PROTINFO
	IFLA_TXQLEN,
#define IFLA_TXQLEN IFLA_TXQLEN
	IFLA_MAP,
#define IFLA_MAP IFLA_MAP
	IFLA_WEIGHT,
#define IFLA_WEIGHT IFLA_WEIGHT
	IFLA_OPERSTATE,
	IFLA_LINKMODE,
	IFLA_LINKINFO,
#define IFLA_LINKINFO IFLA_LINKINFO
	IFLA_NET_NS_PID,
	IFLA_IFALIAS,
	IFLA_NUM_VF,		/* Number of VFs if device is SR-IOV PF */
	IFLA_VFINFO_LIST,
	IFLA_STATS64,
	IFLA_VF_PORTS,
	IFLA_PORT_SELF,
	IFLA_AF_SPEC,
	IFLA_GROUP,		/* Group the device belongs to */
	IFLA_NET_NS_FD,
	IFLA_EXT_MASK,		/* Extended info mask, VFs, etc */
	IFLA_PROMISCUITY,	/* Promiscuity count: > 0 means acts PROMISC */
#define IFLA_PROMISCUITY IFLA_PROMISCUITY
	IFLA_NUM_TX_QUEUES,
	IFLA_NUM_RX_QUEUES,
	IFLA_CARRIER,
	IFLA_PHYS_PORT_ID,
	__IFLA_MAX
};
#endif

#ifndef IFF_LOWER_UP
#define IFF_LOWER_UP   0x10000         /* driver signals L1 up         */
#endif
#ifndef IFF_DORMANT
#define IFF_DORMANT    0x20000         /* driver signals dormant       */
#endif
/*
#ifndef IFLA_IFNAME
#define IFLA_IFNAME 3
#endif
#ifndef IFLA_WIRELESS
#define IFLA_WIRELESS 11
#endif

#ifndef IFLA_WIRELESS
#define IFLA_MASTER 10
#endif*/

#ifndef IFLA_OPERSTATE
#define IFLA_OPERSTATE 16
#endif
#ifndef IFLA_LINKMODE
#define IFLA_LINKMODE 17
#define IF_OPER_DORMANT 5
#define IF_OPER_UP 6
#endif


#define NETLINK_ROUTE		0	/* Routing/device hook				*/
#define NETLINK_UNUSED		1	/* Unused number				*/
#define NETLINK_USERSOCK	2	/* Reserved for user mode socket protocols 	*/
#define NETLINK_FIREWALL	3	/* Firewalling hook				*/
#define NETLINK_INET_DIAG	4	/* INET socket monitoring			*/
#define NETLINK_NFLOG		5	/* netfilter/iptables ULOG */
#define NETLINK_XFRM		6	/* ipsec */
#define NETLINK_SELINUX		7	/* SELinux event notifications */
#define NETLINK_ISCSI		8	/* Open-iSCSI */
#define NETLINK_AUDIT		9	/* auditing */
#define NETLINK_FIB_LOOKUP	10	
#define NETLINK_CONNECTOR	11
#define NETLINK_NETFILTER	12	/* netfilter subsystem */
#define NETLINK_IP6_FW		13
#define NETLINK_DNRTMSG		14	/* DECnet routing messages */
#define NETLINK_KOBJECT_UEVENT	15	/* Kernel messages to userspace */
#define NETLINK_GENERIC		16
/* leave room for NETLINK_DM (DM Events) */
#define NETLINK_SCSITRANSPORT	18	/* SCSI Transports */
#define NETLINK_ECRYPTFS	19

#define MAX_LINKS 32		
#if 0
struct sockaddr_nl
{
	sa_family_t	nl_family;	/* AF_NETLINK	*/
	unsigned short	nl_pad;		/* zero		*/
	__u32		nl_pid;		/* port ID	*/
       	__u32		nl_groups;	/* multicast groups mask */
};
#endif

struct nlmsghdr
{
	__u32		nlmsg_len;	/* Length of message including header */
	__u16		nlmsg_type;	/* Message content */
	__u16		nlmsg_flags;	/* Additional flags */
	__u32		nlmsg_seq;	/* Sequence number */
	__u32		nlmsg_pid;	/* Sending process port ID */
};

/* Flags values */

#define NLM_F_REQUEST		1	/* It is request message. 	*/
#define NLM_F_MULTI		2	/* Multipart message, terminated by NLMSG_DONE */
#define NLM_F_ACK		4	/* Reply with ack, with zero or error code */
#define NLM_F_ECHO		8	/* Echo this request 		*/
#define NLM_F_DUMP_INTR		16	/* Dump was inconsistent due to sequence change */

/* Modifiers to GET request */
#define NLM_F_ROOT	0x100	/* specify tree	root	*/
#define NLM_F_MATCH	0x200	/* return all matching	*/
#define NLM_F_ATOMIC	0x400	/* atomic GET		*/
#define NLM_F_DUMP	(NLM_F_ROOT|NLM_F_MATCH)

/* Modifiers to NEW request */
#define NLM_F_REPLACE	0x100	/* Override existing		*/
#define NLM_F_EXCL	0x200	/* Do not touch, if it exists	*/
#define NLM_F_CREATE	0x400	/* Create, if it does not exist	*/
#define NLM_F_APPEND	0x800	/* Add to end of list		*/

/*
   4.4BSD ADD		NLM_F_CREATE|NLM_F_EXCL
   4.4BSD CHANGE	NLM_F_REPLACE

   True CHANGE		NLM_F_CREATE|NLM_F_REPLACE
   Append		NLM_F_CREATE
   Check		NLM_F_EXCL
 */

#define NLMSG_ALIGNTO	4
#define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )
#define NLMSG_HDRLEN	 ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))
#define NLMSG_LENGTH(len) ((len)+NLMSG_ALIGN(NLMSG_HDRLEN))
#define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))
#define NLMSG_DATA(nlh)  ((void*)(((char*)nlh) + NLMSG_LENGTH(0)))
#define NLMSG_NEXT(nlh,len)	 ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
				  (struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))
#define NLMSG_OK(nlh,len) ((len) >= (int)sizeof(struct nlmsghdr) && \
			   (nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && \
			   (nlh)->nlmsg_len <= (len))
#define NLMSG_PAYLOAD(nlh,len) ((nlh)->nlmsg_len - NLMSG_SPACE((len)))

#define NLMSG_NOOP		0x1	/* Nothing.		*/
#define NLMSG_ERROR		0x2	/* Error		*/
#define NLMSG_DONE		0x3	/* End of a dump	*/
#define NLMSG_OVERRUN		0x4	/* Data lost		*/

#define NLMSG_MIN_TYPE		0x10	/* < 0x10: reserved control messages */


#define RTMGRP_LINK 1
#define RTM_BASE 0x10
#define RTM_NEWLINK (RTM_BASE + 0)
#define RTM_DELLINK (RTM_BASE + 1)
#define RTM_SETLINK (RTM_BASE + 3)

#define RTA_ALIGNTO 4
#define RTA_ALIGN(len) (((len) + RTA_ALIGNTO - 1) & ~(RTA_ALIGNTO - 1))
#define RTA_OK(rta,len) \
((len) > 0 && (rta)->rta_len >= sizeof(struct rtattr) && \
(rta)->rta_len <= (len))
#define RTA_NEXT(rta,attrlen) \
((attrlen) -= RTA_ALIGN((rta)->rta_len), \
(struct rtattr *) (((char *)(rta)) + RTA_ALIGN((rta)->rta_len)))
#define RTA_LENGTH(len) (RTA_ALIGN(sizeof(struct rtattr)) + (len))
#define RTA_DATA(rta) ((void *) (((char *) (rta)) + RTA_LENGTH(0)))
#define RTA_PAYLOAD(rta) ((int) ((rta)->rta_len) - RTA_LENGTH(0))

struct ifinfomsg
{
	unsigned char ifi_family;
	unsigned char __ifi_pad;
	unsigned short ifi_type;
	int ifi_index;
	unsigned ifi_flags;
	unsigned ifi_change;
};

struct rtattr
{
	unsigned short rta_len;
	unsigned short rta_type;
};


struct nlmsgerr
{
	int		error;
	struct nlmsghdr msg;
};

#define NETLINK_ADD_MEMBERSHIP	1
#define NETLINK_DROP_MEMBERSHIP	2
#define NETLINK_PKTINFO		3

struct nl_pktinfo
{
	__u32	group;
};

#define NET_MAJOR 36		/* Major 36 is reserved for networking 						*/

enum {
	NETLINK_UNCONNECTED = 0,
	NETLINK_CONNECTED,
};

/*
 *  <------- NLA_HDRLEN ------> <-- NLA_ALIGN(payload)-->
 * +---------------------+- - -+- - - - - - - - - -+- - -+
 * |        Header       | Pad |     Payload       | Pad |
 * |   (struct nlattr)   | ing |                   | ing |
 * +---------------------+- - -+- - - - - - - - - -+- - -+
 *  <-------------- nlattr->nla_len -------------->
 */

struct nlattr
{
	__u16           nla_len;
	__u16           nla_type;
};

/*
 * nla_type (16 bits)
 * +---+---+-------------------------------+
 * | N | O | Attribute Type                |
 * +---+---+-------------------------------+
 * N := Carries nested attributes
 * O := Payload stored in network byte order
 *
 * Note: The N and O flag are mutually exclusive.
 */
#define NLA_F_NESTED		(1 << 15)
#define NLA_F_NET_BYTEORDER	(1 << 14)
#define NLA_TYPE_MASK		~(NLA_F_NESTED | NLA_F_NET_BYTEORDER)

#define NLA_ALIGNTO		4
#define NLA_ALIGN(len)		(((len) + NLA_ALIGNTO - 1) & ~(NLA_ALIGNTO - 1))
#define NLA_HDRLEN		((int) NLA_ALIGN(sizeof(struct nlattr)))

#endif	/* __LINUX_NETLINK_H */
