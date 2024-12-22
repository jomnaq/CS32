// S2023: CS32 - Project 2
// Joanna Liu
// UID: 405-957-891

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string>

using ItemType = std::string;

class Sequence
{
public:
    Sequence();
    Sequence(const Sequence& src); // copy constructor
    Sequence& operator=(const Sequence& src); // assignment operator
    ~Sequence(); //destructor
    bool empty() const;
    int size() const;
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
    void dump() const; // prints out sequence
private:
    struct Node {
        Node* m_next;
        Node* m_prev;
        ItemType m_data;
    };

    Node* head;
    int m_size;    
};

// non member function 
int subsequence(const Sequence& seq1, const Sequence& seq2);
void concatReverse(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif
