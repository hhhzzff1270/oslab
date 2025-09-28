#ifndef _PMM_H
#define _PMM_H

#include "types.h"

void pmm_init(void);
void* alloc_page(void);
void free_page(void* page);
uint64 get_free_page_count(void);

#endif