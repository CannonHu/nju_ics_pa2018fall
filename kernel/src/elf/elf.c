#include "common.h"
#include "memory.h"
#include "string.h"

#include <elf.h>

#ifdef HAS_DEVICE_IDE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {	
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph, *eph;

#ifdef HAS_DEVICE_IDE
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
	elf = (void*)buf;
	Log("ELF loading from hard disk.");
#else
	elf = (void *)0x0;
	Log("ELF loading from ram disk.");
#endif

/* Load each program segment */
	ph = (void *)elf + elf->e_phoff;
	eph = ph + elf->e_phnum;
	for(; ph < eph; ph ++) {
		if(ph->p_type == PT_LOAD) {
			uint32_t paddr = mm_malloc(ph->p_vaddr, ph->p_memsz);
			Log("mm_malloc_addr: %x vaddr: %x\n", paddr, ph->p_vaddr);		
			
			//Log("filesize: %d", ph->p_filesz);
			//BREAK_POINT
#ifdef HAS_DEVICE_IDE
			ide_read((void*)paddr, ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);
			/*for(int i = 0; i < ph->p_filesz - 1; i++){
				if(ph->p_filesz < 5000 && i == 2342){
				Log("i: %d",i);
				BREAK_POINT
				}
				uint8_t tmp = 0;
				ide_read(&tmp, ELF_OFFSET_IN_DISK + ph->p_offset + i, 1);
				char* ptr = (void*)(paddr + i);
				*ptr = tmp;
			}*/
		#else
			*(uint32_t*)paddr = 0;
			memcpy((void*)paddr, (void*)elf + ph->p_offset, ph->p_filesz);	
#endif
			/* TODO: copy the segment from the ELF file to its proper memory area */
			memset((void*)(paddr + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz);		
			/* TODO: zeror the memory area [vaddr + file_sz, vaddr + mem_sz) */

#ifdef IA32_PAGE
			/*BREAK_POINT
			char* t = (void*)ph->p_vaddr;
			Log("test page: %x", *t);*/
			/* Record the program break for future use */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);
#ifdef HAS_DEVICE_VGA
	create_video_mapping();
#endif
	write_cr3(get_ucr3());
#endif
	//BREAK_POINT
	return entry;
}
