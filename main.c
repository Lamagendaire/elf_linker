#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"
// #include "display.h"

int main(int argc, char **argv) 
{
	FILE* ElfFile = NULL;
	FILE* ElfFile2 = NULL;	

  if(argc != 3 && argc != 2  ) {
    printf("usage: %s <ELF_FILE>\n", argv[0]);
    exit(1);
  }

  if((ElfFile = fopen(argv[1], "r")) == NULL) {
    perror("[E] Error opening file:");
    exit(1);
  }

  if((ElfFile2 = fopen(argv[2], "r")) == NULL) {
  		if(strcmp(argv[2],"-display")==0)
  		{  			
  				displayMenu(ElfFile);
  				//printf("Afficher");
  		}
  	}

  else{
  	printf("Fusion des 2 fichiers");
  }
}