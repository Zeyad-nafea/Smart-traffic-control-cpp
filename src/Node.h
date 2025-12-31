#ifndef NODE_H
#define NODE_H

template <typename T>
struct Node {
    T data;
    int priority;
    Node* next;

    Node() : data(T()), priority(0), next(nullptr) {}
};

#endif 