

#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Sequence s;
    ItemType getter;

    // Testing for an empty sequence
    assert(s.size() == 0);
    assert(s.empty());
    assert(s.remove("hello") == 0);
    assert(s.erase(10) == false);
    assert(s.set(0, "goodbye") == false);
    assert(s.get(10, getter) == false);
    assert(s.find("hello") == -1);

    // Adding items to the sequence
    assert(s.insert("cheese") == 0); // checking insert
    assert(s.insert("potato") == 1); // checking that insert without index inserts items in order
    assert(s.insert("potato") == 1); // checking when there is repeats of words it still inserts correctly
    assert(s.insert(0, "apple") == 0); // checking insert with index
    assert(s.insert(3, "banana") == 3); // checking insert at m_size adds item to the end
    assert(s.insert(6, "tomato") == -1); // should not insert because outside of sequence
    assert(s.size() == 5); // checks that size changed properly
    assert(s.empty() == false); // checks that empty works properly
    assert(s.get(2, getter) == true); // checking get() returns the right value when position is valid
    assert(getter == "potato"); // checking get() gets the correct value
    assert(s.get(6, getter) == false); // checking get() returns correct value when position is not valid
    assert(getter == "potato"); // checking getter isn't changed when position is out of the sequence
    assert(s.find("banana") == 3); // checking find() returns correctly when in sequence
    assert(s.find("tomato") == -1); // checking find() returns correctly when not in sequence

    // Removing items from the sequence
    assert(s.erase(4) == true); // checking erase when there are two of the same val
    assert(s.erase(2) == true); // checking erase when position is in bounds
    assert(s.erase(6) == false); // checking erase when position is not in bounds
    assert(s.insert(0, "banana") == 0); // adding a duplicate string to list to test remove
    assert(s.remove("banana") == 2); // checking that the return value for remove counts the removed items correctly
    assert(s.size() == 2); // making sure the bananas were removed from the sequence

    // Testing swap():
    Sequence t;
    t.swap(s); // swapping with an empty sequence
    assert(t.size() == 2); // making sure size is changed
    assert(s.size() == 0); // making sure size is changed
    // Making sure content in sequences changed:
    cerr << "Expected t: " << "apple cheese" << endl << "Actual t: ";
    t.dump();
    cerr << "Expected s: " << "" << endl << "Actual s: ";
    s.dump();

    // refilling up s
    for (int i = 0; i < 5; i++) {
        s.insert(i, "bread");
    } 

    t.swap(s); // swapping two filled lists
    cerr << "Expected t: " << "bread bread bread bread bread" << endl << "Actual t: ";
    t.dump();
    cerr << "Expected s: " << "apple cheese" << endl << "Actual s: ";
    s.dump();

    t.swap(t); // swapping with itself
    cerr << "Expected t: " << "bread bread bread bread bread" << endl << "Actual t: ";
    t.dump();
    
    // Testing copy constructor
    Sequence x = t; // copying a filled sequence
    assert(x.size() == t.size()); // checking size was copied correctly
    cerr << "Expected x: " << "bread bread bread bread bread" << endl << "Actual x: ";
    x.dump();
    Sequence y;
    Sequence z = y; // copying from an empty sequence
    assert(z.empty()); // checking that the copied sequence is empty as well
    cerr << "Expected z: " << "" << endl << "Actual z: ";
    z.dump();

    // Testing assignment operator
    x = y; // testing with empty sequence (x should be empty)
    assert(x.empty());
    x = t; // testing with a filled sequence 
    assert(x.size() == 5);
    
    // Test concatReverse()
    t.remove("bread"); // emptying old sequences so they can be used for new tests
    // adding more values into the sequences so they can be tested:
    s.insert(0, "pear"); // s = pear, apple, cheese
    t.insert("potato");
    t.insert("tomato");
    t.insert("yogurt");
    t.insert("butter"); // t = butter potato tomato yogurt
    
    concatReverse(s, t, y); // concatReverse() into an empty sequence
    assert(y.size() == 7);
    concatReverse(t, s, x); // concatReverse() into a filled sequence 
    assert(x.size() == 7);

    cerr << "Expected y: " << "cheese apple pear yogurt tomato potato butter" << endl << "Actual y: ";
    y.dump();
    cerr << "Expected x: " << "yogurt tomato potato butter cheese apple pear" << endl << "Actual x: ";
    x.dump();

    concatReverse(s, s, s); // concatReverse() into itself
    cerr << "Expected s: " << "cheese apple pear cheese apple pear" << endl << "Actual s: ";
    s.dump();

    // Test subsequence()
    s.erase(3);
    s.erase(3);
    s.erase(3); // s = cheese apple pear
    z.insert("bread");
    z.insert("omelette"); // z = bread omelette
    assert(subsequence(s, y) == -1); // when seq2 > seq1
    assert(subsequence(x, s) == 4); // when seq2 IS in seq1
    assert(subsequence(y, z) == -1); // when seq2 ISNT in seq1
    y.insert(0, "cheese");
    assert(subsequence(y, s) == 1); // when seq2 is in seq1 but not at the place where you first find seq2_0
    Sequence u;
    assert(subsequence(y, u) == -1); // when one of the sequences is empty
}

