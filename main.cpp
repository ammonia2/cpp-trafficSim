// command to run:
// g++ -o app main.cpp classes/intersection.cpp classes/vehicle.cpp && ./app


#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
// #include "classes/hash_table.h"
// #include "classes/list.h"
// #include "classes/priority_queue.h"
// #include "classes/queue.h"
// #include "classes/stack.h"
// #include "classes/adjacency_list.h"
// #include "classes/DynamicArray.h"
class Road;
class Vehicle;
class Intersection;
class TrafficSignal;

using namespace std;
using namespace std::chrono;
template <typename T>

class LinkedList {

public:

    struct Node {

        T data;
        Node* next;

        Node(T value) : data(value), next(nullptr) {}

    };
private:
    Node* head; 
    int size;


public:

    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() {

        while (size > 0) {

            removeAtStart();

        }

    }

    // to work only for Road* data type. Otherwise causes double free errors
    // (could be resolved by accessing as reference. meh 😪)
    template <typename U = T>
    typename std::enable_if<std::is_same<U, Road*>::value>::type
    printList() const {
        Node* current = head;
        if (!current) {
            std::cout << "List is empty." << std::endl;
            return;
        }

        while (current) {
            std::cout << current->data->getDest()->getName() << " -> ";
            current = current->next;
        }
        std::cout << "null" << std::endl;
    }

    Node* getHead() const {
        return head;
    }

    Node* getNode(int index) const {

        if (index < 0 || index >= size) {

            cout << "Index out of range!" << endl;
            return nullptr;

        }

        Node* current = head;

        for (int i = 0; i < index; i++) {

            current = current->next;

        }

        return current;

    }
    void insertAtStart(T value) {

        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        size++;

    }

    void insertAtEnd(T value) {

        Node* newNode = new Node(value);

        if (isEmpty()) {

            head = newNode;

        } 
        
        else {

            Node* tail = getNode(size - 1);

            if (tail) {

                tail->next = newNode;
 
            }

        }

        size++;

    }

    void insertAtIndex(int index, T value) {

        if (index < 0 || index > size) {

            cout << "Index out of range!" << endl;
            return;

        }

        if (index == 0) {

            insertAtStart(value);

        } 
        
        else if (index == size) {

            insertAtEnd(value);

        } 
        
        else {

            Node* prev = getNode(index - 1);

            if (prev) {

                Node* newNode = new Node(value);
                newNode->next = prev->next;
                prev->next = newNode;
                size++;

            }

        }

    }

    void removeAtStart() {

        if (isEmpty()) {

            cout << "List is empty!" << endl;
            return;

        }

        Node* temp = head;
        head = head->next;
        delete temp;
        temp= nullptr;
        size--;

    }

    void removeAtEnd() {

        if (isEmpty()) {

            cout << "List is empty!" << endl;
            return;

        }

        if (size == 1) {

            removeAtStart();
            return;

        }

        Node* prev = getNode(size - 2);

        if (prev) {

            delete prev->next;
            prev->next = nullptr;
            size--;

        }

    }

    void removeAtIndex(int index) {

        if (index < 0 || index >= size) {

            cout << "Index out of range!" << endl;
            return;

        }

        if (index == 0) {

            removeAtStart();

        } 
        
        else {

            Node* prev = getNode(index - 1);

            if (prev && prev->next) {

                Node* temp = prev->next;
                prev->next = temp->next;
                delete temp;
                size--;

            }

        }

    }
    
    void removeByValue(T value) {
        if (isEmpty()) {
            // cout << "List is empty!" << endl;
            return;
        }
        if (head->data == value) {
            removeAtStart();
            return;
        }
        Node* current = head;
        Node* prev = nullptr;
        while (current && current->data != value) {
            prev = current;
            current = current->next;
        }
        if (!current) {
            // cout << "Value not found in the list!" << endl;
            return;
        }
        prev->next = current->next;
        delete current;
        size--;
    }

    bool isEmpty() const {

        return size == 0;

    }

    void display() const {

        if (isEmpty()) {

            cout << "List is empty!" << endl;
            return;

        }

        Node* current = head;

        while (current) {

            cout << current->data << " -> ";
            current = current->next;
        }

        cout << "nullptr" << endl;

    }

    int getSize() const {

        return size;

    }
    Node* return_Head(){
        return head;
    }
    
};
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
template <typename K, typename V>

class HashNode {

public:

    K key;
    V value;
    HashNode* next;

    HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
};

template <typename K, typename V>

class HashTable {

private:
    HashNode<K, V>** table;
    int capacity;   
    int size;               
    float loadFactor;       

    bool isPrime(int n) {

        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;

        for (int i = 5; i * i <= n; i += 6) {

            if (n % i == 0 || n % (i + 2) == 0) return false;

        }

        return true;

    }

    int nextPrime(int n) {

        while (!isPrime(n)) n++;
        return n;

    }

    template <typename U = K>
    typename std::enable_if<!std::is_same<U, std::string>::value, int>::type
    hashFunction(const U& key) {
        const double A = 0.6180339887;
        return static_cast<int>(capacity * (key * A - static_cast<int>(key * A)));
    }

    // Specialized hash function for strings
    template <typename U = K>
    typename std::enable_if<std::is_same<U, std::string>::value, int>::type
    hashFunction(const U& key) {
        unsigned long hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % capacity;
    }

    void resize() {

        int oldCapacity = capacity;
        capacity = nextPrime(2 * capacity);
        HashNode<K, V>** oldTable = table;

        table = new HashNode<K, V>*[capacity];

        for (int i = 0; i < capacity; i++) {

            table[i] = nullptr;

        }

        for (int i = 0; i < oldCapacity; i++) {

            HashNode<K, V>* current = oldTable[i];

            while (current) {

                insert(current->key, current->value);
                HashNode<K, V>* temp = current;
                current = current->next;
                delete temp; 

            }

        }

        delete[] oldTable;

    }

public:

    HashTable(int cap = 20, float lf = 0.75) : size(0), loadFactor(lf) {

        capacity = nextPrime(cap);
        table = new HashNode<K, V>*[capacity];

        for (int i = 0; i < capacity; i++) {

            table[i] = nullptr;

        }

    }

