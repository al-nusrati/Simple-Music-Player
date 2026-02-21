#pragma once
#include <iostream>

template <typename T>
struct node {
    T data;
    node<T>* next;
    node<T>* prev;
};

template <typename T>
class DoublyLinkedList {
private:
    node<T>* head;
    node<T>* tail;
    int listSize; // The O(1) secret weapon

    node<T>* getNewNode(T val) {
        node<T>* newNode = new node<T>;
        newNode->data = val;
        newNode->next = nullptr;
        newNode->prev = nullptr;
        return newNode;
    }

public:
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
        listSize = 0;
    }

    ~DoublyLinkedList() {
        freeMemory();
    }

    // Expose head for the Playlist domain logic
    node<T>* getHead() const {
        return head;
    }

    void insertAtBeginning(T val) {
        node<T>* newNode = getNewNode(val);
        
        if(head == nullptr) { 
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        listSize++;
    }

    void insertAtEnd(T val) {
        node<T>* newNode = getNewNode(val);
        
        if(head == nullptr) {  
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        listSize++;
    }

    void insertAtAnyPos(int position, T val) {
        if(position <= 0 || position > listSize + 1) {
            std::cout << "Invalid position!\n";
            return;
        }
        
        if(position == 1) {
            insertAtBeginning(val);
            return;
        }
        
        if(position == listSize + 1) {
            insertAtEnd(val);
            return;
        }
        
        node<T>* newNode = getNewNode(val);
        node<T>* temp = head;
        
        for(int i = 1; i < position - 1; i++) {
            temp = temp->next;
        }
        
        newNode->next = temp->next;
        newNode->prev = temp;
        
        if(temp->next != nullptr) {
            temp->next->prev = newNode;
        }
        
        temp->next = newNode;
        listSize++;
    }

    void deleteFromStart() {
        if(isEmpty()) return;
        
        node<T>* temp = head;
        if(head == tail) {  
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        
        delete temp;
        listSize--;
    }

    void deleteFromEnd() {
        if(isEmpty()) return;
        
        node<T>* temp = tail;
        if(head == tail) { 
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        
        delete temp;
        listSize--;
    }

    void deleteAtAnyPos(int position) {
        if(isEmpty()) {
            std::cout << "List is empty!\n";
            return;
        }
        
        if(position <= 0 || position > listSize) {
            std::cout << "Invalid position!\n";
            return;
        }
        
        if(position == 1) {
            deleteFromStart();
            return;
        }
        
        if(position == listSize) {
            deleteFromEnd();
            return;
        }
        
        node<T>* temp = head;
        for(int i = 1; i < position; i++) {
            temp = temp->next;
        }
        
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        
        delete temp;
        listSize--;
    }

    // O(1) Complexity - No more loops!
    int nodeCount() const {
        return listSize;
    }

    bool isEmpty() const {
        return (head == nullptr);
    }

    void traverseForward() const {
        node<T>* temp = head;
        while(temp != nullptr) {
            std::cout << temp->data << "\n";
            temp = temp->next;
        }
    }

    void freeMemory() {
        node<T>* temp;
        while(head != nullptr) {
            temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        listSize = 0;
    }
};