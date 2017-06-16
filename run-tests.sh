#!/bin/bash

# Benchmarks
ARM_TEST=test/ARM_M0_11.cpog
INTEL_TEST=test/Intel_8051_37.cpog
TI_TEST=test/TI_MSP_430_8.cpog
PM_TEST=test/Colibrilog.cpog

# Library
TECH_LIB=test/lib/workcraft_no_async.lib

mkdir result
./scenco $ARM_TEST -m all -top 10 -e espresso -a abc -lib $TECH_LIB -res result -ver micro.v
rm -rf result micro.v

mkdir result
./scenco $INTEL_TEST -m all -top 10 -e espresso -a abc -lib $TECH_LIB -res result -ver micro.v
rm -rf result micro.v

mkdir result
./scenco $TI_TEST -m all -top 10 -e espresso -a abc -lib $TECH_LIB -res result -ver micro.v
rm -rf result micro.v

mkdir result
./scenco $PM_TEST -m all -top 10 -e espresso -a abc -lib $TECH_LIB -res result -ver micro.v
rm -rf result micro.v

