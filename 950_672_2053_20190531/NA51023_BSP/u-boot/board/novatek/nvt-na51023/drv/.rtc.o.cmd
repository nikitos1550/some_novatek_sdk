cmd_board/novatek/nvt-na51023/drv/rtc.o := /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/bin/mipsel-24kec-linux-gnu-gcc -Wp,-MD,board/novatek/nvt-na51023/drv/.rtc.o.d  -nostdinc -isystem /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/bin/../lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include -Iinclude  -I/home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include -D__KERNEL__ -DCONFIG_SYS_TEXT_BASE=0x03900000 -Wall -Wstrict-prototypes -Wno-format-security -fno-builtin -ffreestanding -Os -fno-stack-protector -g -fstack-usage -Wno-format-nonliteral -Werror=date-time -D_BUILD_DATE_=20`date +'%y%m%d'` -DCONFIG_MIPS -D__MIPS__ -G 0 -mabicalls -fpic -EL -msoft-float -DCONFIG_MIPS32 -march=mips32r2 -mabi=32 -DCONFIG_32BIT -mtune=mips32r2 -D_MODEL__ -D_BIN_NAME_='"FW96672A"' -D_BIN_NAME_T_='"FW96672T"' -D_EMBMEM_BLK_SIZE_=0x10000 -D_BOARD_DRAM_ADDR_=0x00000000 -D_BOARD_DRAM_SIZE_=0x04000000 -D_BOARD_REV_ADDR_=0x00000000 -D_BOARD_REV_SIZE_=0x00002000 -D_BOARD_IPC_ADDR_=0x00002000 -D_BOARD_IPC_SIZE_=0x000FF000 -D_BOARD_LINUX_ADDR_=0x00000000 -D_BOARD_LINUX_SIZE_=0x00000000 -D_BOARD_UBOOT_ADDR_=0x03900000 -D_BOARD_UBOOT_SIZE_=0x00700000 -D_BOARD_UITRON_ADDR_=0x00101000 -D_BOARD_UITRON_SIZE_=0x03EFF000 -D_BOARD_UITRON_RESV_SIZE_=0x00000000 -D_BOARD_ECOS_ADDR_=0x00200000 -D_BOARD_ECOS_SIZE_=0x00000000 -D_BOARD_RAMDISK_ADDR_=0x00000000 -D_BOARD_RAMDISK_SIZE_=0x00000000 -D_BOARD_DSP1_ADDR_=0x00101000 -D_BOARD_DSP1_SIZE_=0x00000000 -D_BOARD_DSP2_ADDR_= -D_BOARD_DSP2_SIZE_= -D_BOARD_LOADER_ADDR_=0xC07C0000 -D_BOARD_LOADER_SIZE_=0x0000A000 -D_BOARD_EXTDRAM_ADDR_= -D_BOARD_EXTDRAM_SIZE_= -D_CHIP_96672_ -D_CHIP_NAME_='"NT96672"' -D_FW_NAME_='"FW96672A.bin"' -D_RUN_NAME_='"FW96672T.bin"' -D_EXT_NAME_='"FW96672A.ext.bin"' -D_MODELEXT_BUILT_IN_OFF_ -D_HW_MP_STAGE_ -D_FW_TYPE_LZMA_ -D_KEY_METHOD_4KEY_ -D_CPU1_ECOS_ -D_CPU2_NONE_ -D_DSP1_NONE_ -D_DSP2_NONE_ -D_NETWORK_CPU1_ -D_CLOCK_RTC_ -D_Disp_VIRTUAL_LCD1_OFF_ -D_LCD_NAME_='"Disp_VIRTUAL_LCD1_OFF"' -D_Disp_VIRTUAL_LCD2_OFF_ -D_LCD2_NAME_='"Disp_VIRTUAL_LCD2_OFF"' -D_TV_ON_ -D_TV_NAME_='"TV_ON"' -D_HDMI_OFF_ -D_CMOS_IMX307M_ -D_SENSOR1_CMOS_IMX307M_ -D_SENSOR1_='"CMOS_IMX307M"' -D_CMOS_OFF_ -D_SENSOR2_CMOS_OFF_ -D_SENSOR2_='"CMOS_OFF"' -D_CMOS_OFF_ -D_SENSOR_EXT1_='"CMOS_OFF"' -D_CMOS_OFF_ -D_SENSOR_EXT2_='"CMOS_OFF"' -D_IPL_CAL_SAMPLE_FF_ -D_IPL_CAL1_IPL_CAL_SAMPLE_FF_ -D_IPL_CAL1_='"IPL_CAL_SAMPLE_FF"' -D_IPL_CAL_FAKE_ -D_IPL_CAL2_IPL_CAL_FAKE_ -D_IPL_CAL2_='"IPL_CAL_FAKE"' -D_LENSDRV_AN41908_ -D__ -D_EMBMEM_SPI_NOR_ -D_EMBMEM_UITRON_ON_ -D_EMBMEM_NAME_='"EMBMEM_SPI_NOR"' -D_CARD1_SD_ -D_CARD3_SD_ -D_Gyro_None_ -D_IrRx_NONE_ -D_GSensor_Combo_ -D_UI_STYLE_CARDV_ -D_AUDIO_ON_ -D_AudCodecExt_None_ -D__ -D_GPS_NONE_ -D_TOUCH_OFF_ -D_ENABLE_ -D_DEVICE_ -D_USB3_INT_CORE1_ -D_p2pcam_main_ -D_MCU_NONE_ -D_NVT_LINUX_SMP_OFF_ -D_NVT_DEFAULT_NETWORK_BOOT_PROTOCOL_DHCP_SERVER_ -D_NVT_DEFAULT_CVBS_TYPE_NTSC_ -D_NVT_ROOTFS_TYPE_NOR_SQUASH_ -D_NVT_CURL_SSL_OPENSSL_ -D_GPROF_TOOL_OFF_ -D_NVT_UBOOT_ENV_IN_STORG_SUPPORT_OFF_ -D_NVT_ETHERNET_NONE_ -D_NVT_SDIO_WIFI_RTK_ -D_NVT_USB_WIFI_NONE_ -D_NVT_USB_4G_NONE_ -D_NVT_ETHERNET_NONE_ -D_WIFI_RTK_MDL_8189ftv_ -D_WIFI_BRCM_MDL_43455c0_ampk6255c0_ -D_WIFI_NVT_MDL_18211_ -D_IQ_PARAM_IMX307_EVB_ -D_IQ_PARAM1_IQ_PARAM_IMX307_EVB_ -D_IQ_PARAM1_='"IQ_PARAM_IMX307_EVB"' -D_IQ_PARAM_FAKE_ -D_IQ_PARAM2_IQ_PARAM_FAKE_ -D_IQ_PARAM2_='"IQ_PARAM_FAKE"' -D_AWB_PARAM_IMX307_EVB_ -D_AWB_PARAM1_AWB_PARAM_IMX307_EVB_ -D_AWB_PARAM1_='"AWB_PARAM_IMX307_EVB"' -D_AWB_PARAM_FAKE_ -D_AWB_PARAM2_AWB_PARAM_FAKE_ -D_AWB_PARAM2_='"AWB_PARAM_FAKE"' -D_AE_PARAM_IMX307_EVB_ -D_AE_PARAM1_AE_PARAM_IMX307_EVB_ -D_AE_PARAM1_='"AE_PARAM_IMX307_EVB"' -D_AE_PARAM_FAKE_ -D_AE_PARAM2_AE_PARAM_FAKE_ -D_AE_PARAM2_='"AE_PARAM_FAKE"' -D_SSL_OFF_ -D_NVT_SDIO2_PAD_POWER_3300_ -D_NVT_DS_MODE_DATA_PAD_DRIVING_15MA_ -D_NVT_DS_MODE_CMD_PAD_DRIVING_15MA_ -D_NVT_DS_MODE_CLK_PAD_DRIVING_20MA_ -D_NVT_HS_MODE_DATA_PAD_DRIVING_15MA_ -D_NVT_HS_MODE_CMD_PAD_DRIVING_15MA_ -D_NVT_HS_MODE_CLK_PAD_DRIVING_20MA_ -D_NVT_SDR50_MODE_DATA_PAD_DRIVING_25MA_ -D_NVT_SDR50_MODE_CMD_PAD_DRIVING_25MA_ -D_NVT_SDR50_MODE_CLK_PAD_DRIVING_30MA_ -D_NVT_SDR104_MODE_DATA_PAD_DRIVING_25MA_ -D_NVT_SDR104_MODE_CMD_PAD_DRIVING_25MA_ -D_NVT_SDR104_MODE_CLK_PAD_DRIVING_30MA_ -D_FORCE_FLASH_DUAL_OFF_ -D_MODEL_CFG_STR_='"-D_MODEL__ -D_BIN_NAME_='"FW96672A"' -D_BIN_NAME_T_='"FW96672T"' -D_EMBMEM_BLK_SIZE_=0x10000 -D_BOARD_DRAM_ADDR_=0x00000000 -D_BOARD_DRAM_SIZE_=0x04000000 -D_BOARD_REV_ADDR_=0x00000000 -D_BOARD_REV_SIZE_=0x00002000 -D_BOARD_IPC_ADDR_=0x00002000 -D_BOARD_IPC_SIZE_=0x000FF000 -D_BOARD_LINUX_ADDR_=0x00000000 -D_BOARD_LINUX_SIZE_=0x00000000 -D_BOARD_UBOOT_ADDR_=0x03900000 -D_BOARD_UBOOT_SIZE_=0x00700000 -D_BOARD_UITRON_ADDR_=0x00101000 -D_BOARD_UITRON_SIZE_=0x03EFF000 -D_BOARD_UITRON_RESV_SIZE_=0x00000000 -D_BOARD_ECOS_ADDR_=0x00200000 -D_BOARD_ECOS_SIZE_=0x00000000 -D_BOARD_RAMDISK_ADDR_=0x00000000 -D_BOARD_RAMDISK_SIZE_=0x00000000 -D_BOARD_DSP1_ADDR_=0x00101000 -D_BOARD_DSP1_SIZE_=0x00000000 -D_BOARD_DSP2_ADDR_= -D_BOARD_DSP2_SIZE_= -D_BOARD_LOADER_ADDR_=0xC07C0000 -D_BOARD_LOADER_SIZE_=0x0000A000 -D_BOARD_EXTDRAM_ADDR_= -D_BOARD_EXTDRAM_SIZE_= -D_CHIP_96672_ -D_CHIP_NAME_='"NT96672"' -D_FW_NAME_='"FW96672A.bin"' -D_RUN_NAME_='"FW96672T.bin"' -D_EXT_NAME_='"FW96672A.ext.bin"' -D_MODELEXT_BUILT_IN_OFF_ -D_MODELEXT_X1_ -D_HW_MP_STAGE_ -D_FW_TYPE_LZMA_ -D_KEY_METHOD_4KEY_ -D_CPU1_ECOS_ -D_CPU2_NONE_ -D_DSP1_NONE_ -D_DSP2_NONE_ -D_NETWORK_CPU1_ -D_GPROF_TOOL_OFF_ -D_DEVICE_ -D__ -D_NVT_ETHERNET_NONE_ -D_NVT_SDIO_WIFI_RTK_ -D_NVT_USB_WIFI_NONE_ -D_NVT_USB_4G_NONE_ -D_WIFI_RTK_MDL_8189ftv_ -D_WIFI_BRCM_MDL_43455c0_ampk6255c0_ -D_WIFI_NVT_MDL_18211_ -D_CMOS_IMX307M_ -D_CMOS_OFF_;"' -D_IPL_SAMPLE_FF_ -D_IPL1_IPL_SAMPLE_FF_ -D_IPL1_='"IPL_SAMPLE_FF"' -D_IPL1_EXT_LIB_TAB_=IPL_SAMPLE_FF_EXT_LIB_TAB -D_IPL_FAKE_ -D_IPL2_IPL_FAKE_ -D_IPL2_='"IPL_FAKE"' -D_IPL2_EXT_LIB_TAB_=IPL_FAKE_EXT_LIB_TAB -D_IPL_EXT1_IPL_FAKE_ -D_IPL_EXT1_='"IPL_FAKE"' -D_IPL_EXT1_EXT_LIB_TAB_=IPL_FAKE_EXT_LIB_TAB -D_IPL_EXT2_IPL_FAKE_ -D_IPL_EXT2_='"IPL_FAKE"' -D_IPL_EXT2_EXT_LIB_TAB_=IPL_FAKE_EXT_LIB_TAB -D_IPL_CAL_SAMPLE_FF_ -D_IPL_CAL1_IPL_CAL_SAMPLE_FF_ -D_IPL_CAL1_='"IPL_CAL_SAMPLE_FF"' -D_IPL_CAL1_EXT_LIB_TAB_=IPL_CAL_SAMPLE_FF_EXT_LIB_TAB -D_IPL_CAL_FAKE_ -D_IPL_CAL2_IPL_CAL_FAKE_ -D_IPL_CAL2_='"IPL_CAL_FAKE"' -D_IPL_CAL2_EXT_LIB_TAB_=IPL_CAL_FAKE_EXT_LIB_TAB -I/home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/include -DCONFIG_SYS_TEXT_BASE=0x03900000 -ffunction-sections -fdata-sections -pipe    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(rtc)"  -D"KBUILD_MODNAME=KBUILD_STR(rtc)" -c -o board/novatek/nvt-na51023/drv/rtc.o board/novatek/nvt-na51023/drv/rtc.c

