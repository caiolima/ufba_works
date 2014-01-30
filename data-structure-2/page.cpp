#include <stdio.h>
#include <stdlib.h>
#include "page.h"

overflow_page createEmptyOverflowPage(){
  overflow_page p;
  for (int i = 0; i < OVERFLOW_N; i++) {
    p.data[i] = createEmptyUser();
  }
  p.next = -1;
  p.avaiable = false;

  return p;
}

overflow_page getOverflowPage(int pos){
  FILE *f = fopen("overflow.data", "r");
  fseek(f, sizeof(overflow_page)*pos, SEEK_SET);

  overflow_page p;
  fread(&p,sizeof(overflow_page), 1, f);

  fclose(f);

  return p;
}

void persistOverflowPage(int pos, overflow_page p){
  FILE *f = fopen("overflow.data", "r+");
  fseek(f, sizeof(overflow_page)*pos, SEEK_SET);

  fwrite(&p,sizeof(overflow_page), 1, f);

  fclose(f);
}

int allocOverflowPage(){
  FILE *f = fopen("overflow.data", "r+");
  if (f) {
    int free_pos = 0;
    overflow_page p;
    fread(&p, sizeof(overflow_page), 1, f);
    while (!p.avaiable) {
      
      free_pos++;
      
      if (fread(&p, sizeof(overflow_page), 1, f) == 0) {
        overflow_page p = createEmptyOverflowPage();

        fwrite(&p, sizeof(overflow_page), 1, f);

        break;
      }

    }

    fclose(f);
    return free_pos;
  }
  fclose(f);

  f = fopen("overflow.data", "w");
  overflow_page p = createEmptyOverflowPage();

  fwrite(&p, sizeof(overflow_page), 1, f);
  fclose(f);

  return 0;

}

bool insertOn(overflow_page *page, user u){
  for (int i = 0; i < OVERFLOW_N; i++) {
    if(isEmpty(page->data[i])){
      page->data[i] = u;

      return true;
    }
  }

  return false;
}

bool deleteRecordOn(overflow_page* oPage, int key) {
  for(int i = 0; i < OVERFLOW_N; i++){
    if(oPage->data[i].id == key){
      oPage->data[i].id = EMPTY;
      return true;
    }
  }

  return false;
}

bool isOverflowPageEmpty(overflow_page p){
  for(int i = 0; i < OVERFLOW_N; i++){
    if(!isEmpty(p.data[i]))
      return false;
  }

  return true;
}
