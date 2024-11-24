#include <iostream>
using namespace std;

template <typename T>

class LinkedList {

private:

    struct Node {

        T data;
        Node* next;

        Node(T value) : data(value), next(nullptr) {}

    };

    Node* head; 
    int size;

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

public:

    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() {

        while (size > 0) {

            removeAtStart();

        }

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
    
};
+