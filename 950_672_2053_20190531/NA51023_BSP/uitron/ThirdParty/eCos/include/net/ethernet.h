//==========================================================================
//
//      include/net/ethernet.h
//
//==========================================================================
//####BSDCOPYRIGHTBEGIN####
//
// -------------------------------------------
//
// Portions of this software may have been derived from OpenBSD,
// FreeBSD or other sources, and are covered by the appropriate
// copyright disclaimers included herein.
//
// Portions created by Red Hat are
// Copyright (C) 2002 Red Hat, Inc. All Rights Reserved.
//
// -------------------------------------------
//
//####BSDCOPYRIGHTEND####
//==========================================================================

/*
 * Fundamental constants relating to ethernet.
 *
 * $FreeBSD: src/sys/net/ethernet.h,v 1.12.2.5 2000/07/19 21:43:52 archie Exp $
 *
 */

#ifndef _NET_ETHERNET_H_
#define _NET_ETHERNET_H_

/*
 * The number of bytes in an ethernet (MAC) address.
 */
#define	ETHER_ADDR_LEN		6

/*
 * The number of bytes in the type field.
 */
#define	ETHER_TYPE_LEN		2

/*
 * The number of bytes in the trailing CRC field.
 */
#define	ETHER_CRC_LEN		4

/*
 * The length of the combined header.
 */
#define	ETHER_HDR_LEN		(ETHER_ADDR_LEN*2+ETHER_TYPE_LEN)

/*
 * The minimum packet length.
 */
#define	ETHER_MIN_LEN		64

/*
 * The maximum packet length.
 */
#define	ETHER_MAX_LEN		1518

#if 1
 /*
  *      IEEE 802.3 Ethernet magic constants.  The frame sizes omit the preamble
  *      and FCS/CRC (frame check sequence).
  */
#define ETH_ALEN       6              // Octets in one ethernet addr (Ethernet address size)
#define ETH_HLEN       14             // Total octets in header (Ethernet header size)
#define ETH_ZLEN       60             // Min. octets in frame without FCS
#define ETH_DATA_LEN   1500           // Max. octets in payload
#define ETH_FRAME_LEN  1514           // Max. octets in frame without FCS
#define ETH_FCS_LEN    4              // Octets in the FCS
#endif

/*
 * A macro to validate a length with
 */
#define	ETHER_IS_VALID_LEN(foo)	\
	((foo) >= ETHER_MIN_LEN && (foo) <= ETHER_MAX_LEN)

/*
 * Structure of a 10Mb/s Ethernet header.
 */
struct	ether_header {
	u_char	ether_dhost[ETHER_ADDR_LEN];
	u_char	ether_shost[ETHER_ADDR_LEN];
	u_short	ether_type;
} __attribute__ ((aligned(1), packed));

/*
 * Structure of a 48-bit Ethernet address.
 */
struct	ether_addr {
	u_char octet[ETHER_ADDR_LEN];
} __attribute__ ((aligned(1), packed));
#define ether_addr_octet octet

#define	ETHERTYPE_PUP		0x0200	/* PUP protocol */
#define	ETHERTYPE_IP		0x0800	/* IP protocol */
#define	ETHERTYPE_ARP		0x0806	/* Addr. resolution protocol */
#define	ETHERTYPE_REVARP	0x8035	/* reverse Addr. resolution protocol */
#define	ETHERTYPE_VLAN		0x8100	/* IEEE 802.1Q VLAN tagging */
#define	ETHERTYPE_IPV6		0x86dd	/* IPv6 */
#define	ETHERTYPE_LOOPBACK	0x9000	/* used to test interfaces */
#define	ETH_P_ALL			0x0003	/* Every packet (be careful!!!) */
/* XXX - add more useful types here */

/*
 * The ETHERTYPE_NTRAILER packet types starting at ETHERTYPE_TRAIL have
 * (type-ETHERTYPE_TRAIL)*512 bytes of data followed
 * by an ETHER type (as given above) and then the (variable-length) header.
 */
#define	ETHERTYPE_TRAIL		0x1000		/* Trailer packet */
#define	ETHERTYPE_NTRAILER	16

#define	ETHERMTU	(ETHER_MAX_LEN-ETHER_HDR_LEN-ETHER_CRC_LEN)
#define	ETHERMIN	(ETHER_MIN_LEN-ETHER_HDR_LEN-ETHER_CRC_LEN)

#ifdef _KERNEL

/*
 * For device drivers to specify whether they support BPF or not
 */
#define ETHER_BPF_UNSUPPORTED	0
#define ETHER_BPF_SUPPORTED	1

struct ifnet;
struct mbuf;

extern	void (*ng_ether_input_p)(struct ifnet *ifp,
		struct mbuf **mp, struct ether_header *eh);
extern	void (*ng_ether_input_orphan_p)(struct ifnet *ifp,
		struct mbuf *m, struct ether_header *eh);
extern	int  (*ng_ether_output_p)(struct ifnet *ifp, struct mbuf **mp);
extern	void (*ng_ether_attach_p)(struct ifnet *ifp);
extern	void (*ng_ether_detach_p)(struct ifnet *ifp);

#else /* _KERNEL */

#include <sys/param.h>

/*
 * Ethernet address conversion/parsing routines.
 */
__BEGIN_DECLS
struct	ether_addr *ether_aton __P((const char *));
int	ether_hostton __P((const char *, struct ether_addr *));
int	ether_line __P((const char *, struct ether_addr *, char *));
char 	*ether_ntoa __P((const struct ether_addr *));
int	ether_ntohost __P((char *, const struct ether_addr *));
__END_DECLS

#endif /* !_KERNEL */

#endif /* !_NET_ETHERNET_H_ */
