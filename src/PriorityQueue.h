#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include "Node.h"

template <class T>
class PriorityQueue {
private:
    Node<T>* Front;
    Node<T>* Rear;
    int count;
public:
    PriorityQueue();
    ~PriorityQueue();
    bool isEmpty() const;       
    int Size() const;           
    int size() const;            
    bool enqueue(T value, int priority);
    T dequeue();
    T peek() const;           
    int peekPriority() const;  
    bool delitem(T item);        
    Node<T>* getFront() const { return Front; }
    Node<T>* getRear() const { return Rear; }
};

template <class T>
PriorityQueue<T>::PriorityQueue()
{
    Front = nullptr;
    Rear = nullptr;
    count = 0;  
}

template<class T>
PriorityQueue<T>::~PriorityQueue()
{
    while (!isEmpty()) {
        dequeue();
    }
}

template <class T>
bool PriorityQueue<T>::isEmpty() const  
{
    return Front == nullptr;
}

template <class T>
int PriorityQueue<T>::Size() const  
{
    return count;
}

template <class T>
int PriorityQueue<T>::size() const 
{
    return count;
}

template <class T>
bool PriorityQueue<T>::enqueue(T value, int priority)
{
    Node<T>* newNode = new Node<T>;
    newNode->data = value;
    newNode->priority = priority;
    newNode->next = nullptr;


    if (isEmpty()) {
        Front = Rear = newNode;
        count++;
        return true;
    }

    if (priority > Front->priority) {
        newNode->next = Front;
        Front = newNode;
        count++;
        return true;
    }

 
    Node<T>* temp = Front;


    while (temp->next != nullptr && temp->next->priority >= priority) {
        temp = temp->next;
    }


    newNode->next = temp->next;
    temp->next = newNode;


    if (newNode->next == nullptr) {
        Rear = newNode;
    }

    count++;
    return true;
}

template <class T>
T PriorityQueue<T>::dequeue()
{
    if (isEmpty()) {
        return nullptr;
    }

    Node<T>* temp = Front;
    T data = temp->data;
    Front = Front->next;

    if (Front == nullptr) {
        Rear = nullptr;
    }

    delete temp;
    count--;
    return data;
}

template <class T>
T PriorityQueue<T>::peek() const  
{
    if (isEmpty()) {
        return nullptr;
    }
    return Front->data;
}

template <class T>
int PriorityQueue<T>::peekPriority() const  
{
    if (isEmpty()) {
        return -1;
    }
    return Front->priority;
}

template <class T>
bool PriorityQueue<T>::delitem(T item)
{
    if (isEmpty()) {
        return false;
    }

    if (Front->data == item) {
        Node<T>* temp = Front;
        Front = Front->next;
        if (Front == nullptr) {
            Rear = nullptr;
        }
        delete temp;
        count--;
        return true;
    }

    Node<T>* prev = Front;
    Node<T>* curr = Front->next;

    while (curr != nullptr) {
        if (curr->data == item) {
            prev->next = curr->next;
            if (curr == Rear) {
                Rear = prev;
            }
            delete curr;
            count--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    return false;
}

#endif 