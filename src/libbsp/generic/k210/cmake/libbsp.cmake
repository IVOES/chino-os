
SET(DT_DIR ${DRIVER_DIR}/devicetree)
SET(CA_DIR ${DT_DIR}/canaan/k210)

SET(SD_DIR ${DRIVER_DIR}/sdio)
INCLUDE_DIRECTORIES(${CA_DIR}/hal/inc)

SET(BSP_SRC ${DT_DIR}/Fdt.cpp
	${DT_DIR}/FdtRoot.cpp
	${CA_DIR}/Boot.c 
	${CA_DIR}/threading/Timer.cpp
	
	${SD_DIR}/DummySdioRoot.cpp)

FILE(GLOB_RECURSE CA_HAL_SRC "${CA_DIR}/hal/*.c")
LIST(APPEND BSP_SRC ${CA_HAL_SRC})

SET(DEVICE_TREE_PATH "${BOARD_CMAKE_DIR}/../devicetree/k210")
ADD_CUSTOM_COMMAND(OUTPUT devicetree.c
		COMMAND dtc -O dtb -o devicetree.dtb ${DEVICE_TREE_PATH}.dts
		COMMAND xxd -i devicetree.dtb | sed 's/unsigned/const unsigned/' > devicetree.c
        COMMENT "Compiling Device Tree ..."
		DEPENDS ${DEVICE_TREE_PATH}.dts)

LIST(APPEND BSP_SRC devicetree.c)

SET(BSP_SRC_ASM ${CA_DIR}/Boot.S)

LIST(APPEND BSP_LIBS fdt)