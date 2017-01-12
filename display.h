#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"
#include "readelf.h"


void displaySection(ElfFile elf1);
void displaySectionTable(ElfFile elf1);
void displayHeaderInfos(ElfFile elf1);
void displaySymbolTable(ElfFile elf1);
void displayRelocatableTable(ElfFile elf1);
void displayMenu(FILE* ElfFile);

#endif
