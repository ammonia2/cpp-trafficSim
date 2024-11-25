#pragma once

#include <iostream>
using namespace std;
template <typename T>

class Queue {

private:

    struct Node {

        T data;
        Node* next;

        Node(T value) : data(value), next(nullptr) {}

    };

    Node* front; 
    Node* rear;  
    int size;   

public:

    Queue() : front(nullptr), rear(nullptr), size(0) {}

    ~Queue() {

        while (!isEmpty()) {

            dequeue();

        }

    }

    void enqueue(T value) {

        Node* newNode = new Node(value);

        if (isEmpty()) {

            front = rear = newNode;

        } 
        
        else {

            rear->next = newNode;
            rear = newNode;

        }

        size++;

    }

    void dequeue() {

        if (isEmpty()) {

            cout << "Queue underflow!" << endl;
            return;

        }

        Node* temp = front;
        front = front->next;

        if (front == nullptr) { 

            rear = nullptr;

        }

        delete temp;
        size--;

    }

    T peek() {

        if (isEmpty()) {

            cout << "The queue is empty, nothing to return!" << endl;
            return;

        }

        return front->data;

    }

    bool isEmpty() {

        return front == nullptr;

    }

    int getSize() {

        return size;

    }

};