    void clear() {
        for (int i = 0; i < capacity; i++) {
            HashNode<K, V>* current = table[i];
            while (current) {
                HashNode<K, V>* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        
        // Reset to original state
        capacity = nextPrime(20);
        size = 0;
        loadFactor = 0.75;
    }

    ~HashTable() {
        clear();
        delete[] table;

    }

    void insert(K key, V value) {

        if (size / static_cast<float>(capacity) > loadFactor) {

            resize(); 

        }
        int hashIndex = hashFunction(key);
        HashNode<K, V>* newNode = new HashNode<K, V>(key, value);

        if (!table[hashIndex]) {

            table[hashIndex] = newNode;

        }

        else {

            HashNode<K, V>* current = table[hashIndex];

            while (current) {

                if (current->key == key) {

                    current->value = value;
                    delete newNode;
                    return;

                }

                if (!current->next) break;
                current = current->next;

            }

            current->next = newNode;

        }

        size++;

    }

    V search(K key) {

        int hashIndex = hashFunction(key);
        HashNode<K, V>* current = table[hashIndex];

        while (current) {

            if (current->key == key) {

                return current->value;

            }

            current = current->next;

        }

        cout << "Key not found!" << endl;
        return V();

    }
    bool find(K key){
        int hashIndex = hashFunction(key);
        HashNode<K, V>* current = table[hashIndex];

        while (current) {

            if (current->key == key) {

                return true;

            }

            current = current->next;

        }

        // cout << "Key not found,Returning False!" << endl;
        return false;
    }
    void remove(K key) {

        int hashIndex = hashFunction(key);
        HashNode<K, V>* current = table[hashIndex];
        HashNode<K, V>* prev = nullptr;

        while (current) {

            if (current->key == key) {

                if (prev) {

                    prev->next = current->next;

                } 
                
                else {

                    table[hashIndex] = current->next;

                }

                delete current;
                size--;
                return;

            }

            prev = current;
            current = current->next;

        }

        cout << "Key not found, nothing to remove!" << endl;

    }
    
    void display() {

        for (int i = 0; i < capacity; i++) {

            cout << "Bucket " << i << ": ";
            HashNode<K, V>* current = table[i];

            while (current) {

                cout << "[" << current->key << ": " << current->value << "] -> ";
                current = current->next;

            }

            cout << "nullptr" << endl;

        }

    }
    
};
template <typename T>

class DynamicArray {

private:

    T* data;
    int sz;
    int cap;

    void resizeCapacity(int newCap) {

        T* newData = new T[newCap];

        for (int i = 0; i < sz; ++i) {

            newData[i] = data[i];

        }

        delete[] data;
        data = newData;
        cap = newCap;

    }

public:

    DynamicArray() : data(nullptr), sz(0), cap(0) {}

    DynamicArray(const DynamicArray& other) : sz(other.sz), cap(other.cap) {
        data = new T[cap];
        for (int i = 0; i < sz; ++i) {
            data[i] = other.data[i];
        }
    }

    // Copy assignment operator
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            sz = other.sz;
            cap = other.cap;
            data = new T[cap];
            for (int i = 0; i < sz; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~DynamicArray() {

        delete[] data;

    }

    bool contains(T d) {
        for (int i=0; i<sz; i++) {
            if (data[i] == d) return true;
        }
        return false;
    }

    int size() const { // return size of Dynamic Array

        return sz;

    }

    int capacity() const { // return max capacity of Dynamic Array

        return cap;

    }

    bool empty() const { // return if Dynamic Array is empty or not

        return sz == 0;

    }

    // Note: 
    // dummy used to handle error cases where index is out of range by returning a referencable object.
    // const functions are the same as the functions defined above them with the difference that the 
    // vector object is not modified in them, useful for const DynamicArray objects.

    T& at(int index) { // return data at specified index

        if (index < 0 || index >= sz) {

            cout << "Index out of range\n";
            static T dummy;        
            return dummy;

        }

        return data[index];

    }

    const T& at(int index) const { 

        if (index < 0 || index >= sz) {

            cout << "Index out of range\n";
            static T dummy;
            return dummy;

        }

        return data[index];

    }

    T& operator[](int index) { // use operator overloading to define [] to reference given index

        return data[index];

    }

    const T& operator[](int index) const {

        return data[index];

    }

    T& front() { // return the first element in the Dynamic Array

        if (empty()) {

            cout << "Dynamic Array is empty\n";
            static T dummy;
            return dummy;

        }

        return data[0];

    }

    const T& front() const {

        if (empty()) {

            cout << "Dynamic Array is empty\n";
            static T dummy;
            return dummy;

        }

        return data[0];

    }

    T& back() { // return the last element in the Dynamic Array

        if (empty()) {

            cout << "Dynamic Array is empty\n";
            static T dummy;
            return dummy;

        }

        return data[sz - 1];

    }

    const T& back() const {

        if (empty()) {

            cout << "Dynamic Array is empty\n";
            static T dummy;
            return dummy;

        }

        return data[sz - 1];

    }

    void push_back(const T& value) { // insert element at end of Dynamic Array

        if (sz == cap) {

            if (cap == 0) {

                cap = 1;  
            } 
            
            else {

                cap *= 2; 

            }

            T* newData = new T[cap];  
            
            for (int i = 0; i < sz; ++i) {

                newData[i] = data[i];

            }

            delete[] data;  
            data = newData;  

        }

        data[sz++] = value;

    }

    void pop_back() { // remove element from end of Dynamic Array

        if (empty()) {

            cout << "Dynamic Array is empty\n";
            return;

        }

        sz--;

    }

    void clear() {
        delete[] data;
        data = nullptr;
        sz = 0;
        cap = 0;
    }

    void reserve(int newCap) { // allocate an amount of memory to the Dynamic Array

        if (newCap > cap) {

            resizeCapacity(newCap);

        }

    }

    void shrink_to_fit() { // shrink the Dynamic Array to its current size

        if (sz < cap) {

            resizeCapacity(sz);

        }

    }

    void insert(int index, const T& value) {

        if (index < 0 || index > sz) {

            cout << "Index out of range\n";
            return;

        }

        if (sz == cap) {

            int newCap;

            if (cap == 0) {

                newCap = 1;

            }
            
            else {

                newCap = cap * 2;

            }

            resizeCapacity(newCap);

        }

        for (int i = sz; i > index; --i) {

            data[i] = data[i - 1];

        }

        data[index] = value;
        ++sz;

    }

    void erase(int index) {

        if (index < 0 || index >= sz) {

            cout << "Index out of range\n";
            return;

        }

        for (int i = index; i < sz - 1; ++i) {

            data[i] = data[i + 1];

        }

        sz--;

    }

    void resize(int newSize) {
        if (newSize > cap) {
            resizeCapacity(newSize);
        }
        if (newSize > sz) {
            for (int i = sz; i < newSize; ++i) {
                data[i] = T();
            }
        }
        sz = newSize;
    }

    // Functions to support range-based for loop
    T* begin() {
        return data;
    }

    const T* begin() const {
        return data;
    }

    T* end() {
        return data + sz;
    }

    const T* end() const {
        return data + sz;
    }

    void display() {
        for (int i=0; i<sz; i++) {
            cout<<data[i]<<" ";
        } cout<<endl;
    }

};
class Road {
    Intersection* dest;
    int travelTime; //weight
    PriorityQueue<Vehicle*> priority_Queue;
    string status;

    public:
    Road(int time, Intersection* b): dest(b), status("Clear"), priority_Queue(false) {
        travelTime = time;
    }

    void setStatus(string s) {
        status = s;
    }
    
    string getStatus(){
        return status;
    }

    Vehicle* getHeapTop() {
        return priority_Queue.top();
    }

    void clearQueue() {
        while (priority_Queue.getSize() > 0) {
            priority_Queue.pop();
        }
    }

    Intersection*& getDest() {
        return dest;
    }

    int getTrafficLoad() {
        return priority_Queue.getSize();
    }

    int getCapacity() const {
        return 1.0 / travelTime;
    }

    int getWeight() {
        return travelTime;
    }

    void insertVehicle(Vehicle* vehicle) {
        priority_Queue.push(vehicle);
    }

    void removeVehicle() {
        priority_Queue.pop();
    }

    void displayQ() {
        priority_Queue.display();
    }

    friend ostream& operator<<(ostream& os, const Road& road);

};
class Vehicle {
    protected:
    string name;
    DynamicArray<Road*> route;
    int currRoadIndex; // will change as road changes
    Intersection* start, *end;
    bool atDest;
    int priorityLevel;
    int travelTime;

    public:
    Vehicle(string n, int p=0) : name(n), currRoadIndex(0), atDest(false), priorityLevel(p) {}

    Vehicle(string n, Intersection* a, Intersection* b, int p=0) : name(n), start(a), end(b), currRoadIndex(0), atDest(false), priorityLevel(p) {}

    void setRoad();

    void addRoad(Road* r) {
        route.push_back(r);
    }

    void setPriorityLevel(int p) {
        priorityLevel = p;
    }

    string getName() {
        return name;
    }

    int getTime() {
        return travelTime;
    }

    void setTime(int t) {
        travelTime= t;
    }

    void updateTime() {
        if (travelTime>0) travelTime--;
    }

    void setStart(Intersection* temp){
        start=temp;
    }

    Intersection* getStart() {
        return start;
    }

    Intersection* getEnd() {
        return end;
    }

    DynamicArray<Road*> getRoute() {
        return route;
    }

    int getIndex(){
        return currRoadIndex;
    }

    void clearRoute() {
        route.clear();
    }

    void setIndex(int i) {
        currRoadIndex = i;
    }
    
    void moveIndex();

    void setAtDest(bool arg) {
        atDest = arg;
    }
    bool getAtDest() {
        return atDest;
    }

    int getPriorityLevel() {
        return priorityLevel;
    }

    virtual void setOld(char o) {}

    virtual char getOld() {
        return '\0';
    }

    bool operator<(const Vehicle& other) const {
        if (this->travelTime == other.travelTime) {
            return this->priorityLevel < other.priorityLevel;
        }
        return this->travelTime > other.travelTime;
    }

    bool operator>(const Vehicle& other) const {
        if (this->travelTime == other.travelTime) {
            return this->priorityLevel > other.priorityLevel;
        }
        return this->travelTime < other.travelTime;
    }

    bool operator<=(const Vehicle& other) const {
        return !(*this > other);
    }

    bool operator>=(const Vehicle& other) const {
        return !(*this < other);
    }

    bool operator==(const Vehicle& other) const {
        return this->priorityLevel == other.priorityLevel &&
            this->travelTime == other.travelTime;
    }

    friend ostream& operator<< (ostream& os, Vehicle* obj) {
        os<<obj->getName()<<" ";
        return os;
    }

    virtual ~Vehicle() {}
};
class TrafficSignal {
    int green, yellow, red;
    string currentState;
    int currentTime;

    public:
    TrafficSignal(int g=0, int r =69) {
        green = g;
        yellow = 3;
        red =r;
    }

    void setGreenTime(int g) {

        green = g;
    }

    void setRedTime(int r) {
        red = r;
    }

    void setYellowTime(int y) {
        yellow = y;
    }

    void setCurrentTime(int c) {
        currentTime = c;
    }

    int getRedTime() {
        return red;
    }

    void update() {
        if (currentState == "GREEN") {
            if (currentTime>0) currentTime--;
            if (currentTime <= 0 && yellow != 0) {
                currentState = "YELLOW";
                currentTime = yellow;
            }
            else if (currentTime <= 0) {
                currentTime = green;
            }
        } else if (currentState == "YELLOW") {
            if (currentTime>0) currentTime--;
            if (currentTime <= 0) {
                currentState = "RED";
                currentTime = red;
            }
        } else if (currentState == "RED") {
            if (currentTime>0) currentTime--;
            if (currentTime <= 0) {
                currentState = "GREEN";
                currentTime = green;
            }
        }
    }

    void setState(string s) {
        currentState =s;
    }

    void resetTime() {
        if (currentState == "GREEN") {
            currentTime = green;
        } else if (currentState == "YELLOW") {
            currentTime = yellow;
        } else if (currentState == "RED") {
            currentTime = red;
        }
    }

    int getGreenTime() const {
        return green;
    }

    string getState() const {
        return currentState;
    }

    int getTimeRemaining() const {
        return currentTime;
    }
};
class Intersection {
    char name;
    DynamicArray<Road*>inRoads;
    DynamicArray<Road*>outRoads;
    DynamicArray<TrafficSignal> signals;
    int greenTime;
    int x,y;

    public:
    Intersection (char n): x(0), y(0) {
        name =n;
    }

    void setGreenTime(int t) {
        greenTime = t;
    }

    void addOutRoad(Road* r);

    void addInRoad(Road* r);

    void set_X(int a){
        x=a;
    }

    void set_Y(int a){
        y=a;
    }
    
    DynamicArray<Road*> getInroads(){
        return inRoads;
    }

    string getSignal(Road* road);
    
    int get_X(){
        return x;
    }

    int get_Y(){
        return y;
    }
    char getName() const {
        return name;
    }
    int getSignalTime(Road* road){
        for (int i=0; i<inRoads.size(); i++ ) {
            if (inRoads[i] == road) {
                return signals[i].getTimeRemaining();
            }
        }
        return -1;
    }
    void calculateSignalTimings();

    void updateSignals();

    bool signalActive(Road* road) {
        for (int i=0; i<inRoads.size(); i++ ) {
            if (inRoads[i] == road) {
                return signals[i].getState() == "GREEN";
            }
        }
        return false;
    }

};

class EmergencyVehicle : public Vehicle { // dynamic routing
    string priority;
    char old;

    public:
    EmergencyVehicle(string n, string p) : Vehicle(n), priority(p) {
        setPriorityLevel();
    }

    EmergencyVehicle(string n, Intersection* a, Intersection* b , string p) : Vehicle(n, a, b), priority(p) {
        setPriorityLevel();
    }

    void setOld(char o) override {
        old = o;
    }
    char getOld() override {
        return old;
    }

    void setPriorityLevel() {
        if (priority=="High")
            priorityLevel = 69;
        else if (priority=="Medium")
            priorityLevel = 20;
        else if (priority=="Low")
            priorityLevel = 10;
    }
};
ostream& operator<<(ostream& os, const Road& road) {
    os << road.dest->getName();
    return os;
}
void Vehicle::moveIndex() {
    currRoadIndex+=1;
    setRoad();
}

void Vehicle::setRoad() {
    if( route.size()!=0 && (currRoadIndex < route.size() && !atDest) ) {
        travelTime = route[currRoadIndex]->getWeight();
        route[currRoadIndex]->insertVehicle(this);
    }
}
void Intersection::addOutRoad(Road* r) {
    outRoads.push_back(r);
}

void Intersection::addInRoad(Road* r) {
    inRoads.push_back(r);
}

string Intersection:: getSignal(Road* road){
    for (int i=0; i<inRoads.size(); i++ ) {
        
        if (inRoads[i] == road) {
            return signals[i].getState();
        }
    }
    return "false";
}

void Intersection::calculateSignalTimings() {
    if (inRoads.size() == 0) {
        return;
    }

    int totalCycleTime = inRoads.size() * (greenTime + 3); // Total cycle time including yellow time

    signals.clear();

    if (inRoads.size() == 1) { // signal will always be green if only 1 incoming road
        Road* road = inRoads[0];
        TrafficSignal signal(greenTime, 0);
        signal.setYellowTime(0);
        signal.setState("GREEN");
        signal.resetTime();
        signals.push_back(signal);
    }
    else {
        for (int i = 0; i < inRoads.size(); ++i) {
            Road* road = inRoads[i];
            int roadRedTime = totalCycleTime - (greenTime );

            TrafficSignal signal(greenTime, roadRedTime);
            if (i == 0) {
                signal.setState("GREEN");
                signal.resetTime();
            } else {
                signal.setState("RED");
                signal.setCurrentTime(totalCycleTime - (i * (greenTime + 3)));
            }
            signals.push_back(signal);

        }
    }
}

void Intersection::updateSignals() {
    for (TrafficSignal& signal : signals) {
        signal.update();
    }
}

#define INT_MAX 10000000

int stringToInt(const string& str) {
    int result = 0;

    for (int i = 0; i < str.length(); ++i) {
        char c = str[i];
        int digit = c - '0';
        result = result * 10 + digit;
    }

    return result;
}

struct IntersectionRoad {
    Intersection* intersection;
    Road* road;
};

class DistanceNode { // for priority q
public:
    Intersection* intersection;
    int distance;

    DistanceNode() {}

    DistanceNode(Intersection* intersection, int distance) : intersection(intersection), distance(distance) {}

    bool operator<(const DistanceNode& other) const {
        return distance < other.distance;
    }

    bool operator > (const DistanceNode& other) const {
        return distance > other.distance;
    }

    friend ostream& operator << (ostream& ss, const DistanceNode& n) {
        ss<<n.intersection->getName()<<" ";
        return ss;
    }
};

class AdjacencyList {
    DynamicArray<IntersectionRoad> roads; // temporary
    DynamicArray<LinkedList<Road*>> graph;
    DynamicArray<Intersection*> intersections;
    DynamicArray<Vehicle*> vehicles;
    DynamicArray<EmergencyVehicle*> emergencyVehicles;
    HashTable<string, LinkedList<Vehicle*>*> roadVehicleMap;
    bool Simulation=true;
    bool initialised = false;

    void loadEmergencyVehicles(fstream& file) {
        string line;
        int counter=0;
        while (getline(file, line)) {
            if (counter == 0) {
                counter += 1;
                continue;
            }
            stringstream ss(line);
            string veh, st, end, priority;
            getline(ss, veh, ',');
            getline(ss, st, ',');
            getline(ss, end, ',');
            getline(ss, priority, ',');

            if (!priority.empty() && priority.back() == '\r') {
                priority.pop_back();
            }

            EmergencyVehicle* vehicle = new EmergencyVehicle(veh, intersections[st[0]-'A'], intersections[end[0]-'A'], priority);
            emergencyVehicles.push_back(vehicle);
        }
        file.close();
    }

    void loadVehicles(fstream& file) {
        string line;
        int counter=0;
        while (getline(file, line)) {
            if (counter == 0) {
                counter += 1;
                continue;
            }
            stringstream ss(line);
            string veh, st, end;
            getline(ss, veh, ',');
            getline(ss, st, ',');
            getline(ss, end, ',');

            Vehicle* vehicle = new Vehicle(veh, intersections[st[0]-'A'], intersections[end[0]-'A']);
            vehicles.push_back(vehicle);
        }
        file.close();
    }

    void loadSignalTimings(fstream& file) {
        string line;
        int counter=0;
        while (getline(file, line)) {
            if (counter == 0) {
                counter += 1;
                continue;
            }
            string node, time;
            stringstream ss(line);
            getline(ss, node, ',');
            getline(ss, time, ',');

            if (!time.empty() && time.back() == '\r') {
                time.pop_back();
            }

            intersections[node[0]-'A']->setGreenTime(stoi(time));
        }
        file.close();
    }

    void loadBlockages(fstream& file) { // for setting statuses of roads
        string line;
        int counter=0;
        while (getline(file, line)) {
            if (counter == 0) {
                counter += 1;
                continue;
            }
            stringstream ss(line);
            string start, end, status;
            getline(ss, start, ',');
            getline(ss,end , ',');
            getline(ss, status, ',');

            if (!status.empty() && status.back() == '\r') {
                status.pop_back();
            }

            char startNode = start[0]; char endNode= end[0];
            LinkedList<Road*>& edges = graph[startNode-'A'];
            LinkedList<Road*>::Node* edge = edges.getHead();

            Road* road=nullptr;
            while (edge) {
                if (edge->data->getDest()->getName() == endNode) {
                    road= edge->data;
                    road->setStatus(status);

                    break;
                }
                edge = edge->next;
            }

        }

        file.close();
    }

    void initialiseEmergencyVehicles() {
        for (EmergencyVehicle* vehicle : emergencyVehicles) {
            if (!vehicles.contains(vehicle))
                vehicles.push_back(vehicle);
            
            // Calculate initial path using A* algorithm
            DynamicArray<char> refs = aStarAlgo(vehicle->getStart(), vehicle->getEnd());
            DynamicArray<char> path = constructPath(refs, vehicle->getStart(), vehicle->getEnd());

            vehicle->clearRoute();
            
            if (!path.empty() && path.size() > 1) {
                // Only process the first road in the path
                char currentNode = path[0];
                char nextNode = path[1];
                
                // Find the corresponding road
                Road* road = nullptr;
                LinkedList<Road*>& edges = graph[currentNode - 'A'];
                LinkedList<Road*>::Node* edge = edges.getHead();
                
                while (edge) {
                    if (edge->data->getDest()->getName() == nextNode) {
                        road = edge->data;
                        break;
                    }
                    edge = edge->next;
                }
                
                if (road) {
                    vehicle->addRoad(road);
                    vehicle->setRoad();
                    vehicle->setOld(vehicle->getStart()->getName());
                    
                    // Add to roadVehicleMap
                    string key = "";
                    key += currentNode;
                    key += nextNode;
                    
                    if (roadVehicleMap.find(key)) {
                        roadVehicleMap.search(key)->insertAtStart(vehicle);
                    } else {
                        LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                        newList->insertAtStart(vehicle);
                        roadVehicleMap.insert(key, newList);
                    }
                }
            }
        }
    }

    void initialiseVehicles() {

        for (Vehicle* vehicle: vehicles) {
            DynamicArray<char> refs = dijkstraAlgo(vehicle->getStart(), vehicle->getEnd());
            DynamicArray<char> path = constructPath(refs, vehicle->getStart(), vehicle->getEnd());
            vehicle->clearRoute();
            if(!path.empty()) {
                for (int i=0; i<path.size()-1; i++) {
                    char c = path[i];
                    char cNext= path[i+1];
                    Road* road = nullptr;
                    LinkedList<Road*>& edges = graph[c-'A'];
                    LinkedList<Road*>::Node* edge = edges.getHead();

                    while (edge) {
                        if (edge->data->getDest() -> getName() == cNext) {
                            road= edge->data;
                            break;
                        }
                        edge = edge->next;
                    }

                    if (road){
                        vehicle->addRoad(road);
                    }
                }

                vehicle->setRoad();
                //Initializing HashMap of vehicles on roads
                string key="";
                key+=path[0];
                key+=path[1];
                if (roadVehicleMap.find(key)) {

                    roadVehicleMap.search(key)->insertAtStart(vehicle);
                } 
                else {
                    LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                    newList->insertAtStart(vehicle);
                    roadVehicleMap.insert(key, newList);
                }
                
            }else{
                vehicle->setAtDest(true);
            }

        }
    }

    void initialiseGraph() {
        fstream network_file("files/road_network.csv", ios::in);
        fstream signals_file("files/traffic_signal_timings.csv", ios::in);
        fstream vehicles_file("files/vehicles.csv", ios::in);
        fstream EmergencyVehicles_file("files/emergency_vehicles.csv", ios::in);
        fstream blockages_file("files/accidents_or_closures.csv", ios::in);

        string line;
        char maxNode='\0';
        int counter=0;
        while (getline(network_file, line)) {
            if (counter == 0) {
                counter += 1;
                continue;
            }
            stringstream ss(line);
            string start, end, weight;
            getline(ss, start, ',');
            getline(ss, end, ',');
            getline(ss, weight, ',');

            if (!weight.empty() && weight.back() == '\r') {
                weight.pop_back();
            }
        
            char startNode = start[0];
            char endNode = end[0];
            int weightInt = stringToInt(weight);
            maxNode = max(maxNode, max(startNode, endNode));
        
            // Ensure intersections DynamicArray is large enough
            while (intersections.size() <= (maxNode - 'A')) {
                char val = 'A' + intersections.size();
                Intersection* newNode = new Intersection(val);
                intersections.push_back(newNode);
            }
        
            Intersection* a = nullptr;
            for (Intersection* intsc : intersections) {
                if (intsc->getName() == startNode) {
                    a = intsc;
                    break;
                }
            }
        
            Intersection* b = nullptr;
            for (Intersection* intsc : intersections) {
                if (intsc->getName() == endNode) {
                    b = intsc;
                    break;
                }
            }
        
            if (a && b) {
                Road* edge = new Road(weightInt, b);
                roads.push_back({a, edge});
            }
        }

        graph.resize(maxNode - 'A' + 1);

        // pushin the edges
        for (IntersectionRoad road: roads) {
            // separated Intersection and List for edge for ease of access.
            char startNode = road.intersection->getName();
            char endNode = road.road->getDest()->getName();
            graph[startNode - 'A'].insertAtEnd(road.road);

            intersections[startNode-'A']->addOutRoad(road.road);
            intersections[endNode -'A']->addInRoad(road.road);
        }

        network_file.close();
        loadBlockages(blockages_file);
        loadSignalTimings(signals_file);
        loadVehicles(vehicles_file);
        loadEmergencyVehicles(EmergencyVehicles_file);
        // initialiseVehicles();
        // initialiseEmergencyVehicles();

        // initialise signal timings
        for (Intersection* intsc: intersections) {
            intsc->calculateSignalTimings();
        }
    }

    public:
    AdjacencyList() {
        initialiseGraph();
    }

    bool getSimulationBool(){
        return Simulation;
    }

    void setSimultaionBool(bool set){
        Simulation=set;
    } 

    DynamicArray<char> dijkstraAlgo(Intersection* start, Intersection* end) { // returns a list of closest previous nodes for each nodes that can be tracked to a single starting point through constructPath
        // setting initial distances to MAX, visited, and priority q
        DynamicArray<int> distance;
        DynamicArray<char> previousReferences;
        DynamicArray<bool> visited;
        PriorityQueue<DistanceNode> pq(true, intersections.size());
        distance.resize(intersections.size());
        previousReferences.resize(intersections.size());
        visited.resize(intersections.size());
        for (bool v: visited) v=false;
    
        for (Intersection* intsc : intersections) {
            distance[intsc->getName() - 'A'] = INT_MAX;
            previousReferences[intsc->getName() - 'A'] = '\0';
        }
        distance[start->getName() - 'A'] = 0;
        pq.push(DistanceNode(start, 0));
        // main algo
        while (!pq.isEmpty()) {
            DistanceNode minNode = pq.top();
            pq.pop();
    
            Intersection* minIntersection = minNode.intersection;
            char minNodeName = minIntersection->getName();
    
            if (visited[minNodeName - 'A'])
                continue;
            
            visited[minNodeName - 'A'] = true;
    
            LinkedList<Road*>& edges = graph[minNodeName - 'A'];
            LinkedList<Road*>::Node* edge = edges.getHead();
            while (edge) {

                if(edge->data->getStatus() == "Clear") {
                    Intersection* neighbour = edge->data->getDest();
                    int weight = edge->data->getWeight();
                    int tempDistance = distance[minNodeName - 'A'] + weight;
                    if (tempDistance < distance[neighbour->getName() - 'A']) {
                        distance[neighbour->getName() - 'A'] = tempDistance;
                        previousReferences[neighbour->getName() - 'A'] = minNodeName;
                        pq.push(DistanceNode(neighbour, tempDistance));
                    }
                }
                edge = edge->next;
            }

        }
        return previousReferences;
    }
    
    DynamicArray<char> constructPath(DynamicArray<char>& previousRefs, Intersection* start, Intersection* end) {
        DynamicArray<char> path;
        for (char at = end->getName(); at != '\0'; at = previousRefs[at - 'A']) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
    
        if (path.empty() || path[0] != start->getName()) {
            path.clear();
        }
        return path;
    }

    // exact way to measure heuristic is to calculate the travelTime from the edge to the ending edge. 
    // (could also require checking congestion (secondary weight))

    DynamicArray<char> aStarAlgo(Intersection* start, Intersection* end) {

        // Initialize DynamicArrays and priority queue
        DynamicArray<int> distance; // g(n): cost to reach the node
        DynamicArray<int> heuristic; // h(n): estimated cost to goal
        DynamicArray<char> previousReferences; // Tracks the path
        DynamicArray<bool> visited;
        PriorityQueue<DistanceNode> pq(true, intersections.size()); // Min-heap

        distance.resize(intersections.size());
        heuristic.resize(intersections.size());
        previousReferences.resize(intersections.size());
        visited.resize(intersections.size());

        for (int i = 0; i < visited.size(); i++) {
            visited[i] = false;
            distance[i] = INT_MAX;
            previousReferences[i] = '\0';
            heuristic[i] = 0; // Default heuristic
        }

        // Heuristic: travelTime of roads connected to the goal
        for (Intersection* intsc : intersections) {
            
            char name = intsc->getName();
            LinkedList<Road*>& edges = graph[name - 'A'];
            LinkedList<Road*>::Node* edge = edges.getHead();

            int minTravelTime = INT_MAX;
            
            while (edge) {
                Road* road = edge->data;
                if (road->getStatus() == "Clear") {
                    int travelTime = road->getWeight() + road->getTrafficLoad();
                    if (travelTime < minTravelTime) {
                        minTravelTime = travelTime;
                    }
                }
                edge = edge->next;
            }
            heuristic[name - 'A'] = (minTravelTime == INT_MAX) ? 0 : minTravelTime;
        }

        // Initialize the starting node
        distance[start->getName() - 'A'] = 0;
        pq.push(DistanceNode(start, heuristic[start->getName() - 'A'])); // f(n) = g(n) + h(n)

        // Main A* loop
        while (!pq.isEmpty()) {
            DistanceNode minNode = pq.top();
            pq.pop();

            Intersection* minIntersection = minNode.intersection;
            char minNodeName = minIntersection->getName();

            if (visited[minNodeName - 'A'])
                continue;

            visited[minNodeName - 'A'] = true;

            // Stop early if the goal is reached
            if (minIntersection == end)
                break;

            LinkedList<Road*>& edges = graph[minNodeName - 'A'];
            LinkedList<Road*>::Node* edge = edges.getHead();

            while (edge) {
                Road* road = edge->data;
                if (road->getStatus() == "Clear") {
                    Intersection* neighbour = road->getDest();
                    
                    // Adjust travel time for congestion
                    int travelTime = road->getWeight() + road->getTrafficLoad();

                    int tempDistance = distance[minNodeName - 'A'] + travelTime; 

                    if (tempDistance < distance[neighbour->getName() - 'A']) {
                        distance[neighbour->getName() - 'A'] = tempDistance;
                        previousReferences[neighbour->getName() - 'A'] = minNodeName;

                        // Calculate f(n) = g(n) + h(n)
                        int priority = tempDistance + heuristic[neighbour->getName() - 'A'];
                        pq.push(DistanceNode(neighbour, priority));
                    }
                }
                edge = edge->next;
            }
        }

        return previousReferences;
    }

    // set a 1 sec delay in addition of vehicles in vehicle array so each car appears
    // behind the other and no collision happens to occur
    void updateSimulation() {
        DynamicArray<Vehicle*> atRoadEnd;
        DynamicArray<Road*> doneRoads;
        

        for(Vehicle* veh: vehicles) {
            
            if( !veh->getRoute().empty() && !veh->getAtDest() ) {
                DynamicArray<Road*> route = veh->getRoute();

                if (route.size()==veh->getIndex() && veh->getTime() <= 1 ) {
                    Intersection* currentIntersection =  route[veh->getIndex() - 1]->getDest();

                    if(currentIntersection->getName() == veh->getEnd()->getName()){
                        if(veh->getTime()==1){
                            veh->setTime(0);
                        }
                        if(veh->getRoute()[veh->getIndex()-1]->getDest()->signalActive(veh->getRoute()[veh->getIndex()-1])){
                            cout << "\033[1;33m"<<veh->getName()<<" Reached Destination\033[0m" << endl;
                            veh->setAtDest(true);
                        }
                    }
                    else if(veh->getRoute()[0]->getDest()->signalActive(veh->getRoute()[0])) {
                        // Need to find next path
                        string oldKey="";
                        oldKey+=veh->getOld();
                        oldKey+=currentIntersection->getName();

                        DynamicArray<char> refs = aStarAlgo(currentIntersection, veh->getEnd());
                        DynamicArray<char> newPath = constructPath(refs, currentIntersection, veh->getEnd());
                        
                        if(!newPath.empty() && newPath.size() > 1) {

                            char currentNode = newPath[0];
                            char nextNode = newPath[1];
                            Road* nextRoad = nullptr;

                            roadVehicleMap.search(oldKey)->removeByValue(veh);
                            
                            LinkedList<Road*>& edges = graph[currentNode - 'A'];
                            LinkedList<Road*>::Node* edge = edges.getHead();
                            
                            while(edge) {

                                if(edge->data->getDest()->getName() == nextNode && 
                                edge->data->getStatus() == "Clear") {
                                    nextRoad = edge->data;
                                    break;
                                }
                                edge = edge->next;
                            }

                            if(nextRoad) {
                                veh->clearRoute();
                                veh->addRoad(nextRoad);
                                veh->setTime(nextRoad->getWeight());
                                veh->setIndex(1);

                                veh->setOld(currentIntersection->getName());
                                route = veh->getRoute();

                                // Update hashmap
                                string newKey = "";
                                newKey += currentNode;
                                newKey += nextNode;

                                if(roadVehicleMap.find(newKey)) {
                                    roadVehicleMap.search(newKey)->insertAtStart(veh);
                                } else {
                                    LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                                    newList->insertAtStart(veh);
                                    roadVehicleMap.insert(newKey, newList);
                                }
                            }
                        }
                    }
                }

                else {
                    //Making key
                    string key="69";

                    //Updating road
                    veh->updateTime();
                    Road* road = veh->getRoute()[veh->getIndex()];
            
                    if (veh->getTime() == 0) {
                        atRoadEnd.push_back(veh);
                    }
                    
                    int temp_idx=veh->getIndex();
                    if (!doneRoads.contains(road)) {
                        if ( veh->getTime()==0 && road->getDest()->signalActive(road)) {

                            veh = road->getHeapTop();

                            route = veh->getRoute();
                            temp_idx=veh->getIndex();
                            key="";

                            if(veh->getIndex()==0) {
                                key+=veh->getStart()->getName();
                                key+=route[veh->getIndex()]->getDest()->getName();
                            }
                            else {
                                key+=route[veh->getIndex() - 1]->getDest()->getName();
                                key+=route[veh->getIndex()]->getDest()->getName();
                            }
                            
                            doneRoads.push_back(road);
                            veh->moveIndex();
                            road->removeVehicle();
                        }
                    }

                    // FINDING NEXT ROAD
                    // need to add the functionality that if the next road is blocked, then the 
                    // route needs to be recalculated from that intersection (don't forget to
                    // delete the roads ahead of that!)

                    // Updating HashMap
                    if( veh->getIndex()>temp_idx && route.size()!=veh->getIndex() ) {
                        Road* nextRoad = route[veh->getIndex()];

                        if (nextRoad->getStatus() != "Clear") {
                            nextRoad->removeVehicle();
                            Intersection* currentIntersection = route[veh->getIndex() - 1]->getDest();

                            // new path calculation
                            DynamicArray<char> refs = aStarAlgo(currentIntersection, veh->getEnd());
                            DynamicArray<char> newPath = constructPath(refs, currentIntersection, veh->getEnd());

                            roadVehicleMap.search(key)->removeByValue(veh);
                            if (!newPath.empty() && newPath.size() > 1) {
                                // Clear routes after current index
                                DynamicArray<Road*> newRoute;
                                for (int i = 0; i < veh->getIndex(); i++) {
                                    newRoute.push_back(route[i]);
                                }
                                
                                // Add new roads from the new path
                                for (int i = 0; i < newPath.size() - 1; i++) {
                                    char c = newPath[i];
                                    char cNext = newPath[i + 1];
                                    Road* road = nullptr;
                                    LinkedList<Road*>& edges = graph[c - 'A'];
                                    LinkedList<Road*>::Node* edge = edges.getHead();
                                    
                                    while (edge) {
                                        if (edge->data->getDest()->getName() == cNext) {
                                            road = edge->data;
                                            break;
                                        }
                                        edge = edge->next;
                                    }
                                    
                                    if (road) {
                                        newRoute.push_back(road);
                                    }
                                }
                                
                                // Update vehicle's route
                                veh->clearRoute();
                                for (Road* r : newRoute) {
                                    veh->addRoad(r);
                                }
                                veh->setRoad();
                                
                                // Update hashmap with first new road
                                string newKey = "";
                                newKey += newPath[0];
                                newKey += newPath[1];
                                
                                if (roadVehicleMap.find(newKey)) {
                                    roadVehicleMap.search(newKey)->insertAtStart(veh);
                                } else {
                                    LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                                    newList->insertAtStart(veh);
                                    roadVehicleMap.insert(newKey, newList);
                                }
                            } else {
                                cout << "\033[1;33m" << veh->getName() 
                                    << " cannot reach destination - no alternate route available\033[0m" << endl;
                                veh->setAtDest(true);  // Mark as "reached" to remove from simulation
                            }
                        }
                        else {
                            // existing hashmap updation logic
                            roadVehicleMap.search(key)->removeByValue(veh);

                            key="";
                            key+=route[veh->getIndex() - 1]->getDest()->getName();
                            key+=route[veh->getIndex()]->getDest()->getName();

                            if (roadVehicleMap.find(key)) {
                                roadVehicleMap.search(key)->insertAtStart(veh);
                            }
                            else {
                                LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                                newList->insertAtStart(veh);
                                roadVehicleMap.insert(key, newList);
                            }
                        }

                    }
                }

            }
            else {
                // cout<<"No path Found\n";
            }
        
        }

        for (Intersection* intsc: intersections) {
            intsc->updateSignals();
        }

        // displaying HashMap
        display_Vehicles_at_Roads();
    }
   
    //Displaying Road Status
    void display_Roads_Status() {
        for (Intersection* intsc : intersections) {
            LinkedList<Road*>& list = graph[intsc->getName()-'A'];
            for (LinkedList<Road*>::Node* node = list.getHead(); node != nullptr; node = node->next) {
                Road* rd = node->data;
                cout<<intsc->getName()<<" to "<<rd->getDest()->getName()<<" status: "<<rd->getStatus()<<endl;
            }
        }
    }
    
    //Displaying Vehicles on each road
    void display_Vehicles_at_Roads() {
        cout << "\033[1;34m----------------------------Display of vehicles------------------------\033[0m" << endl;
        for(Intersection* intsc : intersections){
            char name=intsc->getName();
            LinkedList<Road*>::Node* node=graph[name-'A'].getHead();
            while(node) {
                string key="";
                key+=name;
                key+=node->data->getDest()->getName();
                if(roadVehicleMap.find(key)) {
                    LinkedList<Vehicle*>* temp_veh = roadVehicleMap.search(key);
                    LinkedList<Vehicle*>::Node* veh_head = temp_veh->getHead();
                    
                    // getting which vehicles have reached dest.
                    DynamicArray<Vehicle*> tempVs;
                    while (veh_head) {
                        if (veh_head->data->getAtDest()){
                            tempVs.push_back(veh_head->data);
                        }
                        veh_head = veh_head->next;
                    }

                    // removing vehicles that have reached dest.
                    for (Vehicle* veh: tempVs) {
                        temp_veh->removeByValue(veh);
                    }
                    string color="";
                    if(node->data->getDest()->getSignal(node->data)=="GREEN"){
                        color="\033[1;32m";
                    }
                    else if(node->data->getDest()->getSignal(node->data)=="RED"){
                        color="\033[1;31m";
                    }
                    else if(node->data->getDest()->getSignal(node->data)=="YELLOW"){
                        color="\033[1;33m";
                    }
                    veh_head= temp_veh->getHead();
                    cout<<color<<"Signal: "<<node->data->getDest()->getSignal(node->data)<<" Time: "<<node->data->getDest()->getSignalTime(node->data)<<" Vehicles in Path "<<key[0]<<" to "<<key[1]<<" are "<<temp_veh->getSize()<<" : ";
                    
                    while (veh_head) {
                        cout<<veh_head->data->getName()<<" ( " << veh_head->data->getPriorityLevel() << ", "<< veh_head->data->getTime() << " ) ";
                        veh_head = veh_head->next;
                    }
                    cout << "\033[0m" << endl;

                }
                node=node->next;

            }
        }
        bool flag=false;
        for(Vehicle* veh: vehicles){
            if(!veh->getAtDest()){
                flag=true;
            }
        }
        setSimultaionBool(flag);

    }

    void displaySignals() {
        cout<<"\033[1;34m---------------Traffic Signal Status-------------------\033[0m\n";
        for (Intersection* intsc : intersections){
            
            cout<<"\n\033[1;34mIntersection: "<<intsc->getName()<<" \033[0m\n";
            if(intsc->getInroads().empty()){
                cout<<"\033[1;33mNo Signal\033[0m\n";
            }
            int count=1;
            for(Road* inrd: intsc->getInroads()) {
                if (inrd->getDest()->getSignal(inrd) == "GREEN") {
                    cout << "\033[1;32mSignal "<<count<<": GREEN TIME: ";
                    cout<<inrd->getDest()->getSignalTime(inrd)<<"\033[0m";
                } 
                else if (inrd->getDest()->getSignal(inrd) == "RED") {
                    cout << "\033[1;31mSignal "<<count<< ": RED TIME: ";
                    cout<<inrd->getDest()->getSignalTime(inrd)<<"\033[0m";
                } 
                else if (inrd->getDest()->getSignal(inrd) == "YELLOW") {
                    cout << "\033[1;33mSignal "<< count << ": YELLOW TIME: ";
                    cout<<inrd->getDest()->getSignalTime(inrd)<<"\033[0m";
                }
                count++;
                cout<<endl;
            }
        }
    }

    void displayVehiclePaths(){
        cout<<"\033[1;34m---------------Vehicle's Travel Routes-------------------\033[0m\n";
        for(Vehicle* veh : vehicles){
            if(veh->getPriorityLevel()==0){
                cout<<"\033[1;32m"<<veh->getName()<<" : ";
                DynamicArray<Road*> route=veh->getRoute();
                if(route.empty()){
                    cout<<"\033[1;31mNo Path Found.\033[0m\n";
                }                
                else{
                    cout<<veh->getStart()->getName();
                    int weight=0;
                    for(Road* rd:route){
                        weight+=rd->getWeight();
                        cout<<"->"<<rd->getDest()->getName();
                    }
                    cout<<"\033[1;33m  | Weight: "<<weight;
                    cout<<"\033[0m\n";
                }
            }
        }
    }

    void displayGraph() {
        cout << "\033[1;36m-------------City Graph System-----------\033[0m" << endl;
        for (Intersection* intsc : intersections) {
            char name = intsc->getName();
            cout << "\033[1;36m" << name << ": \033[0m";
            LinkedList<Road*>::Node* temp = graph[name - 'A'].getHead();
            if (temp == nullptr) {
                cout << "\033[1;36mNo Road\033[0m";
            } 
            else {
                while (temp != nullptr) {
                    cout << "\033[1;36m{" << temp->data->getDest()->getName() 
                        << ", " << temp->data->getWeight() << "} \033[0m";
                    temp = temp->next;
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    Intersection* getIntersection(char A) {
        return intersections[A - 'A'];
    }

    void appendVehicle(Vehicle* vehicle) {
        DynamicArray<char> refs = dijkstraAlgo(vehicle->getStart(), vehicle->getEnd());
        DynamicArray<char> path = constructPath(refs, vehicle->getStart(), vehicle->getEnd());
        if(!path.empty()) {
            for (int i=0; i<path.size()-1; i++) {
                char c = path[i];
                char cNext= path[i+1];
                Road* road = nullptr;
                LinkedList<Road*>& edges = graph[c-'A'];
                LinkedList<Road*>::Node* edge = edges.getHead();

                while (edge) {
                    if (edge->data->getDest() -> getName() == cNext) {
                        road= edge->data;
                        break;
                    }
                    edge = edge->next;
                }

                if (road){
                    vehicle->addRoad(road);
                }
            }

            vehicle->setRoad();
            
            // inserting in hashmap
            string key="";
            key+=path[0];
            key+=path[1];
            if (roadVehicleMap.find(key)) {
                roadVehicleMap.search(key)->insertAtStart(vehicle);
            }
            else {
                LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                newList->insertAtStart(vehicle);
                roadVehicleMap.insert(key, newList);
            }
        }

    }

    void appendEV(Vehicle* vehicle) {
        vehicles.push_back(vehicle);

        DynamicArray<char> refs = aStarAlgo(vehicle->getStart(), vehicle->getEnd());
        DynamicArray<char> path = constructPath(refs, vehicle->getStart(), vehicle->getEnd());
        
        if (!path.empty() && path.size() > 1) {
            // Only process the first road in the path
            char currentNode = path[0];
            char nextNode = path[1];
            
            // Find the corresponding road
            Road* road = nullptr;
            LinkedList<Road*>& edges = graph[currentNode - 'A'];
            LinkedList<Road*>::Node* edge = edges.getHead();
            
            while (edge) {
                if (edge->data->getDest()->getName() == nextNode) {
                    road = edge->data;
                    break;
                }
                edge = edge->next;
            }
            
            if (road) {
                vehicle->addRoad(road);
                vehicle->setRoad();
                vehicle->setOld(vehicle->getStart()->getName());
                
                // Add to roadVehicleMap
                string key = "";
                key += currentNode;
                key += nextNode;
                
                if (roadVehicleMap.find(key)) {
                    roadVehicleMap.search(key)->insertAtStart(vehicle);
                } else {
                    LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                    newList->insertAtStart(vehicle);
                    roadVehicleMap.insert(key, newList);
                }
            }
        }
    }

    Road* findRoad(char start, char end) {
        LinkedList<Road*>::Node* edge = graph[start-'A'].getHead();
        while (edge) {
            if (edge->data->getDest()->getName() == end) {
                return edge->data;
            }
            edge = edge->next;
        }

        return nullptr;
    }

    void initialiseRoutes() {
        initialiseVehicles();
        initialiseEmergencyVehicles();
    }

    void clearQueues() {
        for (Intersection* intsc: intersections) {
            LinkedList<Road*>& edges = graph[intsc->getName() - 'A'];
            LinkedList<Road*>::Node* head = edges.getHead();

            while (head) {
                head->data->clearQueue();
                head = head->next;
            }
            
        }
    }

    bool getInitialised() {
        return initialised;
    }

    void setInitialised(bool param) {
        initialised = param;
    }

    bool hasVehiclesOnRoads() {
        for(Intersection* intsc : intersections){
            char name = intsc->getName();
            LinkedList<Road*>::Node* node = graph[name-'A'].getHead();
            while(node) {
                string key = "";
                key += name;
                key += node->data->getDest()->getName();
                if(roadVehicleMap.find(key)) {
                    LinkedList<Vehicle*>* temp_veh = roadVehicleMap.search(key);
                    if(temp_veh->getSize() > 0) {
                        return true;
                    }
                }
                node = node->next;
            }
        }
        return false;
    }

    void displayBlockedRoads() {
        cout << "\033[1;31m---------------Blocked Roads-------------------\033[0m\n";
        bool hasBlocked = false;
        
        for (Intersection* intsc : intersections) {
            LinkedList<Road*>& list = graph[intsc->getName()-'A'];
            for (LinkedList<Road*>::Node* node = list.getHead(); node != nullptr; node = node->next) {
                Road* rd = node->data;
                if (rd->getStatus() != "Clear") {
                    hasBlocked = true;
                    cout << "\033[1;31m" << intsc->getName() << " to " 
                        << rd->getDest()->getName() << " is blocked\033[0m" << endl;
                }
            }
        }
        
        if (!hasBlocked) {
            cout << "\033[1;32mNo blocked roads in the network\033[0m" << endl;
        }
        cout << endl;
    }

};

class Road;
class Vehicle;
class AdjacencyList;
class TrafficSignal;

class TrafficManagement {
    DynamicArray<Intersection*> intersection;
    DynamicArray<Road*> roads;
    DynamicArray<Vehicle*> vehicles;
    AdjacencyList graph;

public:
    void startSimulation(int duration = -1) {
        int time = 0;
        auto lastUpdate = steady_clock::now();
        graph.setSimultaionBool(true);
        if (!graph.getInitialised()) {
            graph.initialiseRoutes();
            graph.setInitialised(true);
        }
        while (true) {
            auto now = steady_clock::now();
            if (duration_cast<seconds>(now - lastUpdate).count() >= 1) { // 1 sec
                graph.updateSimulation();
                lastUpdate = now;
                time += 1;
            }

            this_thread::sleep_for(milliseconds(10));
            
            // will maybe cause probs if multiple calculations needed per second
            if (duration != -1 && time >= duration || graph.getSimulationBool() == false) {
                return;
            }
            else {
                if (!graph.hasVehiclesOnRoads()) return;
            }
        }
    }

    Intersection* getIntersection(char A) {
        return graph.getIntersection(A);
    }

    void displayCongestionStatus() {
        if (!graph.getInitialised()) {
            graph.initialiseRoutes();
            graph.setInitialised(true);
        }
        graph.display_Vehicles_at_Roads();
    }

    void displaySignalStatus() {
        graph.displaySignals();
    }

    void displayCityTrafficNetwork() { // adjacency list
        graph.displayGraph();
    }

    void displayBlockedRoads() {
        graph.displayBlockedRoads();
    }

    void displayPaths(){
        if (!graph.getInitialised()) {
            graph.initialiseRoutes();
            graph.setInitialised(true);
        }
        graph.displayVehiclePaths();
    }

    void blockRoad() {
        char start, end;
        cout << "Enter Start Intersection of the road to Block: ";
        cin >> start;
        cout << "Enter End Intersection of the road to Block: ";
        cin >> end;

        Road* road = graph.findRoad(start, end);
        if (road && road->getTrafficLoad() >0) {
            cout<<"Vehicles present in the road. Can't block it yetn\n";
        }
        if(road) {
            road->setStatus("Blocked");
            return;
        }
        cout << "Road Not Found!\n";
    }

    void addVehicle(Vehicle* veh, bool ev = 0) {
        if (!ev)
            graph.appendVehicle(veh);
        else
            graph.appendEV(veh);
    }
};

int main() {
    TrafficManagement obj;

    cout << "-----------------Simulation Dashboard------------------------\n";
    cout << "1. Display City Traffic Network\n";
    cout << "2. Display Traffic Signal Status\n";
    cout << "3. Display Congestions Status\n";
    cout << "4. Display Blocked Roads\n";
    cout << "5. Display Vehicle Paths\n";
    cout << "6. Block a Road\n";
    cout << "7. Add a Vehicle/Emergency Vehicle\n";
    cout << "8. Simulate Vehicle Routing\n";
    cout << "9. Exit Simulation\n";

    int choice;
    cin >> choice;
    string name;

    Intersection* s, *e;

    while (choice <= 8 && choice >= 1) {
        switch (choice) {
            case 1:
                obj.displayCityTrafficNetwork();
                break;
            case 2:
                obj.displaySignalStatus();
                break;
            case 3:
                obj.displayCongestionStatus();
                break;
            case 4:
                obj.displayBlockedRoads();
                break;
            case 5:
                obj.displayPaths();
                break;
            case 6:
                obj.blockRoad();
                break;
            case 7:
                cout << "Emergency Vehicle? (Y/N): ";
                char choice;
                cin >> choice;

                cout << "Enter Vehicle Name: ";
                cin >> name;

                char start, end;
                cout << "Enter Start and End Road: ";
                cin >> start;
                cin >> end;

                s = obj.getIntersection(start);
                e = obj.getIntersection(end);

                if (choice == 'y' || choice == 'Y') {
                    string priority;
                    cout << "Enter Vehicle priority: ";
                    cin >> priority;

                    EmergencyVehicle* ev = new EmergencyVehicle(name, s, e, priority);
                    obj.addVehicle(ev, 1);
                } else {
                    Vehicle* v = new Vehicle(name, s, e, 0);
                    obj.addVehicle(v);
                }
                break;
            case 8:
                cout << "Simulate for a Specific Time? (Y/N): ";
                char ch;
                cin >> ch;
                if (ch == 'y' || ch == 'Y') {
                    int t = 0;
                    cout << "Enter Time(sec): ";
                    cin >> t;
                    obj.startSimulation(t);
                } else {
                    obj.startSimulation();
                }
                break;
            case 9:
                cout << "Try again\n";
            default:
                cout << "\n-------------- Exiting --------------\n";
                return 0;
                break;
        }

        cout << "-----------------Simulation Dashboard------------------------\n";
        cout << "1. Display City Traffic Network\n";
        cout << "2. Display Traffic Signal Status\n";
        cout << "3. Display Congestions Status\n";
        cout << "4. Display Blocked Roads\n";
        cout << "5. Display Vehicle Paths\n";
        cout << "6. Block a Road\n";
        cout << "7. Add a Vehicle/Emergency Vehicle\n";
        cout << "8. Simulate Vehicle Routing\n";
        cout << "9. Exit Simulation\n";
        cin >> choice;
    }
}