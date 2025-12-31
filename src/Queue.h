#ifndef QUEUE_H
#define QUEUE_H
#include "Node.h"

template <class T>
class Queue {
private:
    Node<T>* Front;
    Node<T>* Rear;
    int count;
public:
    Queue();
    ~Queue();
    bool isEmpty() const;  
    int size() const;      
    bool enqueue(T value);
    T dequeue();
    T peek() const;       
    bool delitem(T item);
    Node<T>* getFront() const { return Front; }
    Node<T>* getRear() const { return Rear; }
};

template <class T>
Queue<T>::Queue()
{
    Front = nullptr;
    Rear = nullptr;
    count = 0;
}

template <class T>
Queue<T>::~Queue()
{
    while (!isEmpty()) {
        dequeue();
    }
}

template<class T>
bool Queue<T>::isEmpty() const  
{
    return Front == nullptr;  
}

template <class T>
int Queue<T>::size() const  
{
    return count;
}

template<class T>
bool Queue<T>::enqueue(T value)
{
    Node<T>* newNode = new Node<T>;
    newNode->data = value;
    newNode->priority = 0;
    newNode->next = nullptr;

    if (isEmpty())
    {
        Front = Rear = newNode;
    }
    else {
        Rear->next = newNode;
        Rear = newNode;
    }
    count++;
    return true;
}

template <class T>
T Queue<T>::dequeue()
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
T Queue<T>::peek() const  
{
    if (isEmpty()) {
        return nullptr;
    }
    return Front->data;
}

template <class T>
bool Queue<T>::delitem(T item)
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