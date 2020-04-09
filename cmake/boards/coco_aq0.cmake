set(CHINO_ARCH arm/armv7-m)
add_compile_options(-mthumb -march=armv7-m -mcpu=cortex-m3 -nostartfiles)

set(CHINO_KERNEL_LDS ${CHINO_ROOT}/src/hal/src/chip/arm/lds/w600.lds)

set(CHINO_KERNEL_STACK_SIZE "8192")