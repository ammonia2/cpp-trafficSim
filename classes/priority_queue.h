#pragma once

#include <iostream>
#include <type_traits>
using namespace std;

class Vehicle;


template <typename T>
class PriorityQueue {

private:

    T* heap;       
    int capacity;  
    int size;      
    bool isMinHeap;

    bool compare(T a, T b) const {
        if constexpr (std::is_base_of<Vehicle, std::remove_pointer_t<T>>::value) {
            return isMinHeap ? (*a < *b) : (*a > *b);
        } 
        else {
            return isMinHeap ? (a < b) : (a > b);
        }
    }


    void heapifyUp(int index) {

        while (index > 0) {

            int parent = (index - 1) / 2;

            if (compare(heap[index], heap[parent])) {

                swap(heap[index], heap[parent]);
                index = parent;

            } 
            
            else {

                break;

            }

        }

    }

    void heapifyDown(int index) {

        while (true) {

            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int selectedChild = index;

            if (leftChild < size && compare(heap[leftChild], heap[selectedChild])) {

                selectedChild = leftChild;

            }

            if (rightChild < size && compare(heap[rightChild], heap[selectedChild])) {

                selectedChild = rightChild;

            }

            if (selectedChild == index) {

                break;

            }

            swap(heap[index], heap[selectedChild]);
            index = selectedChild;

        }

    }

    void resize() {
        int newCapacity = capacity * 2;
        T* newHeap = new T[newCapacity];
        
        for (int i = 0; i < size; i++) {
            if constexpr (std::is_pointer<T>::value) {
                // For pointer types, create new objects if needed
                newHeap[i] = heap[i]; // Assumes ownership management is handled elsewhere
            } else {
                newHeap[i] = heap[i];
            }
        }
        
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

public:
    PriorityQueue(const PriorityQueue& other) {
        capacity = other.capacity;
        size = other.size;
        isMinHeap = other.isMinHeap;
        heap = new T[capacity];
        
        for (int i = 0; i < size; i++) {
            if constexpr (std::is_pointer<T>::value) {
                // Deep copy if needed
                heap[i] = other.heap[i]; // Assumes ownership management is handled elsewhere
            } else {
                heap[i] = other.heap[i];
            }
        }
    }

    PriorityQueue& operator=(const PriorityQueue& other) {
        if (this != &other) {
            delete[] heap;
            capacity = other.capacity;
            size = other.size;
            isMinHeap = other.isMinHeap;
            heap = new T[capacity];
            
            for (int i = 0; i < size; i++) {
                if constexpr (std::is_pointer<T>::value) {
                    heap[i] = other.heap[i]; // Assumes ownership management is handled elsewhere
                } else {
                    heap[i] = other.heap[i];
                }
            }
        }
        return *this;
    }

    PriorityQueue(bool minHeap = true, int initialCapacity = 10): isMinHeap(minHeap), capacity(initialCapacity), size(0) {

        heap = new T[capacity];

    }

    ~PriorityQueue() {

        delete[] heap;

    }

    void push(T value) {

        if (size == capacity) {

            resize();

        }

        heap[size] = value;
        heapifyUp(size);
        size++;

    }

    void deleteValue(T value) {
        int index = -1;
        for (int i = 0; i < size; i++) {
            if constexpr (std::is_pointer<T>::value) {
                if (*heap[i] == *value) {
                    index = i;
                    break;
                }
            } else {
                if (heap[i] == value) {
                    index = i;
                    break;
                }
            }
        }
        
        if (index == -1) return;
        
        heap[index] = heap[size - 1];
        size--;
        
        if (index < size) {
            heapifyDown(index);
            heapifyUp(index);
        }
    }

    void pop() {

        if (isEmpty()) {

            cout << "Priority queue is empty!" << endl;
            return;

        }

        heap[0] = heap[size - 1];
        size--;

        if (size > 0) {

            heapifyDown(0);

        }

    }

    T top() {

        if (isEmpty()) {

            cout << "Priority queue is empty!" << endl;
            return T(); 

        }

        return heap[0];

    }

    bool isEmpty() const {

        return size == 0;

    }

    int getSize() {

        return size;

    }

    void display() const {

        if (isEmpty()) {

            cout << "Priority queue is empty!" << endl;
            return;

        }

        for (int i = 0; i < size; i++) {

            cout << heap[i] << " ";

        }

        cout << endl;

    }
    
};