
TEMPLATE = subdirs

SUBDIRS += \
    $${PWD}/dll_library_init_deinit_test            \
    $${PWD}/bin_library_init_deinit_test/bin_library_init_deinit_test.pro


$${PWD}/bin_library_init_deinit_test/bin_library_init_deinit_test.pro.depend = $${PWD}/dll_library_init_deinit_test

OTHER_FILES += \
    $${PWD}/../../linux.Makefile
