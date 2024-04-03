
out_path=out
out_elf=test.elf
CFG_USE_C_ONLY=0
CFG_ENABLE_MAIN_FUNCTION=1
CFG_ENABLE_LOWER_RAM=0
CFG_USE_BMP_H=1

if [[ "$1" == "clean" ]]; then
	rm -rf $out_elf
	exit
fi

if [[ "$CFG_USE_C_ONLY" == "1" ]]; then
	gcc src/*.c -I./include -I./finger_print_pictures -lm -DCFG_USE_C_ONLY=$CFG_USE_C_ONLY \
		-DCFG_ENABLE_MAIN_FUNCTION=1 \
		-DCFG_ENABLE_LOWER_RAM=$CFG_ENABLE_LOWER_RAM -DCFG_USE_BMP_H=$CFG_USE_BMP_H -include win2linux.h -o $out_elf
else
	g++ src/*.c -I./include -I./finger_print_pictures -lm -DCFG_USE_C_ONLY=$CFG_USE_C_ONLY \
		-DCFG_ENABLE_MAIN_FUNCTION=1 \
		-DCFG_ENABLE_LOWER_RAM=$CFG_ENABLE_LOWER_RAM -DCFG_USE_BMP_H=$CFG_USE_BMP_H -include win2linux.h -o $out_elf
fi

#armclang --target=arm-arm-none-eabi --std=c++11 src/*.cpp -I./include -include win2linux.h -o $out_elf
