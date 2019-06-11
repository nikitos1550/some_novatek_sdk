############################################################################
#
# Makefile -- Top level makefile.
#
# Copyright (c) 2016, Novatek
#
############################################################################
# common
MULTI_CORES ?= $(shell grep -c ^processor /proc/cpuinfo)
SHELL = /bin/bash
PYTHON := python
PWD = $(shell pwd)

# nvt modelconfig
include $(NVT_PRJCFG_MODEL_CFG)
-include $(BUILD_DIR)/definitions.mk

# kernel & modules
CUSTBOARDCONFIG := $(strip $(shell echo $(NVT_PRJCFG_MODEL_CFG) | grep NVT_CFG_KERNEL_CFG | awk -F'=' '{print $$NF;}'))
ifeq ($(EMBMEM),EMBMEM_SPI_NOR)
BOARDCONFIG := $(shell if [ ! -z $(CUSTBOARDCONFIG) ]; then echo $(CUSTBOARDCONFIG)_nor_debug; else echo na51023_IPCAM_defconfig_nor_debug; fi)
#BOARDCONFIG := $(shell if [ ! -z $(CUSTBOARDCONFIG) ]; then echo $(CUSTBOARDCONFIG)_nor_release; else echo na51023_IPCAM_defconfig_nor_release; fi)
else
BOARDCONFIG := $(shell if [ ! -z $(CUSTBOARDCONFIG) ]; then echo $(CUSTBOARDCONFIG)_debug; else echo na51023_IPCAM_defconfig_debug; fi)
#BOARDCONFIG := $(shell if [ ! -z $(CUSTBOARDCONFIG) ]; then echo $(CUSTBOARDCONFIG)_release; else echo na51023_IPCAM_defconfig_release; fi)
endif

KERNEL_OUTPUT := $(KERNELDIR)/OUTPUT
KERNEL_IMGNAME := uImage.bin
KERNEL_HEADER := linux_header
CPU2_OS_TYPE := $(shell echo $(CPU2_TYPE))
linux_load_addr := $(shell $(call gen_linux_loadbase))

# uboot
UBOOT_IMGNAME := u-boot.bin

# busybox
#BUSYBOX_CFG := busybox_cfg_full
BUSYBOX_CFG := busybox_cfg_small

# log
log_stdout = echo -e "\e[1;44m$@: Build start\e[0m"; \
		$(1) 2> >(tee -a $(LOGS_DIR)/$@.log_err) > >(tee -a $(LOGS_DIR)/$@.log); \
		ret=$$PIPESTATUS; \
		echo -e "\e[1;44m$@: Build finish\e[0m"; \
		cat $(LOGS_DIR)/$@.log_err >&2; \
		if [ -z "`cat $(LOGS_DIR)/$@.log_err`" ]; then \
			rm $(LOGS_DIR)/$@.log_err; \
		fi; \
		exit $$ret;
log_remove = if [ ! -e $(LOGS_DIR) ]; then mkdir $(LOGS_DIR); fi; if [ -e $(LOGS_DIR)/$@.log ]; then rm $(LOGS_DIR)/$@.log; fi; if [ -e $(LOGS_DIR)/$@.log_err ]; then rm $(LOGS_DIR)/$@.log_err; fi;

# misc
BUILD_COMPLETE_STRING ?= $(shell date "+%a, %d %b %Y %T %z")
UID := $(shell id -u)
GID := $(shell id -g)
removeimg = $(foreach a,$(1),$(if $(wildcard $(a)),rm -rf $(a)))
unexport KBUILD_OUTPUT

# linux rtc driver
export CONFIG_NVT_RTC_CLOCK = $(shell echo $(CLOCK))
ifeq ($(CONFIG_NVT_RTC_CLOCK),CLOCK_DRTC)
	export NVT_DRTC_CLOCK = 1
else
	export NVT_DRTC_CLOCK = 0
endif

# ipcam ui style 
export CONFIG_NVT_UI_STYLE = $(shell echo $(UI_STYLE))
ifeq ($(CONFIG_NVT_UI_STYLE),UI_STYLE_IPCAM)
	export NVT_UI_STYLE_IPCAM = 1
