/*-------------------------------
Universal ELF spec implementation
-------------------------------*/

#pragma once

#include <menix/common.h>

// ELF Header Identification
#define EI_MAG0		  0		// File identification
#define EI_MAG1		  1		//
#define EI_MAG2		  2		//
#define EI_MAG3		  3		//
#define EI_CLASS	  4		// File class
#define EI_DATA		  5		// Data encoding
#define EI_VERSION	  6		// File version
#define EI_OSABI	  7		// OS/ABI identification
#define EI_ABIVERSION 8		// ABI version
#define EI_PAD		  9		// Start of padding bytes
#define EI_NIDENT	  16	// Size of e_ident[]
// ELF Header Type
#define ET_NONE		  0
#define ET_REL		  1
#define ET_EXEC		  2
#define ET_DYN		  3
#define ET_CORE		  4
// Program Header Type
#define PT_NULL		  0x00000000
#define PT_LOAD		  0x00000001
#define PT_DYNAMIC	  0x00000002
#define PT_INTERP	  0x00000003
#define PT_NOTE		  0x00000004
#define PT_SHLIB	  0x00000005
#define PT_PHDR		  0x00000006
#define PT_TLS		  0x00000007
// Program Header Flags
#define PF_X		  0x01
#define PF_W		  0x02
#define PF_R		  0x04
// Section Header
#define SHF_WRITE	  0x1			// Section contains writable data
#define SHF_ALLOC	  0x2			// Section is allocated in memory image of program
#define SHF_EXECINSTR 0x4			// Section contains executable instructions
#define SHF_MASKOS	  0x0F000000	// Environment - specific use
#define SHF_MASKPROC  0xF0000000	// Processor - specific use
// Section Header Linkage Type
#define SHT_NULL	  0		// Marks an unused section header
#define SHT_PROGBITS  1		// Contains information defined by the program
#define SHT_SYMTAB	  2		// Contains a linker symbol table
#define SHT_STRTAB	  3		// Contains a string table
#define SHT_RELA	  4		// Contains “Rela” type relocation entries
#define SHT_HASH	  5		// Contains a symbol hash table
#define SHT_DYNAMIC	  6		// Contains dynamic linking tables
#define SHT_NOTE	  7		// Contains note information
#define SHT_NOBITS	  8		// Contains uninitialized space; does not occupy any space in the file
#define SHT_REL		  9		// Contains “Rel” type relocation entries
#define SHT_SHLIB	  10	// Reserved
#define SHT_DYNSYM	  11	// Contains a dynamic loader symbol table

#define ELF_ST_BIND(i)	  ((i) >> 4)
#define ELF_ST_TYPE(i)	  ((i) & 0xf)
#define ELF_ST_INFO(b, t) (((b) << 4) + ((t) & 0xf))

#ifdef CONFIG_64bit
#define ELF_R_SYM(i)	 ((i) >> 32)
#define ELF_R_TYPE(i)	 ((i) & 0xffffffffL)
#define ELF_R_INFO(s, t) (((s) << 32) + ((t) & 0xffffffffL))
#else
#define ELF_R_SYM(i)	 ((i) >> 8)
#define ELF_R_TYPE(i)	 ((uint8_t)(i))
#define ELF_R_INFO(s, t) (((s) << 8) + (uint8_t)(t))
#endif

#ifdef CONFIG_64bit
#define Elf_Phdr Elf64_Phdr
#define Elf_Dyn	 Elf64_Dyn
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Shdr Elf64_Shdr
#define Elf_Addr Elf64_Addr
#define Elf_Off	 Elf64_Off
#define Elf_Sym	 Elf64_Sym
typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
#else
#define Elf_Phdr Elf32_Phdr
#define Elf_Dyn	 Elf32_Dyn
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Shdr Elf32_Shdr
#define Elf_Addr Elf32_Addr
#define Elf_Off	 Elf32_Off
#define Elf_Sym	 Elf32_Sym
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;
#endif

