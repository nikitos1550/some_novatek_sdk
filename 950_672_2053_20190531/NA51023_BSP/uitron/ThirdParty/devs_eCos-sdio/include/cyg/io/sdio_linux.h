#ifndef _SDIO_LINUX_H
#define _SDIO_LINUX_H

#include <cyg/io/sdio_dev.h>
#include <cyg/io/core.h>
#include <cyg/io/card.h>
#include <cyg/io/host.h>
#include <cyg/io/mmcsdio.h>

/**
    SDIO host io interrupt enable/disable

    @note For sdio_host_ioint_enable()
*/
typedef enum
{
    SDIO_HOST_IOINT_DIS = 0,      ///< sdio host io interrupt enable
    SDIO_HOST_IOINT_EN  = 1       ///< sdio host io interrupt disable
}SDIO_HOST_IOINT_ENDIS;


/**
    SDIO host io interrupt auto disable/not auto disable when reveive io interrupt

    @note For sdio_host_ioint_config()
*/
typedef enum
{
    SDIO_HOST_IOINT_NOTDIS  = 0,  ///< sdio host io interrupt not auto disable when receive io interrupt
    SDIO_HOST_IOINT_AUTODIS = 1   ///< sdio host io interrupt auto disable when receive io interrupt
}SDIO_HOST_IOINT_CONF;


/**
    SDIO host cmd error callback value

    @note For sdio_reg_errcallback()
*/
#define SDIO_HOST_ERR_CMD52  1   ///< sdio host cmd52 error
#define SDIO_HOST_ERR_CMD53  2   ///< sdio host cmd53 error


struct device {
#if 0
    struct device       *parent;

    struct device_private   *p;

    struct kobject kobj;
    const char      *init_name; /* initial name of the device */
    struct device_type  *type;

    struct semaphore    sem;    /* semaphore to synchronize calls to
                     * its driver.
                     */

    struct bus_type *bus;       /* type of bus device is on */
    struct device_driver *driver;   /* which driver has allocated this
                       device */
#endif
    void        *driver_data;   /* data private to the driver */
#if 0
    void        *platform_data; /* Platform specific data, device
                       core doesn't touch it */
    struct dev_pm_info  power;

#ifdef CONFIG_NUMA
    int     numa_node;  /* NUMA node this device is close to */
#endif
    u64     *dma_mask;  /* dma mask (if dma'able device) */
    u64     coherent_dma_mask;/* Like dma_mask, but for
                         alloc_coherent mappings as
                         not all hardware supports
                         64 bit addresses for consistent
                         allocations such descriptors. */

    struct device_dma_parameters *dma_parms;

    struct list_head    dma_pools;  /* dma pools (if dma'ble) */

    struct dma_coherent_mem *dma_mem; /* internal for coherent mem
                         override */
    /* arch specific additions */
    struct dev_archdata archdata;

    dev_t           devt;   /* dev_t, creates the sysfs "dev" */

    spinlock_t      devres_lock;
    struct list_head    devres_head;

    struct klist_node   knode_class;
    struct class        *class;
    struct attribute_group  **groups;   /* optional groups */

    void    (*release)(struct device *dev);
#endif
};


//
/*
 * SDIO function CIS tuple (unknown to the core)
 */
struct sdio_func_tuple {
    struct sdio_func_tuple *next;
    unsigned char code;
    unsigned char size;
    unsigned char data[0];
};

typedef void (sdio_irq_handler_t)(struct sdio_func *);

typedef void (*sdio_iocallback_hdl)(void);

typedef void (*sdio_errcallback_hdl)(unsigned int error);

/*
 * SDIO function devices
 */
struct sdio_func {
    struct mmc_card     *card;      /* the card this device belongs to */
    struct device       dev;        /* the device */
    sdio_irq_handler_t  *irq_handler;   /* IRQ callback */
    unsigned int        num;        /* function number */

    unsigned char       class;      /* standard interface class */
    unsigned short      vendor;     /* vendor id */
    unsigned short      device;     /* device id */

    unsigned        max_blksize;    /* maximum block size */
    unsigned        cur_blksize;    /* current block size */

    unsigned        enable_timeout; /* max enable timeout in msec */

    unsigned int        state;      /* function state */
#define SDIO_STATE_PRESENT  (1<<0)      /* present in sysfs */

    u8          tmpbuf[4];  /* DMA:able scratch buffer */

    unsigned        num_info;   /* number of info strings */
    const char      **info;     /* info strings */

    struct sdio_func_tuple *tuples;
};



extern unsigned char sdio_f0_readb(struct sdio_func *func,unsigned int addr, int *err_ret);
extern void sdio_f0_writeb(struct sdio_func *func, unsigned char b,unsigned int addr, int *err_ret);
extern int sdio_memcpy_toio(struct sdio_func *func, unsigned int addr,void *src, int count);

extern int sdio_writesb(struct sdio_func *func, unsigned int addr, void *src, int count);
extern void sdio_writeb(struct sdio_func *func, u8 b,unsigned int addr, int *err_ret);
extern void sdio_writew(struct sdio_func *func, u16 b,unsigned int addr, int *err_ret);
extern void sdio_writel(struct sdio_func *func, u32 b,unsigned int addr, int *err_ret);

extern int sdio_readsb(struct sdio_func *func, void *dst, unsigned int addr, int count);
extern u8 sdio_readb(struct sdio_func *func, unsigned int addr, int *err_ret);
extern u16 sdio_readw(struct sdio_func *func, unsigned int addr, int *err_ret);
extern u32 sdio_readl(struct sdio_func *func, unsigned int addr, int *err_ret);

extern int sdio_memcpy_fromio(struct sdio_func *func, void *dst,unsigned int addr, int count);

extern unsigned int sdio_align_size(struct sdio_func *func, unsigned int sz);


extern int sdio_claim_irq(struct sdio_func *func, sdio_irq_handler_t *handler);
extern int sdio_release_irq(struct sdio_func *func);

extern int sdio_set_block_size(struct sdio_func *func, unsigned blksz);
extern int sdio_enable_func(struct sdio_func *func);
extern int sdio_disable_func(struct sdio_func *func);

//extern void sdio_claim_host(struct sdio_func *func);
//extern void sdio_release_host(struct sdio_func *func);
extern int sdio_set_max_byte_size(struct sdio_func *func, unsigned int max_byte_size);


//extern int sdio_register_driver(struct sdio_driver *);
//extern void sdio_unregister_driver(struct sdio_driver *);

extern struct sdio_func *sdio_initialize(void);
extern void sdio_reg_iocallback(sdio_iocallback_hdl sdio_hdl);
extern void sdio_reg_errcallback(sdio_errcallback_hdl sdio_err_hdl);

extern int sdio_host_ioint_enable(SDIO_HOST_IOINT_ENDIS endis);
extern int sdio_host_ioint_config(SDIO_HOST_IOINT_CONF config);

extern int sdio_host_set_config(SDIODEV_CONFIG_ID configID, unsigned long configContext);

#endif