else
	export NVT_UI_STYLE_IPCAM = 0
endif

ifeq ($(CPU2_OS_TYPE),CPU2_NONE)
all: uitron uboot pack
else ifeq ($(CPU2_OS_TYPE),CPU2_ECOS)
all: uitron uboot ecos-kit pack
else
all: uitron linux modules supplement uboot busybox library app sample rootfs pack
endif
	@echo "Finish generating images at $(BUILD_COMPLETE_STRING)"
	@$(call update_modelcfg_to_build)

.PHONY: rootfs uboot linux modules tools app sample

pack:
	@$(call gen_nvt_pack)

checkenv:
	@if [ -z $(LINUX_BUILD_TOP) ]; then \
		echo -e "\r\nERROR :Please source build/envsetup.sh in NA51000_BSP firstly to have auto copyso function\r\n"; \
		exit 1; \
	fi
	@if [ $(PWD) != $(LINUX_BUILD_TOP) ]; then \
		echo -e "\r\nERROR: Current path is not the same as the environment path ("$(LINUX_BUILD_TOP)")\r\n"; \
		exit 1; \
	fi
	@if [ ! -e $(OUTPUT_DIR) ]; then \
		mkdir $(OUTPUT_DIR); \
	fi
ifeq ($(CPU2_OS_TYPE),CPU2_LINUX)
	@# Generate etc link
	make -C ${ROOTFS_DIR} gen_etc
endif

dep:
	@$(call make_dep_by_modelselect)

linux: checkenv
	@if [ ! -e $(KERNEL_OUTPUT) ]; then \
		mkdir $(KERNEL_OUTPUT); \
	fi
	@echo BOARDCONFIG = $(BOARDCONFIG)
	@cp $(KERNELDIR)/arch/$(ARCH)/configs/$(BOARDCONFIG) $(KERNEL_OUTPUT)/.config
	@echo "##### Build kernel #####"
	@$(call log_remove)
	@echo ">>>>> build kernel with CONFIG_NVT_KERNEL_LOAD_ADDR=$(linux_load_addr) <<<<<"
	@$(call log_stdout, CONFIG_NVT_KERNEL_LOAD_ADDR=$(linux_load_addr) make CFLAGS_KERNEL="-DNVT_RTC_CLOCK=$(NVT_DRTC_CLOCK) -DNVT_IPCAM_MODEL=$(NVT_UI_STYLE_IPCAM) -DCONFIG_NVT_KERNEL_LOAD_ADDR=$(linux_load_addr)" -C $(KERNELDIR) O=$(KERNEL_OUTPUT) uImage -j$(MULTI_CORES))
	@cp -af $(KERNEL_OUTPUT)/arch/$(ARCH)/boot/uImage* $(OUTPUT_DIR)/
	@mv $(OUTPUT_DIR)/uImage $(OUTPUT_DIR)/$(KERNEL_IMGNAME)

modules: checkenv
	@if [ ! -e $(KERNEL_OUTPUT) ]; then \
		echo "##### Please run \"make linux\" firstly #####";\
		exit 1;\
	fi
	@$(call log_remove)
	@$(call log_stdout, make -C $(KERNELDIR) O=$(KERNEL_OUTPUT) modules -j$(MULTI_CORES))
	@echo "##### Build modules #####"
	@rm -rf $(KERNEL_OUTPUT)/_install_modules/
	@$(call log_stdout, make -C $(KERNELDIR) O=$(KERNEL_OUTPUT) INSTALL_MOD_PATH=$(KERNEL_OUTPUT)/_install_modules/ modules_install -j$(MULTI_CORES))
	# copy kernel objects to rootfs
	@echo "##### Copy kernel objects to rootFS #####"
	@cp -af $(KERNEL_OUTPUT)/_install_modules/lib/modules $(ROOTFS_DIR)/rootfs/lib/

supplement: checkenv
	@if [ ! -e $(KERNEL_OUTPUT)/_install_modules ]; then \
		echo "##### Please run \"make modules\" firstly #####";\
		exit 1;\
	fi
	@echo "##### Build supplement #####"
	@$(call log_remove)
	@$(call log_stdout, make -C $(SUPPLEMENT_DIR) modules)
	@$(call log_stdout, make -C $(SUPPLEMENT_DIR) modules_install)
	@echo "##### Copy modules to rootFS #####"
	cp -af $(KERNEL_OUTPUT)/_install_modules/lib/modules $(ROOTFS_DIR)/rootfs/lib/

