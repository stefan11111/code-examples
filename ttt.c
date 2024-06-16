free_files(void **ptr, char type) { if(type == 0) { int *p = *ptr; while(*p){ close(*p); p++; } } else { while(ptr && *ptr) { fclose(*ptr); ptr++;} } }
