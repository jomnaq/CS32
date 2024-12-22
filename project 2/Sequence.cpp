// S2023: CS32 - Project 2
// Joanna Liu
// UID: 405-957-891

#include "Sequence.h"

#include <iostream>

using namespace std;

// Sequence class functions
Sequence::Sequence() { // constructor
    m_size = 0;
    head = new Node; // dummy node
    // pointers point back to head to implement a circular linked list
    head->m_next = head; 
    head->m_prev = head;
}

Sequence::~Sequence() { // destructor
    Node* deleter = head; 
    for (int i = 0; i < m_size; i++) {
        deleter = deleter->m_next;
        delete deleter->m_prev; // deletes the previous node
    }
    delete deleter; // deletes the last node
}

Sequence::Sequence(const Sequence& src) { // copy constructor
    // constructing new Sequence object for stuff to be copied into 
    m_size = 0;
    head = new Node;
    head->m_next = head;
    head->m_prev = head;

    ItemType getter;
    for (int i = 0; i < src.m_size; i++) { // adding nodes to the newly created sequence
        src.get(i, getter);
        this->insert(i, getter);
    }
}

Sequence& Sequence::operator=(const Sequence& src) { // assignment operator
    if (this != &src) {
        Sequence temp(src); // copies sequence to a new sequence using copy constructor
        this->swap(temp); // switches the old sequence to the new, copied constructor
    }
    return *this; 
}

bool Sequence::empty() const {
    if (m_size == 0)
        return true;
    return false;
}

int Sequence::size() const {
    return m_size;
}

int Sequence::insert(int pos, const ItemType& value) {
    if (pos < 0 || pos > m_size) // if position is out of range
        return -1;

    Node* findptr1 = head; // points to the location before where the new node should be inserted
    for (int i = 0; i < pos; i++) {
        findptr1 = findptr1->m_next;
    }

    Node* findptr2 = findptr1->m_next; // points to the location after where the new node should be inserted

    // dynamically allocating to make new node and inserting in correct position by rearranging pointers
    Node* newNode = new Node;
    newNode->m_data = value; // assigning the data in the node
    newNode->m_next = findptr1->m_next; // assigning m_next for new node
    findptr1->m_next = newNode; // assigning previous node's m_next to new node

    newNode->m_prev = findptr2->m_prev; // assigns m_prev for the new node
    findptr2->m_prev = newNode; // assigns next node's m_prev to new node

    m_size++;
    return pos;
}

int Sequence::insert(const ItemType& value) {
    Node* findptr = head->m_next;

    for (int i = 0; i < m_size; i++) { // traverse the linked list
        if (findptr->m_data < value) { // gets data value of node and checks if less than or equal to value
            findptr = findptr->m_next;
        }
        else { // when the item is bigger than value 
            insert(i, value); // insert the value at that position
            return i;
        }
    }
    // if inserted value is the largest so far of the list, should be inserted at the end
    return insert(m_size, value);
}

bool Sequence::erase(int pos) {
    if (pos < 0 || pos >= m_size) // if position is out of range
        return false;

Node* beforeptr = head; // points to the location before the node that will be deleted
for (int i = 0; i < pos; i++) {
    beforeptr = beforeptr->m_next;
}
Node* deleteptr = beforeptr->m_next; // points to the node that is going to be deleted 
Node* afterptr = deleteptr->m_next; // points to the node after the node that will be deleted

beforeptr->m_next = deleteptr->m_next;
afterptr->m_prev = deleteptr->m_prev;
delete deleteptr; // deletes the node

m_size--;
return true;
}

int Sequence::remove(const ItemType& value) {
    int removedCount = 0;
    Node* movingPtr = head->m_next;
    int originalSize = m_size;

    for (int i = 0; i < originalSize; i++) { // traverses the whole sequence
        if (movingPtr->m_data == value) { // checks if the value is found in the sequence
            movingPtr = movingPtr->m_next; // moves the movingPtr before the node is erased
            erase(i - removedCount); // gets rid of value at i-removedCount (b/c the sequence indexes change after something is erased)
            removedCount++; // to keep count for the return value
        }
        else
            movingPtr = movingPtr->m_next;
    }
    return removedCount;
}

