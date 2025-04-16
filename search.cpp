// Do NOT add any other includes
#include "search.h"

SearchEngine::SearchEngine(){
    // Implement your function here  
}

SearchEngine::~SearchEngine(){
    // Implement your function here  
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    return;
}

Node* SearchEngine::search(string pattern, int& n_matches){
    // Implement your function here  
    return nullptr;
}

// Do NOT add any other includes

OldDictNode::OldDictNode(){
    isDeletedNode = false;
    count = -1;
}
OldDictNode::OldDictNode(string key,long hashVal,long secondaryHash,long long occurences):key(key){
    count = 1;
    this->hashVal = hashVal;
    this->secondaryHash = secondaryHash;
    isDeletedNode = false;
    this->count = occurences;
}
unsigned int const OldDict::primeArr[27] = {37, 67, 131, 257, 521, 1031, 2053, 4099, 8209, 16411, 32771, 65537, 
                                         131101, 262147, 524309, 1048583, 2097169, 4194319, 8388617, 16777259, 
                                         33554467, 67108879, 134217757, 268435459, 536870923, 1073741827, 
                                         2147483659};

const string puncMarks =
" .,-:!\"\'()?-[]“”‘’˙;@";




#define BASE 4861L
#define MOD 979921L
namespace OldDictHelper{
long calculateHash(const string& word){
    long hash = 0;
    for(int i=0;i<word.size();i++){
        unsigned char curChr = (unsigned char) word[i];
        if(word[i] < 0){
            // cout << "Negative character present" << endl;
        }
        hash = (hash*BASE + curChr)%MOD;
    }
    return hash;
}

long calculateHash(string&& word){
    long hash = 0;
    for(int i=0;i<word.size();i++){
        hash = (hash*BASE + word[i])%MOD;
    }
    return hash;
}
#define BASE2 8353L
#define MOD2 952297L
long calculateSecondaryHash(const string& word){
    long hash = 0;
    for(int i=0;i<word.size();i++){
        unsigned char curChr = (unsigned char) word[i];
        if(word[i] < 0){
            // cout << "Negative character present" << endl;
        }
        hash = (hash*BASE2 + curChr)%MOD2;
    }
    return hash;
}
long calculateSecondaryHash(string&& word){
    long hash = 0;
    for(int i=0;i<word.size();i++){
        hash = (hash*BASE2 + word[i])%MOD2;
    }
    return hash;
}
}
OldDict::OldDict(){
    // Implement your function here    
    maxFillFactor = 0.1f;
    primeNumberIndx = 11;
    curCapacity =primeArr[primeNumberIndx];
    nodes = vector<OldDictNode>(curCapacity);
    distWords = 0;
}
namespace OldDictHelper{
bool inPuncMarks(char curChr){
    switch(curChr){
        case '.':
        case ',':
        case '-':
        case ':':
        case '\"':
        case  '\'':
        case '!':
        case '(':
        case ')':
        case '?':
        case '[':
        case ']':
        case ';':
        case '@':
        case ' ':
            return true;
        default:
            return false;    
    }
}
}
OldDict::~OldDict(){}

