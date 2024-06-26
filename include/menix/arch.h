//? Architecture specific operations
//? Everything in this header is architecture dependent

#pragma once

// This file is included from arch/.../include/
#include <arch_bits.h>

// Initializes the CPU for use by the kernel.
void arch_init();
