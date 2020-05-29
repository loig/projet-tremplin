/**
 *
 * \file stack.c
 * \brief library stack.
 * \author Albert.G, Simon.P, Alexis.G
 * \version 1.0
 * \date 17 fevrier 2020
 *
 * library containing methods to manage the stack.
 */

#include "stack.h"
#include <stdlib.h>
#include <string.h>

/**
 * \brief use to add a list to the current stack
 * \param list the current stack we want tu edit
 * \param str the string we want to add to the stack
 * \return 1
*/
int addList(List* list, char* str){
  if (list->next == NULL){
    list->next = (cellule*) malloc(sizeof(cellule));
    list->next->next = NULL;
    int size = 0;
    for (; str[size] != '\0'; size++);
    list->next->txt = (char*) malloc(sizeof(char) * size);
    for (int i = 0; i < size; i++){
      list->next->txt[i] = str[i];
    }
    list->size++;
    return 1;
  }else{
    cellule* c = list->next;
    while (c->next != NULL){
      c = c->next;
    }
    c->next = (cellule*) malloc(sizeof(cellule));
    c = c->next;
    c->next = NULL;
    int size = 0;
    for (; str[size] != '\0'; size++);
    c->txt = (char*) malloc(sizeof(char) * size);
    for (int i = 0; i < size; i++){
      c->txt[i] = str[i];
    }
    list->size++;
    return 1;
  }
}

/**
 * \brief use to add a string at the head of the stack
 * \param list the current stack we want tu edit
 * \param str the string we want to add to the stack
 * \return 1
*/
int addHeadList(List* list, char* str){
  cellule* next = list->next;
  list->next = (cellule*) malloc(sizeof(cellule));
  int size = 0;
  for (; str[size] != '\0'; size++);

  list->next->txt = (char*) malloc(sizeof(char) * size);
  memset(list->next->txt, 0, sizeof(char) * size);
  for (int i = 0; i < size; i++){
    list->next->txt[i] = str[i];
  }
  list->next->next = next;
  list->size++;
  return 1;
}

/**
 * \brief use to return a string of the stack
 * \param index to know where we want to stop
 * \return a string begin at the head an finish to the index
*/
char* getListChar(List* list, int index){
  cellule *c = list->next;
  int i = 0;
  while (c != NULL && i < index) {
    c = c->next;
    i++;
  }
  if (c != NULL) {
    return c->txt;
  }
  return NULL;
}

/**
 * \brief use to delete the head of a current stack
 * \param list the current list we want to edit
*/
void deleteHead(List *list){
  if (list->size > 1){
    //cellule* c = (cellule*) malloc(sizeof(cellule));
    cellule* c = list->next->next;
    list->next = c;
    list->size--;
  }
}
