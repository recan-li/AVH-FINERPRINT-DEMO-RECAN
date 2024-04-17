
dos2unix build.sh

target=$1
CFG_NON_MAGICK_API=1
CFG_MATCHING_V1=0
CFG_USE_BMP_H=1
CFG_ENABLE_LOWER_RAM=1
OUT_PATH=out

test -e $OUT_PATH || mkdir -p $OUT_PATH

demos=(fvs_binarize fvs_direction fvs_mask fvs_matching fvs_thinner fvs_createtestimages fvs_enhancer fvs_minutia)

if [[ $target != "" ]]; then
	demos=($1)
fi

for demo in "${demos[@]}"
do 
	gcc -o $OUT_PATH/$demo ./demos/$demo.c ./src/*.c -I./src -I./include \
		-DCFG_NON_MAGICK_API=$CFG_NON_MAGICK_API -DCFG_MATCHING_V1=$CFG_MATCHING_V1 \
		-DCFG_USE_BMP_H=$CFG_USE_BMP_H -DCFG_ENABLE_LOWER_RAM=$CFG_ENABLE_LOWER_RAM \
		-I./fingerprint_bitmaps -lm
done