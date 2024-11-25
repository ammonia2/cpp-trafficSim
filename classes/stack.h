#pragma once

#include <iostream>
using namespace std;

template <typename T>

class Stack {

private:

    struct Node {
        T data;
        Node* next;

        Node(T value) : data(value), next(nullptr) {}
    };

    Node* top;
    int size;

public:
    
    Stack() : top(nullptr), size(0) {}

    ~Stack() {

        while (!isEmpty()) {

            pop();

        }

    }

    void push(T value) {

        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
        size++;

    }

    void pop() {

        if (isEmpty()) {

            cout << "Stack underflow!" << endl;
            return;
            
        }

        Node* temp = top;
        top = top->next;
        delete temp;
        size--;

    }

    T peek() {

        if (isEmpty()) {

            cout << "The stack is empty, nothing to return!" << endl;
            return;

        }

        return top->data;
    }

    bool isEmpty() {

        return top == nullptr;

    }

    int getSize() {

        return size;

    }

};

