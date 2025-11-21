#ifndef NODE_H
#define NODE_H
template <typename T>

struct Node {
    T data;
    T priority=0;   
    Node* next;
};

#endif