void test2() {
    Sequence go;
    assert(go.size() == 0); //tests size() for zero item list
    ItemType tester;
    assert(go.get(0, tester) == false); //tests get() for zero item list
    assert(go.set(0, "yay") == false); //tests set() for zero item list
    assert(go.find("a") == -1);     //tests find() for zero item list
    assert(go.erase(0) == false);   //tests erase() for zero item list
    assert(go.remove("blink") == 0); //tests remove() for zero item list
    assert(go.insert("apple") == 0);    //tests insert() for zero item list
    assert(go.erase(0) == true);        //test erase() for single item list
    assert(go.insert(0, "animal") == 0); //tests insert(pos,value) for zero item list
    assert(go.remove("animal") == 1);   //tests remove() for single item list
    assert(go.insert("yes") == 0);      //retests insert() for zero item list
    assert(go.size() == 1);         //tests size() for single item list
    assert(go.get(0, tester) == true); //tests get() for single item list
    assert(tester == "yes");           //tests that get() stores correct value
    assert(go.set(0, "yip") == true);    //tests set() for single item list
    assert(go.find("yip") == 0);        //tests find() for single item list
    assert(go.insert("adam") == 0);  //tests insert() for beginning of single item list
    assert(go.find("adam") == 0);       //tests that insert() worked correctly
    assert(go.remove("adam") == 1);
    assert(go.insert("zoom") == 1);     //tests insert() for end of single item list
    assert(go.remove("zoom") == 1);
    assert(go.insert(0, "atlas") == 0);    //tests insert(pos,value) for beginning of single item list
    assert(go.remove("atlas") == 1);
    assert(go.insert(1, "yz") == true);     //tests insert(pos,value) for end of single item list
    assert(go.insert("ball") == 0);      //tests insert() for filled sequence (beginning)
    assert(go.insert("game") == 1);      //tests insert() for filled sequence    (middle)
    assert(go.insert("mouse") == 2);     //tests insert() for filled sequence    (middle)
    assert(go.insert("zoo") == 5);       //tests insert() for filled sequence (end)
    assert(go.find("ball") == 0);           //tests that items have been inserted to correct position
    assert(go.find("game") == 1);
    assert(go.find("mouse") == 2);
    assert(go.find("yip") == 3);
    assert(go.find("yz") == 4);
    assert(go.find("zoo") == 5);
    assert(go.size() == 6);         //tests size() for filled list
    assert(go.get(0, tester) == true);   //tests get() for beginning of filled list
    assert(tester == "ball");
    assert(go.get(5, tester) == true);   //tests get() for end of list
    assert(tester == "zoo");
    assert(go.set(0, "craig") == true);  //tests set() for beginning of filled list
    assert(go.find("craig") == 0);      //tests that set() worked
    assert(go.set(5, "james") == true); //tests set() for end of filled list
    assert(go.find("james") == 5);      //tests that set() worked
    assert(go.erase(5) == true);         //tests erase() for end of filled list
    assert(go.remove("yz") == 1);      //test remove() for end of filled list
    assert(go.erase(0) == true);         //tests erase() for beginning of filled list
    assert(go.remove("game") == 1);     //tests remove() for beginnig of filled list
    assert(go.find("mouse") == 0);      //tests that remove() and erase() worked
    assert(go.find("yip") == 1);        //tests that remove() and erase() worked


    Sequence fin;
    assert(fin.insert("boo") == 0);
    assert(fin.insert("moo") == 1);

    assert(fin.erase(0) == true);       //tests erase() at beginning that makes list single item
    assert(fin.find("moo") == 0);

    assert(fin.insert("block") == 0);
    assert(fin.remove("block") == 1);   //tests remove() at beginning making list single item list
    assert(fin.find("moo") == 0);


    assert(fin.insert("noon") == 1);
    assert(fin.erase(1) == true);       //tests erase() at end that makes single item list
    assert(fin.find("moo") == 0);


    assert(fin.insert("nash") == 1);
    assert(fin.remove("nash") == 1);   //tests remove() at end that makes single item list
    assert(fin.find("moo") == 0);

    assert(fin.insert("bee") == 0);     //inserts "bee" to beginning of list
    assert(fin.insert(2, "bee") == 2);  //inserts "bee" to end of list
    assert(fin.insert("and") == 0);     //inserts "and" to beginning of list
    assert(fin.insert(4, "and") == 4);   //inserts "and" to end of list
    assert(fin.size() == 5);

    assert(fin.remove("and") == 2);     //tests remove() on items found at beginning and end of list
    Sequence copy = fin;
    assert(fin.remove("bee") == 2);     //tests remove() on items found at beginning and end of list making it a single item list
    copy = fin;
    assert(fin.find("moo") == 0);       //tests that pos has been updated for the single remaining item
    copy = fin;
}

