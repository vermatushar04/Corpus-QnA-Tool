// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include "Node.h"
using namespace std;

class SearchEngine {
private:
    // You can add attributes/helper functions here

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    Node* search(string pattern, int& n_matches);

    /* -----------------------------------------*/
};

#define INIT_SIZE 99877

struct OldDictNode{
    long long count;
    string key;
    long hashVal;
    long secondaryHash;
    bool isDeletedNode;
    OldDictNode();
    OldDictNode(string key,long hashVal,long secondaryHash,long long occurences);
};
class OldDict{
private:
    // You can add attributes/helper functions here
    vector<OldDictNode> nodes;
    static unsigned int const primeArr[];
    float maxFillFactor;
    int primeNumberIndx;
    unsigned int distWords;
    unsigned int curCapacity;
public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    OldDict();

    ~OldDict();

    long long get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
    void insert_word(string& word,long long occurences);
    OldDictNode* findKey(string& word,long& hash,long& secondaryHash);
    OldDictNode* findKey(string& word);
    void resizeHashTable();
};