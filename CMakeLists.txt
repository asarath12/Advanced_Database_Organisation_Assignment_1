cmake_minimum_required(VERSION 3.30)
project(DO_Project C)

set(CMAKE_C_STANDARD 99)

add_executable(DO_Project main.c
        dberror.c
        dberror.h
        //storage_mgr.c
        //storage_mgr.h
        test_helper.h
        test_assign1_1.c)
