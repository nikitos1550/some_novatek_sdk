DemoKit_Data/Release/ObjectLds/MODELEXT_X1/MODELEXT_X1.a:../../ARC_eCos/Drv/Release/MODELEXT_X1.a
	@echo Copying ObjectLds:  $(notdir $@)
	@rm -rf $(dir $@)
	@mkdir -p $(dir $@)
	cp $< $@
	chmod 777 $@
	@echo Extracting: DemoKit_Data/Release/ObjectLds/MODELEXT_X1/modelext_info.o
	$(AR) x $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/modelext_info.o)
	$(AR) d $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/modelext_info.o)
	mv $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/modelext_info.o) DemoKit_Data/Release/ObjectLds/MODELEXT_X1/modelext_info.o;
	@echo Extracting: DemoKit_Data/Release/ObjectLds/MODELEXT_X1/bin_info.o
	$(AR) x $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/bin_info.o)
	$(AR) d $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/bin_info.o)
	mv $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/bin_info.o) DemoKit_Data/Release/ObjectLds/MODELEXT_X1/bin_info.o;
	@echo Extracting: DemoKit_Data/Release/ObjectLds/MODELEXT_X1/pinmux_cfg.o
	$(AR) x $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/pinmux_cfg.o)
	$(AR) d $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/pinmux_cfg.o)
	mv $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/pinmux_cfg.o) DemoKit_Data/Release/ObjectLds/MODELEXT_X1/pinmux_cfg.o;
	@echo Extracting: DemoKit_Data/Release/ObjectLds/MODELEXT_X1/intdir_cfg.o
	$(AR) x $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/intdir_cfg.o)
	$(AR) d $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/intdir_cfg.o)
	mv $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/intdir_cfg.o) DemoKit_Data/Release/ObjectLds/MODELEXT_X1/intdir_cfg.o;
	@echo Extracting: DemoKit_Data/Release/ObjectLds/MODELEXT_X1/emb_partition_info.o
	$(AR) x $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/emb_partition_info.o)
	$(AR) d $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/emb_partition_info.o)
	mv $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/emb_partition_info.o) DemoKit_Data/Release/ObjectLds/MODELEXT_X1/emb_partition_info.o;
	@echo Extracting: DemoKit_Data/Release/ObjectLds/MODELEXT_X1/gpio_info.o
	$(AR) x $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/gpio_info.o)
	$(AR) d $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/gpio_info.o)
	mv $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/gpio_info.o) DemoKit_Data/Release/ObjectLds/MODELEXT_X1/gpio_info.o;
	@echo Extracting: DemoKit_Data/Release/ObjectLds/MODELEXT_X1/dram_partition_info.o
	$(AR) x $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/dram_partition_info.o)
	$(AR) d $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/dram_partition_info.o)
	mv $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/dram_partition_info.o) DemoKit_Data/Release/ObjectLds/MODELEXT_X1/dram_partition_info.o;
	@echo Extracting: DemoKit_Data/Release/ObjectLds/MODELEXT_X1/model_cfg.o
	$(AR) x $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/model_cfg.o)
	$(AR) d $@ $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/model_cfg.o)
	mv $(notdir DemoKit_Data/Release/ObjectLds/MODELEXT_X1/model_cfg.o) DemoKit_Data/Release/ObjectLds/MODELEXT_X1/model_cfg.o;

DemoKit_Data/Release/ObjectLds/Driver_Sys/Driver_Sys.a:../../ARC_eCos/Drv/Release/Driver_Sys.a
	@echo Copying ObjectLds:  $(notdir $@)
	@rm -rf $(dir $@)
	@mkdir -p $(dir $@)
	cp $< $@
	chmod 777 $@
	@echo Extracting: DemoKit_Data/Release/ObjectLds/Driver_Sys/clock_power_down.o
	$(AR) x $@ $(notdir DemoKit_Data/Release/ObjectLds/Driver_Sys/clock_power_down.o)
	$(AR) d $@ $(notdir DemoKit_Data/Release/ObjectLds/Driver_Sys/clock_power_down.o)
	mv $(notdir DemoKit_Data/Release/ObjectLds/Driver_Sys/clock_power_down.o) DemoKit_Data/Release/ObjectLds/Driver_Sys/clock_power_down.o;

