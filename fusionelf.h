#ifndef FUSIONELF_H
#define FUSIONELF_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"
#include "readelf.h"

ElfFile fusionProgBit (ElfFile elf1, ElfFile elf2, ElfFile elf3);


#endif