uboot: checkenv
	@echo "##### Build u-boot loader #####"
	@$(call log_remove)
	@$(call log_stdout, make -C $(UBOOT_DIR) O="" distclean)
	@$(call log_stdout, make -C $(UBOOT_DIR) O="" nvt-na51023_config)
	@$(call log_stdout, make -C $(UBOOT_DIR) O="")
	@$(call log_stdout, make -C $(UBOOT_DIR) O="" env)
	@$(UBOOT_DIR)/tools/encrypt_bin SUM $(UBOOT_DIR)/u-boot.bin 0x350 ub51023
	@$(BUILD_DIR)/nvt-tools/bfc c lz $(UBOOT_DIR)/u-boot.bin $(UBOOT_DIR)/u-boot.lz.bin 0 0
	@cp -af $(UBOOT_DIR)/u-boot.bin $(UBOOT_DIR)/u-boot.lz.bin $(OUTPUT_DIR)/

busybox: checkenv
	@echo "##### Build busybox #####"
	cp $(BUSYBOX_DIR)/$(BUSYBOX_CFG) $(BUSYBOX_DIR)/.config
	@$(call log_remove)
	@$(call log_stdout, make -C $(BUSYBOX_DIR) -j$(MULTI_CORES))
	@$(call log_stdout, make -C $(BUSYBOX_DIR) -j$(MULTI_CORES) install)
	@cp -af $(BUSYBOX_DIR)/_install/* $(ROOTFS_DIR)/rootfs

library: checkenv
	@echo "##### Build library #####"
	@$(call log_remove)
	@$(call log_stdout, make -C ${LIBRARY_DIR})
	@$(call log_stdout, make -C ${LIBRARY_DIR} install)

rootfs: checkenv
	@echo "##### Build rootfs #####"
	@$(call log_remove)
	@$(call log_stdout, make -C $(ROOTFS_DIR))
	@$(call log_stdout, make -C $(ROOTFS_DIR) install)

sample: checkenv
	@echo "##### Build device driver test #####"
	@$(call log_remove)
	@$(call log_stdout, make -C ${SAMPLE_DIR})
	@$(call log_stdout, make -C ${SAMPLE_DIR} install)

app: checkenv
	@echo "##### Build app #####"
	@$(call log_remove)
	@$(call log_stdout, make -C ${APP_DIR})
	@$(call log_stdout, make -C ${APP_DIR} install)

tools: checkenv
	@echo "##### Build tools #####"
	@cd ${TOOLS_DIR}; make; make install;

uitron: checkenv
	@echo "##### Build uitron #####"
	@$(call log_remove)
	@$(call log_stdout, cd ${UITRON_DIR}/MakeCommon; make release -j$(MULTI_CORES))
	@cd `dirname $(NVT_PRJCFG_MODEL_CFG)`/`echo $(NVT_PRJCFG_MODEL_CFG) | awk -F/ '{print $$(NF-1)}'`*/*/; cp FW*.bin $(OUTPUT_DIR)

ecos-kit: checkenv
	@echo "##### Build 2nd core ecos #####"
	@$(call log_remove)
	@$(call log_stdout, cd ${ECOS_DIR}/MakeCommon; make release -j$(MULTI_CORES))
	@cd ${ECOS_DIR}/Project/eCosKit/eCosKit_Data/Release/; cp ecos-kit.bin $(OUTPUT_DIR)

cfg: checkenv
	@echo "##### Build config #####"
	@$(call log_remove)
	@$(call log_stdout, cd ${CONFIG_DIR}; make)

busybox_config: checkenv
	@echo "##### busybox Menuconfig #####"
	cp $(BUSYBOX_DIR)/$(BUSYBOX_CFG) $(BUSYBOX_DIR)/.config
	make -C $(BUSYBOX_DIR) -j$(MULTI_CORES) menuconfig
	cp $(BUSYBOX_DIR)/.config $(BUSYBOX_DIR)/$(BUSYBOX_CFG)