// The file header is located at the beginning of the file, and is used to locate the other parts of the file.
typedef struct
{
	uint8_t	 e_ident[EI_NIDENT];	// ELF identification
	uint16_t e_type;				// Object file type
	uint16_t e_machine;				// Machine type
	uint32_t e_version;				// Object file version
	Elf_Addr e_entry;				// Entry point address
	Elf_Off	 e_phoff;				// Program header offset
	Elf_Off	 e_shoff;				// Section header offset
	uint32_t e_flags;				// Processor-specific flags
	uint16_t e_ehsize;				// ELF header size
	uint16_t e_phentsize;			// Size of program header entry
	uint16_t e_phnum;				// Number of program header entries
	uint16_t e_shentsize;			// Size of section header entry
	uint16_t e_shnum;				// Number of section header entries
	uint16_t e_shstrndx;			// Section name string table index
} ATTR(packed) Elf_Hdr;

// Program header. Field structure is different between bit sizes.
#ifdef CONFIG_64bit
typedef struct
{
	uint32_t   p_type;
	uint32_t   p_flags;
	Elf64_Off  p_offset;
	Elf64_Addr p_vaddr;
	Elf64_Addr p_paddr;
	uint64_t   p_filesz;
	uint64_t   p_memsz;
	uint64_t   p_align;
} ATTR(packed) Elf64_Phdr;
#else
typedef struct
{
	uint32_t   p_type;
	Elf32_Off  p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	uint32_t   p_filesz;
	uint32_t   p_memsz;
	uint32_t   p_flags;
	uint32_t   p_align;
} ATTR(packed) Elf32_Phdr;
#endif

// Section header
#ifdef CONFIG_64bit
typedef struct
{
	uint32_t   sh_name;			// Section name
	uint32_t   sh_type;			// Section type
	uint64_t   sh_flags;		// Section attributes
	Elf64_Addr sh_addr;			// Virtual address in memory
	Elf64_Off  sh_offset;		// Offset in file
	uint64_t   sh_size;			// Size of section
	uint32_t   sh_link;			// Link to other section
	uint32_t   sh_info;			// Miscellaneous information
	uint64_t   sh_addralign;	// Address alignment boundary
	uint64_t   sh_entsize;		// Size of entries, if section has table
} ATTR(packed) Elf64_Shdr;
#else
typedef struct
{
	uint32_t   sh_name;			// Section name
	uint32_t   sh_type;			// Section type
	uint32_t   sh_flags;		// Section attributes
	Elf32_Addr sh_addr;			// Virtual address in memory
	Elf32_Off  sh_offset;		// Offset in file
	uint32_t   sh_size;			// Size of section
	uint32_t   sh_link;			// Link to other section
	uint32_t   sh_info;			// Miscellaneous information
	uint32_t   sh_addralign;	// Address alignment boundary
	uint32_t   sh_entsize;		// Size of entries, if section has table
} ATTR(packed) Elf32_Shdr;
#endif

// Symbol
#ifdef CONFIG_64bit
typedef struct
{
	uint32_t   st_name;
	uint8_t	   st_info;
	uint8_t	   st_other;
	uint16_t   st_shndx;
	Elf64_Addr st_value;
	uint32_t   st_size;
} ATTR(packed) Elf64_Sym;
#else
typedef struct
{
	uint32_t   st_name;
	Elf32_Addr st_value;
	uint32_t   st_size;
	uint8_t	   st_info;
	uint8_t	   st_other;
	uint16_t   st_shndx;
} ATTR(packed) Elf32_Sym;
#endif

#ifdef CONFIG_64bit
typedef struct
{
	Elf64_Addr r_offset;
	uint64_t   r_info;
} ATTR(packed) Elf64_Rel;
typedef struct
{
	Elf64_Addr r_offset;
	uint64_t   r_info;
	int64_t	   r_addend;
} ATTR(packed) Elf64_Rela;
#else
typedef struct
{
	Elf32_Addr r_offset;
	uint32_t   r_info;
} ATTR(packed) Elf32_Rel;
typedef struct
{
	Elf32_Addr r_offset;
	uint32_t   r_info;
	int32_t	   r_addend;
} ATTR(packed) Elf32_Rela;
#endif
