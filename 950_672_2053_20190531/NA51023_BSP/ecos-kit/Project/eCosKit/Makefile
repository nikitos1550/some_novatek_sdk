#----------------------------------------------------------------------
# set the ROOT directory here
#----------------------------------------------------------------------
ROOT_DIR = ../..

MAKE_COMMON_DIR = $(ROOT_DIR)/MakeCommon
include $(MAKE_COMMON_DIR)/MakeOption.txt

UITRON_DIR = ../../../uitron
include $(UITRON_DIR)/Project/ProjectConfig.txt
include $(UITRON_DIR)/Project/$(ACTIVE_PROJECT)/ModelConfig.txt
include $(UITRON_DIR)/Project/$(ACTIVE_PROJECT)/ModelConfig_$(MODEL).txt
#include ModelConfig.txt
#include ModelConfig_$(MODEL).txt
include MakeConfig.txt

#----------------------------------------------------------------------
# set the project name here
#----------------------------------------------------------------------
PRJ_NAME = eCosKit

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
    $(ROOT_DIR)/Include

#----------------------------------------------------------------------
# Add/delete the project C source and assembly files here
#----------------------------------------------------------------------

ASM = \
    SrcCode/Kernel/CodeInfo.s

SRC = \
    SrcCode/Kernel/BinInfo.c

ifeq "$(USE_ECOS_KERNEL)" "ON"

SRC += \
    SrcCode/Ecos/ecos_main.c \
    SrcCode/Ecos/KerExcept.c \
    SrcCode/Ecos/init_ipc.c  \
    SrcCode/Ecos/init_cmd.c  \
    SrcCode/Ecos/cmd_usage.c  \
    SrcCode/Ecos/cmd_misc.c  \
    SrcCode/Exam/test_main.c \
    SrcCode/Exam/test_ecos.c \
    SrcCode/Exam/test_nvtipc.c \
    SrcCode/Exam/test_fsipc.c \
#    SrcCode/Ecos/init_inet.c  \
#    SrcCode/Exam/test_inet.c \
#    SrcCode/Exam/test_udp.c \
#    SrcCode/Ecos/cmd_iperf.c  \
#    SrcCode/Exam/dump_netif_hwaddr.c \
#    SrcCode/Exam/test_ftpd.c \
#    SrcCode/Exam/dump_all_netif_hwaddr.c \
#    SrcCode/Exam/test_posix.c \
#    SrcCode/Exam/test_dhcpsrv.c \
#    SrcCode/Exam/test_dhcpcli.c \
#    SrcCode/Exam/test_dhcp.c \
#    SrcCode/Exam/test_ping.c \
#    SrcCode/Exam/test_iperf.c \
#    SrcCode/Exam/test_setip.c \
#    SrcCode/Exam/test_clib.c \
#    SrcCode/Exam/test_info.c \
#    SrcCode/Exam/udp_broadcast_send.c \
#    SrcCode/Exam/udp_broadcast_recv.c

#    SrcCode/Exam/test_wifi.c \

endif

ifeq "$(NETWORK)" "NETWORK_SDIO_EVB_WIFI_BRCM"
SRC += \
    SrcCode/Ecos/test_brcwifi.c \
    SrcCode/Ecos/test_ping.c \

endif

ifeq "$(NETWORK)" "NETWORK_SDIO_EVB_WIFI"
SRC += \
    SrcCode/Ecos/test_wifi.c \
    SrcCode/Ecos/test_ping.c \

endif

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = $(PRJ_NAME)_Data

#----------------------------------------------------------------------
# set the library directory here
#----------------------------------------------------------------------
ARC_BASE_DIR = $(ROOT_DIR)/ARC

#----------------------------------------------------------------------
# add additional C and assembly flags here, for example, -Otime
#----------------------------------------------------------------------
CFLAGS_D = $(C_DEFINE_EXT)

ASMFLAGS_D =

CFLAGS_R = $(C_DEFINE_EXT)

ASMFLAGS_R =

GCCFLAGS = $(C_DEFINE_EXT)

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
include $(MAKE_COMMON_DIR)/OutputImg.txt
