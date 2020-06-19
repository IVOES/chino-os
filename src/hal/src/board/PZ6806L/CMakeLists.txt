cmake_minimum_required (VERSION 3.13)

set(SRCS ../../arch/arm/startup.cpp
         ../../chip/st/stm32f1xx_hd/boot_print.cpp)

target_sources(hal PRIVATE ${SRCS})
target_link_libraries(hal PRIVATE incbin)