cmake_minimum_required(VERSION 3.15)

set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)

project(noise_detector)

pico_sdk_init()

add_executable(noise_detector noise_detector.c)
target_link_libraries(noise_detector pico_stdlib hardware_adc)

# usb output, please
pico_enable_stdio_usb(noise_detector 1)
pico_enable_stdio_uart(noise_detector 0)

pico_add_extra_outputs(noise_detector)