void test3() {
    // default constructor
    Sequence s;

    // For an empty sequence:
    assert(s.size() == 0);             // test size
    assert(s.empty());                 // test empty
    assert(s.remove("test") == 0);  // nothing to remove, 0 returned

    // testing insertion with two arguments
    assert(s.insert(0, "lavash") == 0);
    assert(s.insert(0, "tortilla") == 0);
    assert(s.size() == 2); // test size

    // test insertion with one argument
    Sequence a;
    assert(a.insert("a") == 0);
    assert(a.insert("c") == 1);
    assert(a.insert("a") == 0);
    assert(a.insert("b") == 2);
    assert(a.insert("d") == 4 && a.size() == 5);
    a.dump();

    // copy constructor
    Sequence b(s);
    assert(b.size() == s.size());
    for (int i = 0; i < s.size(); i++)
    {
        ItemType valOfB;
        ItemType valOfS;
        assert(b.get(i, valOfB) && s.get(i, valOfS) && valOfB == valOfS); // testing get
    }
    Sequence c = s;
    assert(c.size() == s.size());
    for (int i = 0; i < s.size(); i++)
    {
        ItemType valOfC;
        ItemType valOfS;
        assert(c.get(i, valOfC) && s.get(i, valOfS) && valOfC == valOfS); // testing get
    }
    b.dump();
    c.dump();

    // assignment operator
    a = s = b; // supports chaining
    assert(a.size() == s.size());
    for (int i = 0; i < s.size(); i++)
    {
        ItemType valOfA;
        ItemType valOfS;
        assert(a.get(i, valOfA) && s.get(i, valOfS) && valOfA == valOfS); // testing get
    }
    a.dump();

    // test get
    ItemType x = "injera"; // test item type
    assert(s.get(0, x) && x == "tortilla"); // test get
    assert(s.get(1, x) && x == "lavash"); // test get

    // test set
    x = "new value";
    ItemType newValueSet;
    assert(s.set(0, x) && s.get(0, newValueSet) && newValueSet == "new value"); // test set
    assert(s.set(1, x) && s.get(1, newValueSet) && newValueSet == "new value"); // test set

    assert(!s.get(2, x) && !s.set(2, x)); // getting and setting out of bounds

    s.dump();

    // test erase
    // erasing out of bounds
    assert(s.size() == 2 && !s.erase(-4)); // test erase
    assert(s.size() == 2 && !s.erase(10)); // test erase
    // erasing within bounds
    bool trueErase = s.erase(0);
    assert(s.size() == 1 && trueErase); // test erase

    s.dump(); // one less value

    // test remove
    s.insert("random");
    s.insert("lol");
    s.insert("again");
    s.insert("random");
    s.insert("random");
    s.insert("more");
    s.insert("word");

    assert(s.remove("not in s") == 0 && s.remove("random") == 3);
    assert(s.size() == 5);

    // testing find
    s.insert("more words");
    s.insert("UCLA");
    s.insert("bruin");
    assert(s.find("not in s") == -1 && s.find("UCLA") == 0 && s.find("more") == 4);
    s.dump();

    // testing swap
    Sequence another;
    another.insert("testing");
    another.insert("testing 12");
    another.insert("testing 23");

    cerr << "s is \n";
    s.dump();

    cerr << "a is \n";
    a.dump();

    cerr << "another is \n";
    another.dump();

    s.swap(s); // aliasing
    s.swap(a);
    a.swap(another);

    cerr << "s is \n";
    s.dump();

    cerr << "a is \n";
    a.dump();

    cerr << "another is \n";
    another.dump();

    s.swap(another); // return s to value before swaps

    // testing subsequence
    Sequence subseq;
    subseq.insert("bruin");
    subseq.insert("lol");
    subseq.insert("more");
    // subseq is a subsequence of s
    assert(subsequence(s, subseq) == 2 && subsequence(s, a) == -1);
    assert(subsequence(s, s) == 0); // aliasing

    Sequence empty;
    assert(subsequence(s, empty) == -1);

    // testing concatreverse
    Sequence par;
    par.insert(0, "p");
    par.insert(1, "a");
    par.insert(2, "r");

    Sequence rot;
    rot.insert(0, "r");
    rot.insert(1, "o");
    rot.insert(2, "t");

    concatReverse(par, rot, s);
    s.dump();

    concatReverse(par, rot, par); // aliasing
    par.dump();

    concatReverse(s, s, s); // aliasing
    s.dump();

    concatReverse(empty, s, s);
    s.dump();

    s.swap(empty); // s is now empty

    concatReverse(rot, s, s);
    s.dump();

}
/*
void test4() 
{
    Sequence s;
    s.insert(0, 10);
    s.insert(0, 20);
    assert(s.size() == 2);
    s.insert(30);
    assert(s.size() == 3);

    ItemType x = 999;
    assert(s.get(0, x) && x == 20);
    assert(s.get(1, x) && x == 10);
    assert(s.set(0, 30));
    assert(s.get(0, x) && x == 30);

    Sequence t = s;
    ItemType y;
    assert(t.get(0, y) && y == 30);
    assert(t.get(1, y) && y == 10);
    assert(t.get(2, y) && y == 30);

    assert(t.insert(t.size(), 40) && t.get(3, y) && y == 40);
    assert(t.insert(20) == 0 && t.get(0, y) && y == 20);

    //test swap
    s.swap(t);
    assert(s.get(0, x) && x == 20);
    assert(s.get(1, x) && x == 30);
    assert(s.get(2, x) && x == 10);
    assert(s.get(3, x) && x == 30);
    assert(s.get(4, x) && x == 40);
    assert(s.get(5, x) != true && x == 40);

    //test erase
    s.erase(2);
    assert(s.get(0, x) && x == 20);
    assert(s.get(1, x) && x == 30);
    assert(s.get(2, x) && x == 30);
    assert(s.get(3, x) && x == 40);
    assert(!s.get(4, x) && x == 40);

    //test remove
    s.remove(30);
    assert(s.get(0, x) && x == 20);
    assert(s.get(1, x) && x == 40);

    //test subsequence
    Sequence seqBig;
    Sequence seqSmall;
    seqBig.insert(0, 30);
    seqBig.insert(1, 21);
    seqBig.insert(2, 63);
    seqBig.insert(3, 42);
    seqBig.insert(4, 17);
    seqBig.insert(5, 63);
    seqBig.insert(6, 17);
    seqBig.insert(7, 29);
    seqBig.insert(8, 8);
    seqBig.insert(9, 32);
    seqSmall.insert(0, 63);
    seqSmall.insert(1, 17);
    seqSmall.insert(2, 29);
    assert(subsequence(seqBig, seqSmall) == 5);
    assert(subsequence(seqSmall, seqBig) == -1);
    assert(subsequence(seqBig, s) == -1);
    assert(subsequence(seqSmall, seqSmall) == 0);

    //test that least subsequence is found
    Sequence seqTiny;
    seqTiny.insert(17);
    assert(subsequence(seqBig, seqTiny) == 4);

    //test invalid subsequence
    assert(subsequence(seqBig, s) == -1);

    //test concatReverse
    Sequence seq1;
    Sequence seq2;
    seq1.insert(1);
    seq1.insert(2);
    seq1.insert(3);
    seq2.insert(0, 4);
    seq2.insert(0, 5);
    seq2.insert(0, 6);

    Sequence oldseq1 = seq1;
    Sequence oldseq2 = seq2;

    Sequence seq3;
    concatReverse(seq1, seq2, seq3);
    assert(seq3.get(0, x) && x == 3);
    assert(seq3.get(1, x) && x == 2);
    assert(seq3.get(2, x) && x == 1);
    assert(seq3.get(3, x) && x == 4);
    assert(seq3.get(4, x) && x == 5);
    assert(seq3.get(5, x) && x == 6);
    assert(seq3.get(6, x) != true && x == 6);

    seq1 = oldseq1;
    seq2 = oldseq2;
    concatReverse(seq1, seq2, seq2);
    assert(seq2.get(0, x) && x == 3);
    assert(seq2.get(1, x) && x == 2);
    assert(seq2.get(2, x) && x == 1);
    assert(seq2.get(3, x) && x == 4);
    assert(seq2.get(4, x) && x == 5);
    assert(seq2.get(5, x) && x == 6);

    seq1 = oldseq1;
    seq2 = oldseq2;
    concatReverse(seq1, seq1, seq1);
    assert(seq1.get(0, x) && x == 3);
    assert(seq1.get(1, x) && x == 2);
    assert(seq1.get(2, x) && x == 1);
    assert(seq1.get(3, x) && x == 3);
    assert(seq1.get(4, x) && x == 2);
    assert(seq1.get(5, x) && x == 1);

    //test remove
    Sequence seq4;
    seq4.insert(4);
    seq4.insert(4);
    seq4.insert(1);
    seq4.insert(6);
    assert(seq4.get(0, x) && x == 1);
    assert(seq4.get(1, x) && x == 4);
    assert(seq4.get(2, x) && x == 4);
    assert(seq4.get(3, x) && x == 6);
    assert(!seq4.get(4, x) && x == 6); //nothing at 4
    assert(!seq4.empty());
    assert(seq4.remove(4) == 2);
    assert(seq4.remove(4) == 0); //no more 4s, should remove nothing
    assert(seq4.remove(1) == 1);
    assert(seq4.remove(6) == 1);
    assert(seq4.empty());
    assert(seq4.erase(3) == false); //invalid erase attempt

    Sequence seq5;
    seq5.insert(10);
    seq5.insert(10);
    seq5.insert(10);
    seq5.insert(10);
    seq5.insert(10);
    seq5.insert(10);
    assert(seq5.size() == 6);
    assert(seq5.find(10) == 0); //test find
    assert(seq5.find(23) == -1); //test invalid find
    assert(seq5.remove(10) == 6); //test complete removal
    assert(seq5.size() == 0 && seq5.empty());

    //test invalid set
    assert(seq5.set(32, 32) == false);
}*/

int main()
{
    test();
    test2();
    test3();
    cout << "Passed all tests" << endl;
}