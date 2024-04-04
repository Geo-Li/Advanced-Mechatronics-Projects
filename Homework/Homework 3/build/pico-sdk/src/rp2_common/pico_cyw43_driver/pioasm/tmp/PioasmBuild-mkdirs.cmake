# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/pico-sdk/tools/pioasm"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/pioasm"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
