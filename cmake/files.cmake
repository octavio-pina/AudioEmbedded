# Add sources to executable/library
target_sources(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/BSP/fat32.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/BSP/sd_card.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/BSP/wav.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Drivers/gpio_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Drivers/nvic_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Drivers/ring_buffer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Drivers/spi_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Drivers/timer_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/Drivers/uart_driver.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/syscalls.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/sysmem.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Startup/startup_stm32f411retx.s"
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/STM32F411RETX_FLASH.ld" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/STM32F411RETX_FLASH.ld")
