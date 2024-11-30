#pragma once

#include <iostream>
using namespace std;

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

    ~HashTable() {

        for (int i = 0; i < capacity; i++) {

            HashNode<K, V>* current = table[i];

            while (current) {

                HashNode<K, V>* temp = current;
                current = current->next;
                delete temp;

            }

        }

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