source_board/novatek/nvt-na51023/drv/rtc.o := board/novatek/nvt-na51023/drv/rtc.c

deps_board/novatek/nvt-na51023/drv/rtc.o := \
    $(wildcard include/config/nvt/drtc.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/arch/rcw_macro.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/posix_types.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
    $(wildcard include/config/64bit.h) \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include/stdbool.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/arch/nvt_common.h \
    $(wildcard include/config/id.h) \
    $(wildcard include/config/nvt/linux/spinand/boot.h) \
    $(wildcard include/config/nvt/linux/spinor/boot.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/io.h \
    $(wildcard include/config/swap/io/space.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/soc/au1x00.h) \
    $(wildcard include/config/tb0229.h) \
    $(wildcard include/config/cpu/r8000.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/byteorder.h \
    $(wildcard include/config/cpu/mipsr2.h) \
    $(wildcard include/config/cpu/mips64/r2.h) \
  include/linux/byteorder/little_endian.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  include/linux/byteorder/swab.h \
  include/linux/byteorder/generic.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/include/modelext/emb_partition_info.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/include/modelext/interrupt.h \
    $(wildcard include/config/id/int/destination.h) \
    $(wildcard include/config/id/int/to/cpu1.h) \
    $(wildcard include/config/id/int/to/cpu2.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/include/nvt_type.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/include/modelext/cc.h \
    $(wildcard include/config/id/mips2/base/address.h) \
    $(wildcard include/config/id/ctrl/srst.h) \
    $(wildcard include/config/id/set/mips1/req.h) \
    $(wildcard include/config/id/set/mips2/req.h) \
    $(wildcard include/config/id/set/dsp/epp/req.h) \
    $(wildcard include/config/id/set/dsp/edp/req.h) \
    $(wildcard include/config/id/set/dsp/pint/base/address.h) \
    $(wildcard include/config/id/set/dsp/isdm/base/address.h) \
    $(wildcard include/config/id/set/dsp/ivdm/base/address.h) \
    $(wildcard include/config/id/cpu2/sleep/int.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/include/modelext/gpio_info.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/arch/IOAddress.h \
  include/common.h \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/4xx.h) \
    $(wildcard include/config/8xx.h) \
    $(wildcard include/config/mpc859.h) \
    $(wildcard include/config/mpc859t.h) \
    $(wildcard include/config/mpc866.h) \
    $(wildcard include/config/mpc866p.h) \
    $(wildcard include/config/mpc866/family.h) \
    $(wildcard include/config/mpc870.h) \
    $(wildcard include/config/mpc875.h) \
    $(wildcard include/config/mpc880.h) \
    $(wildcard include/config/mpc885.h) \
    $(wildcard include/config/mpc885/family.h) \
    $(wildcard include/config/mpc860.h) \
    $(wildcard include/config/mpc860t.h) \
    $(wildcard include/config/mpc86x.h) \
    $(wildcard include/config/5xx.h) \
    $(wildcard include/config/mpc5xxx.h) \
    $(wildcard include/config/mpc512x.h) \
    $(wildcard include/config/mpc8260.h) \
    $(wildcard include/config/mpc8247.h) \
    $(wildcard include/config/mpc8272.h) \
    $(wildcard include/config/mpc8272/family.h) \
    $(wildcard include/config/mpc86xx.h) \
    $(wildcard include/config/mpc85xx.h) \
    $(wildcard include/config/mpc83xx.h) \
    $(wildcard include/config/hymod.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/blackfin.h) \
    $(wildcard include/config/soc/da8xx.h) \
    $(wildcard include/config/sys/support/64bit/data.h) \
    $(wildcard include/config/panic/hang.h) \
    $(wildcard include/config/tqm8xxm.h) \
    $(wildcard include/config/tqm8xxl.h) \
    $(wildcard include/config/tqm823m.h) \
    $(wildcard include/config/tqm850m.h) \
    $(wildcard include/config/tqm855m.h) \
    $(wildcard include/config/tqm860m.h) \
    $(wildcard include/config/tqm862m.h) \
    $(wildcard include/config/tqm866m.h) \
    $(wildcard include/config/tqm823l.h) \
    $(wildcard include/config/tqm850l.h) \
    $(wildcard include/config/tqm855l.h) \
    $(wildcard include/config/tqm860l.h) \
    $(wildcard include/config/tqm862l.h) \
    $(wildcard include/config/tqm885d.h) \
    $(wildcard include/config/env/is/embedded.h) \
    $(wildcard include/config/sys/malloc/len.h) \
    $(wildcard include/config/env/addr.h) \
    $(wildcard include/config/env/size.h) \
    $(wildcard include/config/sys/monitor/base.h) \
    $(wildcard include/config/sys/monitor/len.h) \
    $(wildcard include/config/env/is/in/nvram.h) \
    $(wildcard include/config/display/cpuinfo.h) \
    $(wildcard include/config/sandbox.h) \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/nds32.h) \
    $(wildcard include/config/mips.h) \
    $(wildcard include/config/arc.h) \
    $(wildcard include/config/auto/complete.h) \
    $(wildcard include/config/440.h) \
    $(wildcard include/config/405ex.h) \
    $(wildcard include/config/sys/pci/target/init.h) \
    $(wildcard include/config/sys/pci/master/init.h) \
    $(wildcard include/config/440spe.h) \
    $(wildcard include/config/460ex.h) \
    $(wildcard include/config/460gt.h) \
    $(wildcard include/config/spi.h) \
    $(wildcard include/config/lwmon.h) \
    $(wildcard include/config/sys/i2c/eeprom/addr.h) \
    $(wildcard include/config/sys/def/eeprom/addr.h) \
    $(wildcard include/config/env/eeprom/is/on/i2c.h) \
    $(wildcard include/config/hermes.h) \
    $(wildcard include/config/evb64260.h) \
    $(wildcard include/config/sys/dram/test.h) \
    $(wildcard include/config/74xx/7xx.h) \
    $(wildcard include/config/74x.h) \
    $(wildcard include/config/75x.h) \
    $(wildcard include/config/74xx.h) \
    $(wildcard include/config/microblaze.h) \
    $(wildcard include/config/ddr/ecc.h) \
    $(wildcard include/config/ecc/init/via/ddrcontroller.h) \
    $(wildcard include/config/of/libfdt.h) \
    $(wildcard include/config/of/board/setup.h) \
    $(wildcard include/config/s3c24x0.h) \
    $(wildcard include/config/lh7a40x.h) \
    $(wildcard include/config/ep93xx.h) \
    $(wildcard include/config/imx.h) \
    $(wildcard include/config/mcf52x2.h) \
    $(wildcard include/config/console/mux.h) \
    $(wildcard include/config/status/led.h) \
    $(wildcard include/config/show/activity.h) \
    $(wildcard include/config/mp.h) \
    $(wildcard include/config/arch/map/sysmem.h) \
    $(wildcard include/config/ppc.h) \
    $(wildcard include/config/post.h) \
    $(wildcard include/config/has/post.h) \
    $(wildcard include/config/post/alt/list.h) \
    $(wildcard include/config/post/std/list.h) \
    $(wildcard include/config/init/critical.h) \
    $(wildcard include/config/skip/lowlevel/init.h) \
  include/config.h \
    $(wildcard include/config/sys/arch.h) \
    $(wildcard include/config/sys/cpu.h) \
    $(wildcard include/config/sys/board.h) \
    $(wildcard include/config/sys/vendor.h) \
    $(wildcard include/config/sys/soc.h) \
    $(wildcard include/config/boarddir.h) \
  include/config_cmd_defaults.h \
    $(wildcard include/config/cmd/defaults/h/.h) \
    $(wildcard include/config/cmd/bootm.h) \
    $(wildcard include/config/cmd/crc32.h) \
    $(wildcard include/config/cmd/exportenv.h) \
    $(wildcard include/config/cmd/go.h) \
    $(wildcard include/config/cmd/importenv.h) \
  include/config_defaults.h \
    $(wildcard include/config/defaults/h/.h) \
    $(wildcard include/config/bootm/linux.h) \
    $(wildcard include/config/bootm/netbsd.h) \
    $(wildcard include/config/bootm/plan9.h) \
    $(wildcard include/config/bootm/rtems.h) \
    $(wildcard include/config/bootm/vxworks.h) \
    $(wildcard include/config/gzip.h) \
    $(wildcard include/config/zlib.h) \
    $(wildcard include/config/partitions.h) \
  include/configs/nvt-na51023.h \
    $(wildcard include/config/na51023/h.h) \
    $(wildcard include/config/nvt/fw/update/led.h) \
    $(wildcard include/config/nvt/pwm.h) \
    $(wildcard include/config/fpga/emulation.h) \
    $(wildcard include/config/mips32.h) \
    $(wildcard include/config/na51023.h) \
    $(wildcard include/config/nvt/board.h) \
    $(wildcard include/config/nand/type/onfi.h) \
    $(wildcard include/config/nand/type/spinand.h) \
    $(wildcard include/config/nand/type/spinor.h) \
    $(wildcard include/config/flash/only/dual.h) \
    $(wildcard include/config/sys/clkin.h) \
    $(wildcard include/config/cpu/mhz.h) \
    $(wildcard include/config/sys/mips/timer/freq.h) \
    $(wildcard include/config/sys/hz.h) \
    $(wildcard include/config/pwm.h) \
    $(wildcard include/config/pwm/period00.h) \
    $(wildcard include/config/pwm/period01.h) \
    $(wildcard include/config/pwm/period02.h) \
    $(wildcard include/config/pwm/period03.h) \
    $(wildcard include/config/pwm/period04.h) \
    $(wildcard include/config/pwm/period05.h) \
    $(wildcard include/config/pwm/period06.h) \
    $(wildcard include/config/pwm/period07.h) \
    $(wildcard include/config/pwm/period08.h) \
    $(wildcard include/config/pwm/period09.h) \
    $(wildcard include/config/pwm/period10.h) \
    $(wildcard include/config/pwm/period11.h) \
    $(wildcard include/config/pwm/period12.h) \
    $(wildcard include/config/pwm/period13.h) \
    $(wildcard include/config/pwm/period14.h) \
    $(wildcard include/config/pwm/period15.h) \
    $(wildcard include/config/pwm/duty00.h) \
    $(wildcard include/config/pwm/duty01.h) \
    $(wildcard include/config/pwm/duty02.h) \
    $(wildcard include/config/pwm/duty03.h) \
    $(wildcard include/config/pwm/duty04.h) \
    $(wildcard include/config/pwm/duty05.h) \
    $(wildcard include/config/pwm/duty06.h) \
    $(wildcard include/config/pwm/duty07.h) \
    $(wildcard include/config/pwm/duty08.h) \
    $(wildcard include/config/pwm/duty09.h) \
    $(wildcard include/config/pwm/duty10.h) \
    $(wildcard include/config/pwm/duty11.h) \
    $(wildcard include/config/pwm/duty12.h) \
    $(wildcard include/config/pwm/duty13.h) \
    $(wildcard include/config/pwm/duty14.h) \
    $(wildcard include/config/pwm/duty15.h) \
    $(wildcard include/config/sys/dcache/size.h) \
    $(wildcard include/config/sys/icache/size.h) \
    $(wildcard include/config/sys/cacheline/size.h) \
    $(wildcard include/config/sys/uart.h) \
    $(wildcard include/config/sys/baudrate/table.h) \
    $(wildcard include/config/baudrate.h) \
    $(wildcard include/config/na51023/gpio.h) \
    $(wildcard include/config/sys/sdram/base.h) \
    $(wildcard include/config/uboot/sdram/base.h) \
    $(wildcard include/config/sys/sdram/size.h) \
    $(wildcard include/config/uboot/sdram/size.h) \
    $(wildcard include/config/linux/sdram/base.h) \
    $(wildcard include/config/linux/sdram/size.h) \
    $(wildcard include/config/sys/init/sp/addr.h) \
    $(wildcard include/config/sys/text/base.h) \
    $(wildcard include/config/fixed/eth/parameter.h) \
    $(wildcard include/config/overwrite/ethaddr/once.h) \
    $(wildcard include/config/ethnet.h) \
    $(wildcard include/config/ethaddr.h) \
    $(wildcard include/config/ipaddr.h) \
    $(wildcard include/config/netmask.h) \
    $(wildcard include/config/serverip.h) \
    $(wildcard include/config/gatewayip.h) \
    $(wildcard include/config/hostname.h) \
    $(wildcard include/config/bootp/subnetmask.h) \
    $(wildcard include/config/bootp/gateway.h) \
    $(wildcard include/config/bootp/hostname.h) \
    $(wildcard include/config/bootp/bootpath.h) \
    $(wildcard include/config/bootp/bootfilesize.h) \
    $(wildcard include/config/phylib.h) \
    $(wildcard include/config/cmd/ping.h) \
    $(wildcard include/config/cmd/dhcp.h) \
    $(wildcard include/config/cmd/net.h) \
    $(wildcard include/config/net/multi.h) \
    $(wildcard include/config/netconsole.h) \
    $(wildcard include/config/sys/longhelp.h) \
    $(wildcard include/config/cmdline/editing.h) \
    $(wildcard include/config/sys/hush/parser.h) \
    $(wildcard include/config/sys/prompt/hush/ps2.h) \
    $(wildcard include/config/lzo.h) \
    $(wildcard include/config/bzip2.h) \
    $(wildcard include/config/lzma.h) \
    $(wildcard include/config/cmd/memory.h) \
    $(wildcard include/config/sys/prompt.h) \
    $(wildcard include/config/sys/cbsize.h) \
    $(wildcard include/config/sys/pbsize.h) \
    $(wildcard include/config/sys/maxargs.h) \
    $(wildcard include/config/sys/bootparams/len.h) \
    $(wildcard include/config/sys/load/addr.h) \
    $(wildcard include/config/sys/memtest/start.h) \
    $(wildcard include/config/sys/memtest/end.h) \
    $(wildcard include/config/memsize/in/bytes.h) \
    $(wildcard include/config/misc/init/r.h) \
    $(wildcard include/config/bootargs/common.h) \
    $(wildcard include/config/nvt/linux/autoload.h) \
    $(wildcard include/config/nvt/linux/autodetect.h) \
    $(wildcard include/config/nvt/bin/chksum/support.h) \
    $(wildcard include/config/nvt/linux/ramdisk/boot.h) \
    $(wildcard include/config/bootargs.h) \
    $(wildcard include/config/cmd/ubi.h) \
    $(wildcard include/config/cmd/ubifs.h) \
    $(wildcard include/config/cmd/nand/trimffs.h) \
    $(wildcard include/config/nvt/linux/sd/boot.h) \
    $(wildcard include/config/dsp1/freertos.h) \
    $(wildcard include/config/nvt/mmc.h) \
    $(wildcard include/config/nvt/load/linux/image.h) \
    $(wildcard include/config/sys/flash/base.h) \
    $(wildcard include/config/ramoops/log.h) \
    $(wildcard include/config/stacksize.h) \
    $(wildcard include/config/sys/init/sp/offset.h) \
    $(wildcard include/config/bootdelay.h) \
    $(wildcard include/config/autoboot/keyed.h) \
    $(wildcard include/config/autoboot/stop/str.h) \
    $(wildcard include/config/autoboot/prompt.h) \
    $(wildcard include/config/zero/bootdelay/check.h) \
    $(wildcard include/config/nvt/wdt.h) \
    $(wildcard include/config/watchdog.h) \
    $(wildcard include/config/md5.h) \
    $(wildcard include/config/mmc.h) \
    $(wildcard include/config/cmd/mmc.h) \
    $(wildcard include/config/cmd/fat.h) \
    $(wildcard include/config/fs/fat.h) \
    $(wildcard include/config/cmd/exfat.h) \
    $(wildcard include/config/fs/exfat.h) \
    $(wildcard include/config/fs/ext4.h) \
    $(wildcard include/config/dos/partition.h) \
    $(wildcard include/config/generic/mmc.h) \
    $(wildcard include/config/sdhci.h) \
    $(wildcard include/config/sd/card1/power/pin.h) \
    $(wildcard include/config/sd/card1/on/state.h) \
    $(wildcard include/config/sd/card2/power/pin.h) \
    $(wildcard include/config/sd/card2/on/state.h) \
    $(wildcard include/config/support/emmc/boot.h) \
    $(wildcard include/config/nvt/mmc/channel.h) \
    $(wildcard include/config/nvt/mmc/max/num.h) \
    $(wildcard include/config/fat/write.h) \
    $(wildcard include/config/exfat/write.h) \
    $(wildcard include/config/nvt/nand/ecc.h) \
    $(wildcard include/config/nvt/spi/nand.h) \
    $(wildcard include/config/nvt/nand/type.h) \
    $(wildcard include/config/cmd/nand.h) \
    $(wildcard include/config/sys/max/nand/device.h) \
    $(wildcard include/config/nvt/spi/nor.h) \
    $(wildcard include/config/cmd/sf.h) \
    $(wildcard include/config/spi/flash.h) \
    $(wildcard include/config/mtd/device.h) \
    $(wildcard include/config/spi/flash/macronix.h) \
    $(wildcard include/config/spi/flash/winbond.h) \
    $(wildcard include/config/spi/flash/gigadevice.h) \
    $(wildcard include/config/spi/flash/stmicro.h) \
    $(wildcard include/config/spi/flash/eon.h) \
    $(wildcard include/config/sys/max/sf/device.h) \
    $(wildcard include/config/spi/flash/mtd.h) \
    $(wildcard include/config/sf/default/bus.h) \
    $(wildcard include/config/sf/default/cs.h) \
    $(wildcard include/config/sf/default/mode.h) \
    $(wildcard include/config/sf/default/speed.h) \
    $(wildcard include/config/env/sect/size.h) \
    $(wildcard include/config/rbtree.h) \
    $(wildcard include/config/mtd/partitions.h) \
    $(wildcard include/config/cmd/mtdparts.h) \
    $(wildcard include/config/timestamp.h) \
    $(wildcard include/config/sys/stbc/len.h) \
    $(wildcard include/config/sys/rom/len.h) \
    $(wildcard include/config/sys/no/flash.h) \
    $(wildcard include/config/cmd/imls.h) \
    $(wildcard include/config/modem/support.h) \
    $(wildcard include/config/cmd/saveenv.h) \
    $(wildcard include/config/env/is/in/nand.h) \
    $(wildcard include/config/env/offset.h) \
    $(wildcard include/config/env/range.h) \
    $(wildcard include/config/env/is/in/spi/flash.h) \
    $(wildcard include/config/env/is/nowhere.h) \
    $(wildcard include/config/nvt/mmc/boot.h) \
    $(wildcard include/config/ramdisk/size.h) \
    $(wildcard include/config/image/format/legacy.h) \
    $(wildcard include/config/bootcommand.h) \
  include/linux/sizes.h \
  include/configs/novatek/na51023_mips24k_fpga.h \
    $(wildcard include/config/model/nvt/fpga.h) \
    $(wildcard include/config/mem/base.h) \
    $(wildcard include/config/mem/size.h) \
    $(wildcard include/config/board/rev/addr.h) \
    $(wildcard include/config/board/rev/size.h) \
    $(wildcard include/config/nvt/uimage/size.h) \
    $(wildcard include/config/nvt/uimage/unzip/size.h) \
    $(wildcard include/config/nvt/itron/unzip/size.h) \
    $(wildcard include/config/dsp/sdram/base.h) \
    $(wildcard include/config/dsp/sdram/size.h) \
    $(wildcard include/config/dsp/boot/tmp/buf/addr.h) \
    $(wildcard include/config/dsp/boot/tmp/buf/size.h) \
    $(wildcard include/config/nvt/runfw/sdram/base.h) \
    $(wildcard include/config/ramdisk/sdram/base.h) \
    $(wildcard include/config/ramdisk/sdram/size.h) \
    $(wildcard include/config/itron/sdram/base.h) \
    $(wildcard include/config/itron/sdram/size.h) \
    $(wildcard include/config/nvt/runfw/sdram/size.h) \
    $(wildcard include/config/linux/sdram/start.h) \
    $(wildcard include/config/linux/unzip/sdram/end.h) \
    $(wildcard include/config/linux/sdram/memreserved/base.h) \
    $(wildcard include/config/linux/sdram/memreserved/size.h) \
    $(wildcard include/config/smem/sdram/base.h) \
    $(wildcard include/config/modelext/sdram/base.h) \
    $(wildcard include/config/modelext/sdram/size.h) \
    $(wildcard include/config/nvtipc/shared/mem/addr.h) \
    $(wildcard include/config/nvtipc/shared/mem/size.h) \
    $(wildcard include/config/smem/sdram/size.h) \
    $(wildcard include/config/modelext/spinand/base.h) \
    $(wildcard include/config/modelext/spinor/base.h) \
    $(wildcard include/config/sys/nand/base.h) \
    $(wildcard include/config/sys/hz/clock.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/config.h \
    $(wildcard include/config/h/.h) \
    $(wildcard include/config/sys/generic/global/data.h) \
    $(wildcard include/config/lmb.h) \
    $(wildcard include/config/sys/boot/ramdisk/high.h) \
  include/config_fallbacks.h \
    $(wildcard include/config/fallbacks/h.h) \
    $(wildcard include/config/spl.h) \
    $(wildcard include/config/spl/pad/to.h) \
    $(wildcard include/config/spl/max/size.h) \
    $(wildcard include/config/cmd/ext4.h) \
    $(wildcard include/config/cmd/ext2.h) \
    $(wildcard include/config/cmd/ext4/write.h) \
    $(wildcard include/config/ext4/write.h) \
    $(wildcard include/config/cmd/ide.h) \
    $(wildcard include/config/cmd/sata.h) \
    $(wildcard include/config/cmd/scsi.h) \
    $(wildcard include/config/cmd/usb.h) \
    $(wildcard include/config/cmd/part.h) \
    $(wildcard include/config/cmd/gpt.h) \
    $(wildcard include/config/systemace.h) \
    $(wildcard include/config/partition/uuids.h) \
    $(wildcard include/config/efi/partition.h) \
    $(wildcard include/config/random/uuid.h) \
    $(wildcard include/config/cmd/uuid.h) \
    $(wildcard include/config/bootp/pxe.h) \
    $(wildcard include/config/lib/uuid.h) \
    $(wildcard include/config/lib/rand.h) \
    $(wildcard include/config/lib/hw/rand.h) \
    $(wildcard include/config/api.h) \
    $(wildcard include/config/lcd.h) \
    $(wildcard include/config/cmd/bmp.h) \
    $(wildcard include/config/fit/signature.h) \
    $(wildcard include/config/disable/image/legacy.h) \
  include/config_uncmd_spl.h \
    $(wildcard include/config/uncmd/spl/h//.h) \
    $(wildcard include/config/spl/build.h) \
    $(wildcard include/config/cmd/cdp.h) \
    $(wildcard include/config/cmd/dns.h) \
    $(wildcard include/config/cmd/link/local.h) \
    $(wildcard include/config/cmd/nfs.h) \
    $(wildcard include/config/cmd/rarp.h) \
    $(wildcard include/config/cmd/sntp.h) \
    $(wildcard include/config/cmd/tftpput.h) \
    $(wildcard include/config/cmd/tftpsrv.h) \
  include/asm-offsets.h \
  include/generated/generic-asm-offsets.h \
  include/linux/bitops.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/has/llsc.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/sgidefs.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/system.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/cpu/has/wb.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/ptrace.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/cpu/has/smartmips.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/isadep.h \
    $(wildcard include/config/cpu/r3000.h) \
    $(wildcard include/config/cpu/tx39xx.h) \
  include/linux/string.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/string.h \
  include/linux/linux_string.h \
  include/linux/stringify.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include/stdarg.h \
  include/part.h \
    $(wildcard include/config/lba48.h) \
    $(wildcard include/config/mac/partition.h) \
    $(wildcard include/config/iso/partition.h) \
    $(wildcard include/config/amiga/partition.h) \
  include/ide.h \
    $(wildcard include/config/sys/ide/maxdevice.h) \
    $(wildcard include/config/sys/ide/maxbus.h) \
    $(wildcard include/config/sys/ata/base/addr.h) \
    $(wildcard include/config/ide/led.h) \
    $(wildcard include/config/sys/64bit/lba.h) \
    $(wildcard include/config/ide/preinit.h) \
    $(wildcard include/config/ide/init/postreset.h) \
    $(wildcard include/config/of/ide/fixup.h) \
    $(wildcard include/config/ide/ahb.h) \
  include/flash.h \
    $(wildcard include/config/sys/max/flash/sect.h) \
    $(wildcard include/config/sys/flash/cfi.h) \
    $(wildcard include/config/flash/cfi/mtd.h) \
    $(wildcard include/config/sys/flash/protection.h) \
    $(wildcard include/config/flash/cfi/legacy.h) \
  include/image.h \
    $(wildcard include/config/fit.h) \
    $(wildcard include/config/fit/verbose.h) \
    $(wildcard include/config/spl/crc32/support.h) \
    $(wildcard include/config/spl/md5/support.h) \
    $(wildcard include/config/spl/sha1/support.h) \
    $(wildcard include/config/spl/sha256/support.h) \
    $(wildcard include/config/crc32.h) \
    $(wildcard include/config/sha1.h) \
    $(wildcard include/config/sha256.h) \
    $(wildcard include/config/fit/disable/sha256.h) \
    $(wildcard include/config/sys/boot/get/cmdline.h) \
    $(wildcard include/config/cmd/date.h) \
    $(wildcard include/config/sys/boot/get/kbd.h) \
    $(wildcard include/config/fit/best/match.h) \
    $(wildcard include/config/android/boot/image.h) \
  include/compiler.h \
  /opt/ivot/mipsel-24kec-linux-glibc-4.9-2017.08/usr/lib/gcc/mipsel-24kec-linux-gnu/4.9.3/include/stddef.h \
  include/lmb.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/u-boot.h \
    $(wildcard include/config/sys/generic/board.h) \
  include/command.h \
    $(wildcard include/config/sys/help/cmd/width.h) \
    $(wildcard include/config/cmd/run.h) \
    $(wildcard include/config/cmd/i2c.h) \
    $(wildcard include/config/cmd/itest.h) \
    $(wildcard include/config/cmd/pci.h) \
    $(wildcard include/config/cmd/portio.h) \
    $(wildcard include/config/cmd/bootd.h) \
    $(wildcard include/config/needs/manual/reloc.h) \
  include/linker_lists.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/global_data.h \
    $(wildcard include/config/jzsoc.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/regdef.h \
  include/asm-generic/global_data.h \
    $(wildcard include/config/video.h) \
    $(wildcard include/config/logbuffer.h) \
    $(wildcard include/config/board/types.h) \
    $(wildcard include/config/pre/console/buffer.h) \
    $(wildcard include/config/dm.h) \
    $(wildcard include/config/trace.h) \
    $(wildcard include/config/sys/i2c.h) \
    $(wildcard include/config/sys/i2c/mxc.h) \
    $(wildcard include/config/sys/malloc/f/len.h) \
  include/linux/list.h \
  include/linux/poison.h \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/u-boot-mips.h \
  include/uuid.h \
  include/vsprintf.h \
    $(wildcard include/config/sys/vsnprintf.h) \
  include/u-boot/crc.h \
  include/net.h \
    $(wildcard include/config/sys/rx/eth/buffer.h) \
    $(wildcard include/config/mcast/tftp.h) \
    $(wildcard include/config/bootp/dns2.h) \
  /home/sda3/share2/2019-6-10/950_672_2053_20190531/NA51023_BSP/u-boot/arch/mips/include/asm/cache.h \
  include/bootstage.h \
    $(wildcard include/config/bootstage/user/count.h) \
    $(wildcard include/config/bootstage.h) \
    $(wildcard include/config/show/boot/progress.h) \

board/novatek/nvt-na51023/drv/rtc.o: $(deps_board/novatek/nvt-na51023/drv/rtc.o)

$(deps_board/novatek/nvt-na51023/drv/rtc.o):
