# ? Multiboot 2 support

add_module(boot_mb2 "marv7000" "Boot using the Multiboot2 protocol" MAIN FALSE OFF
	mb2_entry.c
)
conflicts_option(boot_*)
