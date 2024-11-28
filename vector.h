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

    int size() const {

        return sz;

    }

    int capacity() const {

        return cap;

    }

    bool empty() const {

        return sz == 0;

    }

};


