#include <iostream>
using namespace std;

template <typename T>

class Vector {

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

    Vector() : data(nullptr), sz(0), cap(0) {}

    ~Vector() {

        delete[] data;

    }

    int size() const { // return size of vector

        return sz;

    }

    int capacity() const { // return max capacity of vector

        return cap;

    }

    bool empty() const { // return if vector is empty or not

        return sz == 0;

    }

    //Note: 
    //dummy used to handle error cases where index is out of range by returning a referencable object.
    //const functions are the same as the functions defined above them with the difference that the vector object is not modified in them, useful for const vector objects.

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

    T& front() { // return the first element in the vector

        if (empty()) {

            cout << "Vector is empty\n";
            static T dummy;
            return dummy;

        }

        return data[0];

    }

    const T& front() const {

        if (empty()) {

            cout << "Vector is empty\n";
            static T dummy;
            return dummy;

        }

        return data[0];

    }

    T& back() { // return the last element in the vector

        if (empty()) {

            cout << "Vector is empty\n";
            static T dummy;
            return dummy;

        }

        return data[sz - 1];

    }

    const T& back() const {

        if (empty()) {

            cout << "Vector is empty\n";
            static T dummy;
            return dummy;

        }

        return data[sz - 1];

    }

    void push_back(const T& value) { // insert element at end of vector

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


    void pop_back() { // remove element from end of vector

        if (empty()) {

            cout << "Vector is empty\n";
            return;

        }

        sz--;

    }

    void clear() { // clear the contents of the vector

        sz = 0;

    }

    void reserve(int newCap) { // allocate an amount of memory to the vector

        if (newCap > cap) {

            resizeCapacity(newCap);

        }

    }

};


