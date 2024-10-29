# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/ADMIN/OneDrive/Desktop/esp-idf/components/bootloader/subproject"
  "D:/nymble/ESP_UART/build/bootloader"
  "D:/nymble/ESP_UART/build/bootloader-prefix"
  "D:/nymble/ESP_UART/build/bootloader-prefix/tmp"
  "D:/nymble/ESP_UART/build/bootloader-prefix/src/bootloader-stamp"
  "D:/nymble/ESP_UART/build/bootloader-prefix/src"
  "D:/nymble/ESP_UART/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/nymble/ESP_UART/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/nymble/ESP_UART/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
