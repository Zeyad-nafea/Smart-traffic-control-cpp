#ifndef QUEUE_H
#define QUEUE_H
#include "Node.h"
template <class T>

class Queue {
private:
    Node<T>* Front;
    Node<T>* Rear;
public:
    Node<T>* Getfront(); 
    Node<T>* GetRear();
    Queue();
    bool isEmpty();
    bool enqueue(T value ,T p);
    bool dequeue();
    <
    ~Queue();

    
};
template <class T>
Queue<T>::Queue() {
    Front = nullptr;
    Rear = nullptr;
}
template <class T>
Node<T>* Queue<T>::Getfront() {
    return Front;
}

template <class T>
Node<T>* Queue<T>::GetRear() {
    return Rear;
}

template <class T>
bool Queue<T>::isEmpty()
{
    if(Getfront() == NULL && GetRear() == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}
template <class T>
bool Queue<T>::enqueue(T value, T p) {
    Node<T>* newNode = new Node<T>;
    newNode->data = value;
    newNode->priority = p;
    newNode->next = NULL;
    if(isEmpty())
    {
        Front = Rear = newNode;
        return true;
    }
    else if(p>Front->priority)
    {
        newNode->next = Front;
        Front = newNode;
        return true;
    }
    else
    {
        Node <T>* temp = Front;
        while(temp->next!=NULL && temp->next->priority>=p)
        {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
        if(newNode->next==NULL)
        {
            Rear = newNode;
        }
        return true;
    }

}
template <class T>
bool Queue<T>::dequeue()
{
    if(isEmpty())
    {
        return false;
    }
    else if(Front==Rear)
    {
        delete Front;
        Front = Rear = NULL;
        return true;
    }
    else
    {
        Node<T>* delptr = Front;
        Front = Front->next;
        delete delptr;
        return true;
    }
    
}
template <class T>
Queue<T>::~Queue()
{
    while(Front!=NULL)
    {
        dequeue();
    }
}

#endif // QUEUE_H