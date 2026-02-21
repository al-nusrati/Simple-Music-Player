#pragma once
#include <iostream>
using namespace std;

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

    node<T>* getNewNode(T val) {
        node<T>* newNode = new node<T>;
        newNode->data = val;
        newNode->next = nullptr;
        newNode->prev = nullptr;
        return newNode;
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}
    ~DoublyLinkedList() { freeMemory(); }

    // Exposes head so Playlist can store a node<T>* directly — O(1) navigation
    node<T>* getHead() const { return head; }

    // Delete by pointer O(1) — used by removeByID after single search pass
    void deleteNode(node<T>* target) {
        if (!target) return;
        if (target == head) { deleteFromStart(); return; }
        if (target == tail) { deleteFromEnd();   return; }
        target->prev->next = target->next;
        target->next->prev = target->prev;
        delete target;
    }

    void insertAtBeginning(T val) {
        node<T>* n = getNewNode(val);
        if (!head) { head = tail = n; return; }
        n->next = head; head->prev = n; head = n;
    }

    void insertAtEnd(T val) {
        node<T>* n = getNewNode(val);
        if (!head) { head = tail = n; return; }
        tail->next = n; n->prev = tail; tail = n;
    }

    void insertAtAnyPos(int pos, T val) {
        if (pos <= 0 || pos > nodeCount() + 1) { cout << "Invalid position!\n"; return; }
        if (pos == 1)              { insertAtBeginning(val); return; }
        if (pos == nodeCount() + 1){ insertAtEnd(val);       return; }
        node<T>* n = getNewNode(val);
        node<T>* temp = head;
        for (int i = 1; i < pos - 1; i++) temp = temp->next;
        n->next = temp->next; n->prev = temp;
        if (temp->next) temp->next->prev = n;
        temp->next = n;
    }

    void deleteFromStart() {
        if (!head) return;
        node<T>* temp = head;
        if (head == tail) { head = tail = nullptr; }
        else { head = head->next; head->prev = nullptr; }
        delete temp;
    }

    void deleteFromEnd() {
        if (!head) return;
        node<T>* temp = tail;
        if (head == tail) { head = tail = nullptr; }
        else { tail = tail->prev; tail->next = nullptr; }
        delete temp;
    }

    int nodeCount() const {
        int count = 0;
        node<T>* temp = head;
        while (temp) { count++; temp = temp->next; }
        return count;
    }

    bool isEmpty() const { return (head == nullptr); }

    void freeMemory() {
        node<T>* temp;
        while (head) { temp = head; head = head->next; delete temp; }
        tail = nullptr;
    }
};