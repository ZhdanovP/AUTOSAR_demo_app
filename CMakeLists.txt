cmake_minimum_required(VERSION 3.15)

project(sample-applications VERSION 1.1.0 LANGUAGES CXX)

include(AUTOSAR/apd)
include(GNUInstallDirs)

find_package(ara-arxmls REQUIRED)
find_package(ara-core REQUIRED)
find_package(ara-gen REQUIRED)
find_package(ara-com REQUIRED)
find_package(ara-per REQUIRED)
find_package(ara-log REQUIRED)
find_package(ara-rest REQUIRED)
find_package(ara-exec-execution-client REQUIRED)
find_package(ara-exec-state-client REQUIRED)
find_package(ara-phm REQUIRED)

add_subdirectory(interfaces)
add_subdirectory(deployment)
add_subdirectory(common-design)
add_subdirectory(applications)
add_subdirectory(machines)
