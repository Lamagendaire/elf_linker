#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"
#include "display.h"
#include "readelf.h"
#include "fusionelf.h"



int main(int argc, char **argv) 
{ 
  // Déclaration des structures à remplir dans les fonctions de readelf.c
  ElfFile elffile;
  ElfFile elffile2;
  ElfFile elffusion;

  if(argc != 3 && argc != 2  ) {
    printf("usage: %s <ELF_FILE>\n", argv[0]);
    exit(1);
  }

//si argument 1 est pas un fichier-> retourne erreur
  if((elffile.fichierElf = fopen(argv[1], "r")) == NULL)
  {
    perror("[E] Error opening file ");
    exit(1);
  }

// si le deuxième argumgument est vide retourne une erreur
  if(argv[2]==NULL)
  {
    printf("Erreur: Il manque un paramètre:\n -display pour accéder aux fonctions d'affichage\n 'fichier'.o pour fusionner\n");
    exit(1);
  }
//si argument 2 est pas un fichier -> vérifier si option -display
  else if((elffile2.fichierElf = fopen(argv[2], "r")) == NULL)
  {
      if(strcmp(argv[2],"-display")==0)
      {
          elffile = ElfConstructor(fopen(argv[1],"r"));
          displayMenu(elffile);
          //printf("afficher");
      }
      else
      {
        printf("Option ou deuxième argument non valide\n");
      }
  }


//sinon (c'est que c'est un fichier) -> fusion des deux fichiers
  else
  {
  // On remplit les 2 structures à partir des fonctions dans readelf
  elffile = ElfConstructor(fopen(argv[1], "r"));
  elffile2 = ElfConstructor(fopen(argv[2],"r"));

  // On realise la fusion des sections progbits et on remplit elffusion
    printf("Fusion des 2 fichiers\n");
    elffusion=fusionProgBit(elffile,elffile2);

    

  }
}