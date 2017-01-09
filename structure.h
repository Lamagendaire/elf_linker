#include <stdio.h>
#include <stdlib.h>
#include <elf.h>


typedef struct 
{//Section complète (nom, header et contenu)
        char * nomSec;
        Elf32_Shdr  headerSec;
        char * contenuSec;
} Section;


typedef struct 
{//Fichier ELF (header, sections, symboles, reloactions)
        FILE * fichierElf;
        Elf32_Ehdr headerElf;
        Section * tabSections;
        int sizeSections;
        char * tableSymb;
        Elf32_Shdr * tabRelocation;

}ElfFile;



/*
typedef struct {
        ElfFile * ElfFile1;
        ElfFile * ElfFile2;
} LinkedFiles;
*/
