# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/pico-sdk/tools/elf2uf2"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/elf2uf2"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/print/elf2uf2"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/print/elf2uf2/tmp"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/print/elf2uf2/src/ELF2UF2Build-stamp"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/print/elf2uf2/src"
  "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/print/elf2uf2/src/ELF2UF2Build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/print/elf2uf2/src/ELF2UF2Build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/geo/Desktop/Education/Northwestern University/2024 Spring/ME433/me433_sandbox/hw2-4/build/print/elf2uf2/src/ELF2UF2Build-stamp${cfgdir}") # cfgdir has leading slash
endif()
