#ifndef CYGONCE_DEVS_DHD_ECOS_H
#define CYGONCE_DEVS_DHD_ECOS_H
//==========================================================================
//
//      dhd_ecos.h
//
//      Broadcom Wi-Fi BCM43xx chip
//
//==========================================================================

#include <cyg/hal/hal_io.h>

//------------------------------------------------------------------------
// Set to perms of:
// 0 disables all debug output
// 1 for process debug output
// 2 for added data IO output: get_reg, put_reg
// 4 for packet allocation/free output
// 8 for only startup status, so we can tell we're installed OK
#define DEBUG 0x0

#if DEBUG & 1
# define DEBUG_FUNCTION() do { diag_printf("%s\n", __FUNCTION__); } while (0)
#else
# define DEBUG_FUNCTION() do {} while(0)
#endif

// ------------------------------------------------------------------------
// Macros for keeping track of statistics
#if defined(ETH_DRV_GET_IF_STATS) || defined (ETH_DRV_GET_IF_STATS_UD)
#define KEEP_STATISTICS
#endif

#ifdef KEEP_STATISTICS
#define INCR_STAT( _x_ )        (cpd->stats. _x_ ++)
#else
#define INCR_STAT( _x_ )        CYG_EMPTY_STATEMENT
#endif

//------------------------------------------------------------------------
// Macros for accessing structure elements

#define _SU8( _base_, _offset_) \
        *((volatile cyg_uint8 *)((CYG_ADDRWORD)_base_+(_offset_)))
#define _SU16( _base_, _offset_) \
        *((volatile cyg_uint16 *)((CYG_ADDRWORD)_base_+(_offset_)))
#define _SU32( _base_, _offset_) \
        *((volatile cyg_uint32 *)((CYG_ADDRWORD)_base_+(_offset_)))

#define _SI8( _base_, _offset_) \
        *((volatile cyg_int8 *)((CYG_ADDRWORD)_base_+(_offset_)))
#define _SI16( _base_, _offset_) \
        *((volatile cyg_int16 *)((CYG_ADDRWORD)_base_+(_offset_)))
#define _SI32( _base_, _offset_) \
        *((volatile cyg_int32 *)((CYG_ADDRWORD)_base_+(_offset_)))

// ------------------------------------------------------------------------

#ifdef KEEP_STATISTICS
struct dhd_stats {
    unsigned int tx_good             ;
    unsigned int tx_max_collisions   ;
    unsigned int tx_late_collisions  ;
    unsigned int tx_underrun         ;
    unsigned int tx_carrier_loss     ;
    unsigned int tx_deferred         ;
    unsigned int tx_sqetesterrors    ;
    unsigned int tx_single_collisions;
    unsigned int tx_mult_collisions  ;
    unsigned int tx_total_collisions ;
    unsigned int rx_good             ;
    unsigned int rx_crc_errors       ;
    unsigned int rx_align_errors     ;
    unsigned int rx_resource_errors  ;
    unsigned int rx_overrun_errors   ;
    unsigned int rx_collisions       ;
    unsigned int rx_short_frames     ;
    unsigned int rx_too_long_frames  ;
    unsigned int rx_symbol_errors    ;
    unsigned int interrupts          ;
    unsigned int rx_count            ;
    unsigned int rx_deliver          ;
    unsigned int rx_resource         ;
    unsigned int rx_restart          ;
    unsigned int tx_count            ;
    unsigned int tx_complete         ;
    unsigned int tx_dropped          ;
};
#endif

typedef struct dhd_priv_data {
#ifdef CYGINT_IO_ETH_INT_SUPPORT_REQUIRED
    	cyg_handle_t  interrupt_handle;
        cyg_interrupt interrupt_object;
#endif

	int interrupt;      // Interrupt vector
	struct eth_drv_sc * sc;

    cyg_flag_t dhdinit_thread_flags;  // Communication with the dhddpc thread
    cyg_handle_t dhdinit_thread;

	unsigned char mac[6];               // mac address
	int txbusy;                         // A packet has been sent
	unsigned long txkey;                // Used to ack when packet sent

	void * rx_skb;

	unsigned int hdrlen;
	bool isup;
} dhd_priv_data_t;

int dhd_ecos_set_dbg_level(int level);
int dhd_ecos_sdio_probe(struct sdio_func *func);
void dhd_ecos_sdio_dpc_trigger(void);
void dhd_ecos_sdio_bus_stop(void);
int dhd_ecos_sdio_bus_start(struct device *dev);
struct sk_buff * dhd_ecos_sdio_rxdata(struct brcmf_pub *drvr, u8 ifidx);
#endif 
