
include_directories(../include)

list(APPEND TCPIP_TEST_DRV_SOURCE
    ../kmtest_drv/kmtest_standalone.c
    connect.c
    tdi.c
    TcpIp_drv.c)

add_library(tcpip_drv MODULE ${TCPIP_TEST_DRV_SOURCE})
set_module_type(tcpip_drv kernelmodedriver)
target_link_libraries(tcpip_drv kmtest_printf ${PSEH_LIB})
add_importlibs(tcpip_drv ntoskrnl hal)
target_compile_definitions(tcpip_drv PRIVATE KMT_STANDALONE_DRIVER)
#add_pch(example_drv ../include/kmt_test.h)
add_rostests_file(TARGET tcpip_drv)
