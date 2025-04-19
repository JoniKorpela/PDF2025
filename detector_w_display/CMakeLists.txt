# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.15)

set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)

project(noise_detector)

pico_sdk_init()

add_executable(noise_detector noise_detector.c display.c buzzer.c)
target_link_libraries(noise_detector pico_stdlib hardware_adc hardware_i2c hardware_pwm)

# usb output, please
pico_enable_stdio_usb(noise_detector 1)
pico_enable_stdio_uart(noise_detector 0)

pico_add_extra_outputs(noise_detector)
