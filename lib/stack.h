/**
 *
 * \file stack.h
 * \brief library stack.
 * \author Albert.G, Simon.P, Alexis.G
 * \version 1.0
 * \date 17 fevrier 2020
 *
 * library containing prototypes of methods to manage the stack.
 */

#ifndef _stack_
#define _stack_

/**
 * \struct cell_string
 * \brief a data type of a cell with a string, use to display the maze
 * txt : a pointer to a string 
 * next : a pointer to the next cell
*/
typedef struct cell_string{
  char *txt;
  struct cell_string *next;
}cellule;

/**
 * \struct stack
 * \brief a data type of a stack
 * next : a pointer to the next cell 
 * size : number of a cell in a stack
*/
typedef struct stack{
  cellule* next;
  int size;
}List;

/**
 * \brief use to delete the head of a current stack 
 * \param list the current list we want to edit
*/
void deleteHead(List*);

/**
 * \brief use to return a string of the stack
 * \param index to know where we want to stop
 * \return a string begin at the head an finish to the index
*/
char* getListChar(List*, int);

/**
 * \brief use to add a string at the head of the stack 
 * \param list the current stack we want tu edit
 * \param str the string we want to add to the stack
 * \return 1
*/
int addHeadList(List*, char*);

/**
 * \brief use to add a list to the current stack 
 * \param list the current stack we want tu edit
 * \param str the string we want to add to the stack
 * \return 1
*/
int addList(List*, char*);

#endif