bool Sequence::get(int pos, ItemType& value) const {
    if (pos < 0 || pos >= m_size)
        return false;

    Node* findptr = head->m_next; // for pointing to the node to be got
    for (int i = 0; i < pos; i++) {
        findptr = findptr->m_next;
    }
    value = findptr->m_data; // stores data into value
    return true;
}

bool Sequence::set(int pos, const ItemType& value) {
    if (pos < 0 || pos >= m_size) // if the position is not in the sequence
        return false;

    Node* findptr = head->m_next; // points to the node to be changed
    for (int i = 0; i < pos; i++) {
        findptr = findptr->m_next;
    }

    findptr->m_data = value; // changes the data value
    return true;
}

int Sequence::find(const ItemType& value) const {
    Node* findptr = head->m_next;
    for (int i = 0; i < m_size; i++) { // traversing the sequence
        if (findptr->m_data == value) // located the value in the sequence
            return i;
        else
            findptr = findptr->m_next;
    }
    return -1; // value not found in the sequence
}


void Sequence::swap(Sequence& other) {
    // swapping the size
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;

    // swapping the head pointers
    Node* tempptr = head;
    head = other.head;
    other.head = tempptr;
}

void Sequence::dump() const { // prints out sequence
    ItemType getter;
    for (int i = 0; i < m_size; i++) {
        get(i, getter);
        std::cerr << getter << " ";
    }
    std::cerr << endl << endl;
}

// Non-member functions
int subsequence(const Sequence& seq1, const Sequence& seq2) {
    if (seq1.size() < seq2.size() || seq1.empty() || seq2.empty()) { // if seq2 size > seq1 size, it's impossible for seq2 to "fit" into seq1
        return -1;
    }

    ItemType seq1getter;
    ItemType seq2getter;
    seq1.get(0, seq1getter); 
    seq2.get(0, seq2getter);

    int matchedPos = -1; // for returning where the sequences match up (if they do)
    int i = 0; // for parsing seq1  
    int j = 0; // for parsing seq2

    for (i = 0; i < seq1.size(); i++) {
        if (seq1getter == seq2getter) {  // found a match for seq1 and seq2
            matchedPos = i; // keeping track of where the match started
            for (j = 1; j <= seq2.size(); j++) { // comparing the values after the match
                if (seq1getter == seq2getter) { // as long as they are equal, keep going
                    seq1.get(i + j, seq1getter);
                    seq2.get(j, seq2getter);
                }
                else { // if the values are not equal anymore, break out of the for loop.
                    seq2.get(0, seq2getter); // resetting seq2getter
                    matchedPos = -1; // resetting matchedPos
                    break;
                }
            }            
        }
        if (j == seq2.size() + 1) { // if true, it means the for loop traversed fully and all of seq2 was found
            break; // so we get out of the current for loop traversing seq1.
        }
        seq1.get(i+1, seq1getter); // if seq2 hasn't matched seq1 yet, keep going through seq1
    }
    return matchedPos;
}

void concatReverse(const Sequence& seq1, const Sequence& seq2, Sequence& result) {
    Sequence* newResult = new Sequence;
    ItemType getter; 
        
    // add reversed seq1
    int seq1_size = seq1.size();
    int seq1_index = seq1_size - 1;

    for (int i = 0; i < seq1_size; i++) {
        seq1.get(seq1_index - i, getter); // gets values starting from the end of the seq1 to insert to result
        newResult->insert(i, getter); // inserts the gotten value at the end of the result sequence 
    }

    // add reversed seq2
    int seq2_size = seq2.size();
    int seq2_index = seq2_size - 1;
    int counter = 0;

    for (int i = seq1_size; i < seq2_size + seq1_size; i++) {
        seq2.get(seq2_index - counter, getter); // gets values starting from the end of the seq1 to insert to result
        newResult->insert(i, getter); // inserts the gotten value at the end of the result sequence 
        counter++;
    }

    // setting result to the new result array we made
    result.swap(*newResult);
    delete(newResult);

}

