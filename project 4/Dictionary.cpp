#include "Dictionary.h"

#include <functional> // for std::hash for hash functions
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

void removeNonLetters(string &s);

class DictionaryImpl
{
public:
    DictionaryImpl(int maxBuckets);                           // constructor
    ~DictionaryImpl();                                        // destructor
    void insert(string word);                                 // insertting a word into the dictionary
    void lookup(string letters, void callback(string)) const; // looking up all anagrams made of letters

private:
    struct Item // nodes to hold the items in our hash table
    {
        string m_word; // the word itself
        Item *m_next;  // points to the next node
    };
    vector<Item *> dictionaryHash; // vector of pointers to linked list of items to represent a hash table
};

DictionaryImpl::DictionaryImpl(int maxBuckets) // constructor
{
    for (int i = 0; i < maxBuckets; i++)
    {
        dictionaryHash.push_back(nullptr); // fills our vector with nullptrs and makes it the size of maxBuckets (ensuring the smallest load factor)
    }
}

DictionaryImpl::~DictionaryImpl() // destructor
{
    for (int i = 0; i < dictionaryHash.size(); i++) // for every element in the vector
    {
        Item *deleter = dictionaryHash.at(i); // the pointer deleter traverses the vector to do the deleting
        while (deleter != nullptr)            // deletes the items (nodes) until there are no more left
        {
            Item *next = deleter->m_next; // a pointer as a temp marker for where to reassign deleter after deleting
            delete deleter;               // deletes item (node)
            deleter = next;               // reassigns deleter to next (temp marker)
        }
        dictionaryHash.at(i) = nullptr; // Set the pointer to nullptr after deleting the linked list
    }
}

void DictionaryImpl::insert(string word) // words are inserted based on their letters for fast anagram searching
{
    removeNonLetters(word); // cleaning word

    if (word.empty()) // if word is invalid
    {
        return;
    }

    string sortedWord = word;
    sort(sortedWord.begin(), sortedWord.end()); // sorts word so words made up of same letters are put in the same bucket

    unsigned int key = std::hash<string>()(sortedWord); // hashes the word
    int insertLocation = key % dictionaryHash.size();   // find the place in the vector to insert the word by using the key

    // create new item
    Item *newItem = new Item;
    newItem->m_word = word;

    // insert new item into beginning of linked list at proper bucket location
    newItem->m_next = dictionaryHash.at(insertLocation);
    dictionaryHash.at(insertLocation) = newItem;
}

void DictionaryImpl::lookup(string letters, void callback(string)) const // looks up anagrams based on letters
{
    // checking validity of arguments
    if (callback == nullptr)
        return;
    removeNonLetters(letters);
    if (letters.empty())
        return;

    // Finding proper bucket to search
    string sortedWord = letters;
    sort(sortedWord.begin(), sortedWord.end());         // sorting letters
    unsigned int key = std::hash<string>()(sortedWord); // hashes letters to get key
    int location = key % dictionaryHash.size();         // finds location of bucket

    // Finding anagrams within the bucket
    Item *current = dictionaryHash.at(location);
    while (current != nullptr) // traverses whole linked list
    {
        string currentWord = current->m_word;
        sort(currentWord.begin(), currentWord.end()); // sorts current word to be compared to letters

        if (currentWord == sortedWord) // they have all of the same chars = anagram (have to do so b/c collisions)
        {
            callback(current->m_word); // call the callback function on it
        }
        current = current->m_next;
    }
}

void removeNonLetters(string &s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end()); // chop everything off from "to" to end.
}

//******************** Dictionary functions ******************************

// These functions simply delegate to DictionaryImpl's functions
// You probably don't want to change any of this code

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters, callback);
}
