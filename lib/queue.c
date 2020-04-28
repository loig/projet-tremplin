/**
 *
 * \file queue.c
 * \brief library file.
 * \author Albert.G, Simon.P, Alexis.G
 * \version 1.0
 * \date 17 fevrier 2020
 *
 * library containing methods for file.
 */

#include "queue.h"
#include <stdlib.h>
#include "maze.h"

/**
* \brief use to initialize an empty queue.
* \return a data type queue
*/
SetQueue* init_setqueue(){
  SetQueue* queue = (SetQueue*) malloc(sizeof(SetQueue));
  queue->head = NULL;
  queue->size = 0;
  return queue;
}

/**
* \brief use to add a coordinate to the current queue.
* \param queue the queue we want to edit
* \param coor the coordiante we want to add
*/
void setqueue_add_coor(SetQueue* queue, Coordonnes coor){
  if(queue->head == NULL){
    queue->head = (Cellule*) malloc(sizeof(Cellule));
    queue->head->coordinate = coor;
    queue->head->next = NULL;
    queue->size++;
  }else{
    Cellule* cell = queue->head;
    char is_in_the_queue = 0;

    while (cell != NULL)
      {
        if (cell->coordinate.x == coor.x && cell->coordinate.y == coor.y){
          is_in_the_queue = 1;
        }
        cell = cell->next;
      }
    cell = queue->head;
    if (!is_in_the_queue){
      while (cell->next != NULL){
        cell = cell->next;
      }
      cell->next = malloc(sizeof(Cellule*));
      cell->next->coordinate = coor;
      cell->next->next = NULL;
      queue->size++;
    }
  }
}

/**
* \brief use to remove a coordinate to the current queue, 
*   in a queue it's always the first coordinate we added
* \param queue the queue we want to edit
* \return the coordinate we just remove
*/
Coordonnes setqueue_pop(SetQueue* queue){
  Cellule* cell_to_up = queue->head->next;
  Cellule* cell_to_pop = queue->head;
  queue->head = cell_to_up;
  queue->size--;
  Coordonnes to_return = cell_to_pop->coordinate;
  free(cell_to_pop);
  return to_return;
}

/**
* \brief use to add a list of coordinate
* \param queue the queue we want to edit
* \param array a list of Coordonnes
* \param size size of the list we want to add to the queue
*/
void setqueue_add_array(SetQueue* queue, Coordonnes array[], int size){
  for (int i = 0; i < size; i++)
    {
      setqueue_add_coor(queue, array[i]);
    }
}

/**
* \brief use to know if the current queue is empty or not.
* \param queue the current queue that we want to know if it's empty
* \return 1 if the queue is empty
          0 if the queue is not empty
*/
int setqueue_is_empty(SetQueue* queue){
  return (queue->head == NULL ? 1 : 0);
}

/**
* \brief use to if a specific coordinate is in the current queue
* \param queue the current queue 
* \param coor a specific coordinate
* \return 1 if the coordinate is in the queue
          0 if the coordinate is not in the queue
*/
int setqueue_contains(SetQueue* queue, Coordonnes coor){
  if (setqueue_is_empty(queue))
    return 0;
  Cellule* cell = queue->head;
  while (cell != NULL){
    if (cell->coordinate.x == coor.x && cell->coordinate.y == coor.y){
      return 1;
    }
    cell = cell->next;
  }
  return 0;
}

/**
 * \brief use to free up space when we don't need the queue anymore 
 * \param queue the queue we want to delete 
*/
void setqueue_free(SetQueue* queue){
  if(queue->head == NULL){
    free(queue);
  }else{
    while (queue->head != NULL){
      setqueue_pop(queue);
    }
    free(queue);
  }
}

/**
 * \brief 
 * \param 
*/
bool equal(Coordonnes c1,Coordonnes c2){
  if(c1.x == c2.x && c1.y == c2.y){
    return true;
  }
  return false;
}
