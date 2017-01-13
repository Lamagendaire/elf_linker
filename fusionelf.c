#include "fusionelf.h"

ElfFile fusionProgBit(ElfFile elf1, ElfFile elf2, ElfFile elf3) {
    //pour tous les sections de elf2
    for (int i = 0; i < elf2.headerElf.e_shnum; i++) {
        //si la section est == PROGBITS
        if (elf2.tableSections[i].headerSec.sh_type == SHT_PROGBITS) {
            //on parcours toutes les sections de elf1
            for (int j = 0; j < elf1.headerElf.e_shnum; j++) {
                //si les sections sont les mêmes -> concaténation des 2
                if (strcmp(elf1.tableSections[j].nomSec, elf2.tableSections[i].nomSec) == 0) {
                    //on agrandit le contenu de la section existante+celle qu'on doit ajouter
                    elf3.tableSections[j].contenuSec = realloc(elf3.tableSections[j].contenuSec, elf3.tableSections[j].headerSec.sh_size + elf2.tableSections[i].headerSec.sh_size);

                    //si le contenu est vide
                    if (elf3.tableSections[j].contenuSec != NULL) {
                        for (int p = 0; p < elf2.tableSections[i].headerSec.sh_size; p++) { //fusion des sections
                            elf3.tableSections[j].contenuSec[elf3.tableSections[j].headerSec.sh_size + p] = elf2.tableSections[i].contenuSec[p];
                        }
                        //modification de la taille
                        elf3.tableSections[j].headerSec.sh_size += elf2.tableSections[i].headerSec.sh_size;
                    }
                }

            }
        }
    }

    int not_exist;
    //pour toutes les sections de elf2
    for (int i = 0; i < elf2.headerElf.e_shnum; i++) {
        //si le type de la section == PROGBITS
        if (elf2.tableSections[i].headerSec.sh_type == SHT_PROGBITS) {
            not_exist = 0;
            //pour toutes les sections de elf1
            for (int j = 0; j < elf1.headerElf.e_shnum; j++) {
                //si les sections sont de type PROGBITS (dans elf3) et que la section dans elf2 est la même que dans elf3
                if (elf3.tableSections[j].headerSec.sh_type == SHT_PROGBITS && strcmp(elf2.tableSections[i].nomSec, elf3.tableSections[j].nomSec) == 0) { //c'est que la section existe déjà, donc inutile de la copier dans elf3
                    not_exist = 1;
                }
            }
            //si la section est inexistante dans elf3
            if (not_exist == 0) {
                //on agrandit le tableau des sections de elf3 avec une ligne en plus
                elf3.tableSections = realloc(elf3.tableSections, (elf3.headerElf.e_shnum + 1) * sizeof (Section));
                //on alloue une taille suffisante pour le contenu de la section dans l'emplacement ajouté
                //et on copie le contenu de la section de elf2 dans elf3 à cet emplacement, et ce que la taille du contenu de la section elf2
                elf3.tableSections[elf3.headerElf.e_shnum].contenuSec = malloc(elf2.tableSections[i].headerSec.sh_size);
                memcpy(elf3.tableSections[elf3.headerElf.e_shnum].contenuSec, elf2.tableSections[i].contenuSec, elf2.tableSections[i].headerSec.sh_size);
                //on alloue une taille de la taille du nom dans la section elf3, à l'emplacement ajouté
                //et on copie ce nom
                elf3.tableSections[elf3.headerElf.e_shnum].nomSec = malloc(strlen(elf2.tableSections[i].nomSec));
                memcpy(elf3.tableSections[elf3.headerElf.e_shnum].nomSec, elf2.tableSections[i].nomSec, strlen(elf2.tableSections[i].nomSec));

                // on copie le header de la section ajoutée
                elf3.tableSections[elf3.headerElf.e_shnum].headerSec = elf2.tableSections[i].headerSec;
                // on met à jour le nombre de sections (pour affichage)
                elf3.headerElf.e_shnum++;
            }
        }
    }

    return elf3;
}