//returns index of where next word could start
//automatically converts to lower case to
namespace OldDictHelper{
int findNextWord(int start,string& sentence,string& result){
    result = "";
    for(int i=start;i<sentence.size();i++){
        char curChr = sentence[i];
        if(!OldDictHelper::inPuncMarks(curChr)){
            if('A' <= curChr && curChr <= 'Z'){
                curChr = curChr | (1 << 5);
                //curChr = 'a' + (curChr - 'A');
            }
            result = result + curChr;
        }else{
            return i+1;
        }
    }
    return sentence.size(); 
}
};
//unsuccesful search may take a very long time
OldDictNode* OldDict::findKey(string& word){
    long hash = OldDictHelper::calculateHash(word);
    long secondaryHash = OldDictHelper::calculateSecondaryHash(word);
    return findKey(word,hash,secondaryHash);
} 
OldDictNode* OldDict::findKey(string& word,long& hash,long& secondaryHash){
    unsigned int initIndx = hash%curCapacity;
    unsigned int indx = initIndx;
    unsigned int itreration = 0;
    unsigned int secondaryInc = secondaryHash%curCapacity;
    if(secondaryInc == 0){
        //this print statement should be added only to the  function
        //cout << "Increment is zero should be rare" << endl;
        secondaryInc = 1;
    }
    while(true){
        //encountered an empty node
        if(nodes[indx].count == -1){break;}
        if(nodes[indx].hashVal == hash){
            if(nodes[indx].key == word){
                return &nodes[indx];
            }
        }
        indx = (indx + (itreration + 1)*secondaryInc)%curCapacity;
        itreration++;
        if(indx == initIndx){ break; }
    }
    return nullptr;
}
void OldDict::insert_word(string& word,long long occurences){    
    long hash = OldDictHelper::calculateHash(word);
    long secondaryHash = OldDictHelper::calculateSecondaryHash(word);
    OldDictNode* node = findKey(word,hash,secondaryHash);
    if(node != nullptr){
        //node->count++;
        node->count = node->count + occurences;
        //if(node->key == "disqualification"){
        //    cout << "node->count is:" << node->count << endl;
        //}
    }else{
        distWords++;
        float newFillFactor = ((float) distWords)/((float) curCapacity);
        if(newFillFactor > maxFillFactor){
            primeNumberIndx++;
            curCapacity = primeArr[primeNumberIndx];
            resizeHashTable();
        }    
        unsigned int initIndx = hash%curCapacity;
        unsigned int indx = initIndx;
        unsigned int iteration = 0;
        unsigned int secondaryIncr = secondaryHash%curCapacity;
        if(secondaryIncr == 0){
            //cout << "Increment is zero should be a rare occurence for larger table size more common for smaller table size" << endl;
            secondaryIncr = 1;
        }
        while(true){
            if(nodes[indx].count == -1){
                nodes[indx] = OldDictNode(word,hash,secondaryHash,occurences);
                break;
            }   
            indx = (indx + (iteration + 1)*secondaryIncr)% curCapacity;
            iteration++;
            if(indx == initIndx){
                //with secondary hashing cannot guarantee that for half filled table space would be present
                // cout << "Error table should have been large enough insertion failed" << endl;
                break;
            }
        }
    }
}
void OldDict::resizeHashTable(){
    vector<OldDictNode> newArr = vector<OldDictNode>(curCapacity);
    for(int i =0;i<nodes.size();i++){
        OldDictNode& curNode = nodes[i];
        if(curNode.count == -1){
            continue;
        }
        long hash = curNode.hashVal;
        long secondaryHash = curNode.secondaryHash;
        unsigned int initIndx = hash%curCapacity;
        unsigned int indx = initIndx;
        unsigned int iteration = 0;
        unsigned int secondaryIncr = secondaryHash%curCapacity;
        if(secondaryIncr == 0){
            secondaryIncr = 1;
        }
        while(true){
            if(newArr[indx].count == -1){
                //try this out later with std::move and see if it makes a difference shouldn't ideally but can't be sure
                newArr[indx] = curNode;
                break;
            }
            indx  = (indx + (iteration + 1)*secondaryIncr)%curCapacity;
            iteration++;
            if(indx == initIndx){
                // cout << "Error should have had enough space for resizing and filling clearly some problem here" << endl;
                // cout << "iterations is:" << iteration << endl;
                // cout << "curCapacity is:" << curCapacity << endl;
                // cout << "initIndx is:" << initIndx << endl;
                // cout << "hash is:" << hash << endl;
                // cout << "secondaryHash is:" << secondaryHash << endl;
                // cout << "secondaryIncr is:" << secondaryIncr << endl;
                // cout << "word is:" << curNode.key  << "word size is:" << curNode.key.size() << endl;
                // cout << "word count is:" << curNode.count << endl;
                // throw runtime_error("Resizing table filled");
                break;
            }
        }
    }
    nodes = std::move(newArr);
}
long long OldDict::get_word_count(string word){
    // Implement your function here  
    long hash = OldDictHelper::calculateHash(word);
    long secondaryHash = OldDictHelper::calculateSecondaryHash(word);
    OldDictNode* location = findKey(word,hash,secondaryHash);
    if(location == nullptr){
        return -1;
    }
    return location->count;
}
/*
void OldDict::dump_dictionary(string filename){
    // Implement your function here  
    //cout << "called dump dictionary" << endl;
    fstream outFile;
    outFile.open(filename,ios::out);
    //delaing with endline for last times
    int distnctWordsHandl = 1;
    for(int i =0;i<curCapacity;i++){
        OldDictNode& curNode = nodes[i];
        if(curNode.count != -1){
            if(distnctWordsHandl == (distWords)){
                //include last end line if needed
                outFile << curNode.key << "," << curNode.count;
            }else{
                outFile <<  curNode.key << "," << curNode.count << endl;
                distnctWordsHandl++;
            }
        }
    }
    outFile.close();
}
*/