linux_config: checkenv
	@echo "##### linux kernel Menuconfig #####"
	@if [ ! -e $(KERNEL_OUTPUT) ]; then \
		mkdir $(KERNEL_OUTPUT); \
	fi
	@cp -ap $(KERNELDIR)/arch/$(ARCH)/configs/$(BOARDCONFIG) $(KERNEL_OUTPUT)/.config
	@make -C $(KERNELDIR) O=$(KERNEL_OUTPUT) menuconfig
	@cp $(KERNEL_OUTPUT)/.config $(KERNELDIR)/arch/$(ARCH)/configs/$(BOARDCONFIG)

linux_header: checkenv
	@echo "##### generate linux-kernel out of tree headers #####"
	@if [ ! -e $(KERNEL_OUTPUT) ]; then \
		mkdir $(KERNEL_OUTPUT); \
	fi
	@cp -ap $(KERNELDIR)/arch/$(ARCH)/configs/$(BOARDCONFIG) $(KERNEL_OUTPUT)/.config
	@$(call log_remove)
	@$(call log_stdout, make -C $(KERNELDIR) O=$(KERNEL_OUTPUT) prepare)
	@$(call log_stdout, make -C $(KERNELDIR) O=$(KERNEL_OUTPUT) scripts)

listpackage:
	make -C ${APP_DIR} listpackage

pack_clean:
	@$(call rm_nvt_pack)

linux_clean: checkenv
	@echo "##### Clean kernel related objects and config files #####"
	@$(call log_remove)
	@if [ -e $(KERNEL_OUTPUT) ]; then \
		$(call log_stdout, make -C $(KERNELDIR) O=$(KERNEL_OUTPUT) mrproper -j4) \
	fi
	$(call removeimg,$(KERNEL_OUTPUT) $(OUTPUT_DIR)/$(KERNEL_IMGNAME))

supplement_clean: checkenv
	@echo "##### Clean supplement modules #####"
	@$(call log_remove)
	@$(call log_stdout, make -C $(SUPPLEMENT_DIR) clean)
	@$(call removeimg,$(KERNEL_OUTPUT)/_install_modules/)

uboot_clean: checkenv
	@echo "##### Clean u-boot loader #####"
	@$(call log_remove)
	@$(call log_stdout, make -C ${UBOOT_DIR} O="" distclean)
	@$(call removeimg,$(UBOOT_DIR)/u-boot.bin $(OUTPUT_DIR)/$(UBOOT_IMGNAME))

busybox_clean: checkenv
	@echo "##### Clean busybox #####"
	@$(call log_remove)
	@$(call log_stdout, make -C ${BUSYBOX_DIR} clean)

library_clean: checkenv
	@echo "##### Clean library #####"
	@$(call log_remove)
	@$(call log_stdout, make -C ${LIBRARY_DIR} clean)

rootfs_clean: checkenv
	@echo "##### Clean rootfs #####"
	@$(call log_remove)
	@$(call log_stdout, make -C ${ROOTFS_DIR} clean)

sample_clean: checkenv
	@echo "##### Clean device driver test #####"
	@$(call log_remove)
	@$(call log_stdout, make -C ${SAMPLE_DIR} clean)

app_clean: checkenv
	@echo "##### Clean applications #####"
	@$(call log_remove)
	@$(call log_stdout, make -C ${APP_DIR} clean)

tools_clean: checkenv
	@echo "##### Clean tools #####"
	@cd ${TOOLS_DIR}; make clean;

uitron_clean: checkenv
	@echo "##### Clean uitron #####"
	@$(call log_remove)
	@$(call log_stdout, cd ${UITRON_DIR}/MakeCommon; make clean)
	@rm -rf $(OUTPUT_DIR)/FW*A.bin $(OUTPUT_DIR)/FW*T.bin

ecos-kit_clean: checkenv
	@echo "##### Clean ecos-kit #####"
	@$(call log_remove)
	@$(call log_stdout, cd ${ECOS_DIR}/MakeCommon; make clean)
	@rm -rf $(OUTPUT_DIR)/ecos-kit.bin

