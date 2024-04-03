
AVH_DEMO_PATH       := AVH-CM7
USER_CONFIG_PATH    := config
OUT_PATH            := out
AVH_SIMLIMIT_TIME   := 8000
SHELL 				:= /bin/bash

all: source clean build run

help:
	@echo "make help 	-> Show this help msg."
	@echo "make source 	-> Install env parameters."
	@echo "make build 	-> Build thie demo."
	@echo "make clean 	-> Clean object files."
	@echo "make run 	-> Run this demo."
	@echo "make debug 	-> Build & run this demo."
	@echo "make all 	-> Source & clean & build & run all together."

debug: build run

source:
	@echo "Copy and source .bashrc ..."
	@cp -rf .bashrc ~/.bashrc
	@source ~/.bashrc
	@echo "Copy CMSIS-Build-Utils.cmake ..."
	@sudo cp -rf cmake/CMSIS-Build-Utils.cmake /opt/ctools/etc/CMSIS-Build-Utils.cmake
	@echo "All parameters have been installed."

build:
	@echo "Building ..."
	@test -e $(OUT_PATH) || mkdir -p $(OUT_PATH)
	cbuild --packs $(AVH_DEMO_PATH)/VHT_MPS2_Cortex-M7.cprj --update-rte
	@cp -rf $(AVH_DEMO_PATH)/Objects/image.elf $(OUT_PATH)

run:
	@echo "Running ..."
	/opt/VHT/bin/FVP_MPS2_Cortex-M7 --stat --simlimit $(AVH_SIMLIMIT_TIME) -f $(AVH_DEMO_PATH)/vht_config.txt $(OUT_PATH)/image.elf

clean:
	@echo "Clean ..."
	rm -rf $(AVH_DEMO_PATH)/Objects/
	rm -rf $(OUT_PATH)

.PHONY: all source clean build run help
