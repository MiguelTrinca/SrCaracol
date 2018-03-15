#ifndef LIB_H
#define LIB_H

#include <stdio.h>

typedef struct vertex{
    double d;
    double low;
    int id;
} *Vertex;

typedef struct node{
    Vertex v;
    struct node *next;
} Link;

typedef struct int_node{
    int id;
    struct int_node *next;
} Stack;


#endif