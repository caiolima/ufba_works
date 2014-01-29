#include <stdio.h>
#include <string.h>
#include "HashedFile.h"

int hash(int nivel, int id){

}

HashedFile::HashedFile(char *name) {
  this->file_name = name;
  this->init();
}

void HashedFile::init() {
  FILE *f = fopen(this->file_name, "r");
  if(f){
    //File exists, then reade the header
    f_header h;

    fread(&h, sizeof(f_header), 1, f);
    this->header = h;

    fclose(f);
    return;
  }
  fclose(f);

  f = fopen(this->file_name, "w");

  //write the header of file with some informations
  f_header h;
  h.level = 0;
  h.next = 0;

  fwrite(&h, sizeof(f_header), 1, f);

  //start to write empty pages to the file
  page pages[N];
  for (int i = 0; i < N; i++) {
    for(int j = 0; j < REGISTROS_POR_PAGINA; j++){
      pages[i].data[j].id = EMPTY;
      strcpy(pages[i].data[j].name, "");
      pages[i].data[j].age = 0;
    }
  }

  fwrite(pages, sizeof(page), N, f);

}
