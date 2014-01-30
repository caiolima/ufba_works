#include <stdio.h>
#include <string.h>
#include <list>
#include "HashedFile.h"

int hash(int level, int key){
  return key % (N*(1 << level));
}

float HashedFile::spaceUtilization() {
  return this->header.used_space / this->header.total_space;
}

int HashedFile::getPageHash(int key) {
  int page_n = hash(this->header.level, key);
  
  if (page_n < this->header.next){
    page_n = hash(this->header.level + 1, key);
  }

  return page_n;
}

page HashedFile::readPage(int n) {
  FILE *f = fopen(this->file_name, "r");
  fseek(f, sizeof(f_header) + sizeof(page)*n, SEEK_SET);

  page aPage;
  fread(&aPage, sizeof(page), 1, f);

  fclose(f);
  return aPage;
}

void HashedFile::persistPage(page p, int n) {
  FILE *f = fopen(this->file_name, "r+");
  
  //persist data
  fseek(f, sizeof(f_header) + sizeof(page)*n, SEEK_SET);
  fwrite(&p, sizeof(page), 1, f);

  fclose(f);
}

void HashedFile::persistHeader() {
  FILE * f = fopen(this->file_name, "r+");
  //presist header
  fseek(f, 0, SEEK_SET);
  f_header h = this->header;
  fwrite(&h, sizeof(f_header), 1, f);

  fclose(f);
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
  h.used_space = 0;
  h.total_space = REGISTROS_POR_PAGINA * N;
  fwrite(&h, sizeof(f_header), 1, f);
  this->header = h;
  //start to write empty pages to the file
  page pages[N];
  for (int i = 0; i < N; i++) {
    pages[i].overflow_addr = EMPTY;
    for(int j = 0; j < REGISTROS_POR_PAGINA; j++){
      pages[i].data[j].id = EMPTY;
      strcpy(pages[i].data[j].name, "");
      pages[i].data[j].age = 0;
    }
  }

  fwrite(pages, sizeof(page), N, f);
  fclose(f);

}

void HashedFile::add(user aUser) {
  int page_n = this->getPageHash(aUser.id);

  page aPage = this->readPage(page_n);

  //find the empty space
  bool had_space = false;
  for(int i = 0; i < REGISTROS_POR_PAGINA; i++) {
    if(isEmpty(aPage.data[i])) {
      aPage.data[i] = aUser;

      had_space = true;
      break;
    }
  }

  //no space in the normal file. Go to the overflow pages
  if (!had_space) {
    //There is no overflow page
    if(aPage.overflow_addr == EMPTY){
      int overflow_addr = allocOverflowPage();
      overflow_page over_page = createEmptyOverflowPage();

      over_page.data[0] = aUser;

      persistOverflowPage(overflow_addr, over_page);
      this->header.total_space += OVERFLOW_N;

      aPage.overflow_addr = overflow_addr;
    } else {
      overflow_page over_page = getOverflowPage(aPage.overflow_addr);
      int saved_page = aPage.overflow_addr;
      while (!insertOn(&over_page, aUser)) {
        int next_page = over_page.next;
        if (next_page == -1){
          int new_page = allocOverflowPage();

          over_page.next = new_page;
          //save next pointer
          persistOverflowPage(saved_page, over_page);

          over_page = getOverflowPage(new_page);
          over_page.next = -1;
          over_page.avaiable = false;

          this->header.total_space += OVERFLOW_N;
          saved_page = new_page;
        } else {
          over_page = getOverflowPage(next_page);
          saved_page = next_page;
        }
      }
      persistOverflowPage(saved_page, over_page);
    }
  }

  //persist data
  this->persistPage(aPage, page_n);

  this->header.used_space++;
  
  this->persistHeader();
}

bool HashedFile::remove(int key) {
  int page_n = this->getPageHash(key);

  page aPage = this->readPage(page_n);

  bool found_record = false;
  for(int i=0; i < REGISTROS_POR_PAGINA; i++){
    if(aPage.data[i].id == key) {
      aPage.data[i].id = EMPTY;

      this->header.used_space--;
      this->persistPage(aPage, page_n);

      found_record = true;
    }
  }

  if (!found_record) {
    int overflow_addr = aPage.overflow_addr;
    if(overflow_addr != -1) {
      
      overflow_page parent_page;
      int parent_page_addr = -1;

      overflow_page curr_page = getOverflowPage(overflow_addr);
      int curr_page_n = overflow_addr;
      
      found_record = deleteRecordOn(&curr_page, key);
      
      while(!found_record){
        int aux = curr_page_n;
        curr_page_n = curr_page.next;
        if(curr_page_n == -1)
          break;

        parent_page = curr_page;
        parent_page_addr = aux;

        curr_page = getOverflowPage(curr_page_n);
        found_record = deleteRecordOn(&curr_page, key);
      }

      if (found_record) {
        this->header.used_space--;
        //delete page and change params
        if (isOverflowPageEmpty(curr_page)) {

          if (parent_page_addr == -1) {
            aPage.overflow_addr = curr_page.next;
            this->persistPage(aPage, page_n);
          } else {
            parent_page.next = curr_page.next;
            persistOverflowPage(parent_page_addr, parent_page);
          }

          curr_page.avaiable = true;
          curr_page.next = -1;

          this->header.total_space -= OVERFLOW_N;
        }
        persistOverflowPage(curr_page_n, curr_page);
      }
    }
  }

  this->persistHeader();
  return found_record;
}

user HashedFile::get(int key) {
  int page_n = getPageHash(key);

  page aPage = this->readPage(page_n);

  for(int i = 0; i < REGISTROS_POR_PAGINA; i++) {
    if(aPage.data[i].id == key) {
      return aPage.data[i];
    }
  }

  if (aPage.overflow_addr != EMPTY) {
    overflow_page curr_page = getOverflowPage(aPage.overflow_addr);
    int curr_page_num = aPage.overflow_addr;
    do {

      for (int i = 0; i < OVERFLOW_N; i++) {
        if(curr_page.data[i].id == key){
          return curr_page.data[i];
        }
      }

      curr_page_num = curr_page.next;
      if (curr_page_num != -1) {
        curr_page = getOverflowPage(curr_page.next);
      }
    } while(curr_page_num != -1);
  }

  user no_user;
  no_user.id = EMPTY;
  return no_user;
}

std::list<user> HashedFile::getAllDataFromPage(int page_n) {
  std::list<user> user_list;
  page aPage = this->readPage(page_n);
  
  for (int i = 0; i < REGISTROS_POR_PAGINA; i++) {
    if (!isEmpty(aPage.data[i])) {
      user_list.push_back(aPage.data[i]);
    }
  }

  int overflow_addr = aPage.overflow_addr;
  if (overflow_addr != -1) {
    int curr_overflow_addr = overflow_addr;
    do {
      overflow_page curr_page = getOverflowPage(curr_overflow_addr);

      for (int i=0; i < OVERFLOW_N; i++) {
        if (!isEmpty(curr_page.data[i])) {
          user_list.push_back(curr_page.data[i]);
        }
      }

      curr_overflow_addr = curr_page.next;
    }while(curr_overflow_addr != -1);
  }

  return user_list;
}
