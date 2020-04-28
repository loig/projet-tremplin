/**
 *
 * \file queue.c
 * \brief main file witch can be excecute to run the program
 * \author Albert.G, Simon.P, Alexis.G
 * \version 1.0
 * \date 17 février 2020
 *
 */

#ifndef _queue_
#define _queue_
#include <stdbool.h>

/**
 * \struct Coordonnes
 * \brief describe data type of coordinate in the maze.
 * x : position x of a coordinate
 * y : position y of a coordinate
 */

typedef struct Coordonnes{
    int x;
    int y;
} Coordonnes;

/**
 * \struct Cellule
 * \brief describe data type of a cell of a maze.
 * next : it's also a data type cell witch point to the next cell.
 * coordinate : it's the coordinate of the current cell.
 */
typedef struct cell
{
    struct cell* next;
    Coordonnes coordinate;
}Cellule;

/**
 * \struct SetQueue
 * \brief describe data type of a queue
 * head : it's a pointer data type Cellule point to the head of the queue
 */
typedef struct queue
{
    Cellule* head;
    int size;
}SetQueue;

/**
* \brief use to initialize an empty queue.
* \return a data type queue
*/
SetQueue* init_setqueue();

/**
* \brief use to add a coordinate to the current queue.
* \param queue the queue we want to edit
* \param coor the coordiante we want to add
*/
void setqueue_add_coor(SetQueue*, Coordonnes);

/**
* \brief use to remove a coordinate to the current queue, 
*   in a queue it's always the first coordinate we added
* \param queue the queue we want to edit
* \return the coordinate we just remove
*/
Coordonnes setqueue_pop(SetQueue*);

/**
* \brief use to add a list of coordinate
* \param queue the queue we want to edit
* \param array a list of Coordonnes
* \param size size of the list we want to add to the queue
*/
void setqueue_add_array(SetQueue*, Coordonnes[], int);

/**
* \brief use to know if the current queue is empty or not.
* \param queue the current queue that we want to know if it's empty
* \return 1 if the queue is empty
          0 if the queue is not empty
*/
int setqueue_is_empty(SetQueue*);

/**
* \brief use to if a specific coordinate is in the current queue
* \param queue the current queue 
* \param coor a specific coordinate
* \return 1 if the coordinate is in the queue
          0 if the coordinate is not in the queue
*/
int setqueue_contains(SetQueue*, Coordonnes);

/**
 * \brief use to free up space when we don't need the queue anymore 
 * \param queue the queue we want to delete 
*/
void setqueue_free(SetQueue*);

/**
 * \brief 
 * \param 
*/
bool equal(Coordonnes,Coordonnes);

#endif
