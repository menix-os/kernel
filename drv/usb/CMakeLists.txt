# ? USB Drivers

set(MENIX_PARENT_CAT menix_drv_usb)
add_library(${MENIX_PARENT_CAT} INTERFACE)
include(${MENIX_UTIL_PATH})
require_option(usb)

build_module(usb_hid)