cfg_clean: checkenv
	@echo "##### Clean config #####"
	@$(call log_remove)
	@$(call log_stdout, cd ${CONFIG_DIR}; make clean)

ifeq ($(CPU2_OS_TYPE),CPU2_NONE)
clean: rm_logs uitron_clean uboot_clean pack_clean
else ifeq ($(CPU2_OS_TYPE),CPU2_ECOS)
clean: rm_logs uitron_clean ecos-kit_clean uboot_clean pack_clean
else
clean: rm_logs supplement_clean uboot_clean linux_clean busybox_clean rootfs_clean tools_clean library_clean app_clean uitron_clean sample_clean pack_clean
endif
	@rm -rf $(OUTPUT_DIR)
	@echo "make clean completed"

rm_logs:
	$(call removeimg,$(LOGS_DIR))

showenv:
	@echo $(PATH)

help:
	@echo "====================================================="
	@echo "make help             -> show make command info"
	@echo "make all              -> build all"
	@echo "make linux            -> build linux-kernel"
	@echo "make modules          -> build built-in kernel modules"
	@echo "make supplement       -> build supplement modules"
	@echo "make uboot            -> build loader(uboot)"
	@echo "make library          -> build library"
	@echo "make busybox          -> build busybox"
	@echo "make rootfs           -> build rootfs"
	@echo "make app              -> build applications"
	@echo "make tools            -> build tools"
	@echo "make sample           -> build sample code"
	@echo "make uitron           -> build uitron code"
	@echo "make pack             -> Generate nvtpack image"
	@echo "====================================================="
	@echo "make linux_config     -> config linux-kernel"
	@echo "make busybox_config   -> config busybox"
	@echo "make linux_header     -> generate linux-kernel out of tree headers"
	@echo "====================================================="
	@echo "make clean            -> clean all"
	@echo "make linux_clean      -> clean linux-kernel & built-in kernel modules"
	@echo "make supplement_clean -> clean supplement modules"
	@echo "make uboot_clean      -> clean loader(uboot)"
	@echo "make library_clean    -> clean library"
	@echo "make busybox_clean    -> clean busybox"
	@echo "make rootfs_clean     -> clean rootfs"
	@echo "make app_clean        -> clean applications"
	@echo "make tools_clean      -> clean tools"
	@echo "make sample_clean     -> clean sample code"
	@echo "make uitron_clean     -> clean uitron code"
	@echo "make pack_clean       -> Remove nvtpack image"
	@echo "====================================================="

header:
	@echo "Create kernel headers for external driver building, please wait..."
	@cd ${KERNELDIR}; \
	mkdir -p ${KERNEL_HEADER}/arch; \
	mkdir -p ${KERNEL_HEADER}/include/config; \
	mkdir -p ${KERNEL_HEADER}/include/generated; \
	cp -rf arch/${ARCH} ${KERNEL_HEADER}/arch; \
	cp -a usr drivers include init Kbuild Makefile $(KERNEL_OUTPUT)/modules.order $(KERNEL_OUTPUT)/Module.symvers scripts $(KERNEL_OUTPUT)/.config ${KERNEL_HEADER}; \
	cp -a $(KERNEL_OUTPUT)/include/config/auto.conf ${KERNEL_HEADER}/include/config/; \
	cp -a $(KERNEL_OUTPUT)/include/generated/autoconf.h ${KERNEL_HEADER}/include/generated/; \
	find ${KERNEL_HEADER} -name "*.o" | xargs rm -rf; \
	find ${KERNEL_HEADER} -name "*.svn" | xargs rm -rf; \
	find ${KERNEL_HEADER}/arch -name "*.c" | xargs rm -rf; \
	find ${KERNEL_HEADER}/drivers -name "*.c" | xargs rm -rf; \
	find ${KERNEL_HEADER}/init -name "*.c" | xargs rm -rf; \
	tar -czvf $(OUTPUT_DIR)/${KERNEL_HEADER}.tgz ${KERNEL_HEADER}; \
	rm -rf ${KERNEL_HEADER}; \
	cd -;

header_clean:
	@echo "##### Clean header #####"
	@$(call log_remove)
	@$(call removeimg,$(OUTPUT_DIR)/${KERNEL_HEADER}.tgz)
