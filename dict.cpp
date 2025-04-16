// Do NOT add any other includes
#include "dict.h"


DictNode::DictNode(){
    isEmpty = true;
    numOccur = -1;
}
DictNode::DictNode(string key,long hashVal,long secondaryHash,OccurNode& firstOccur):key(key){
    this->hashVal = hashVal;
    this->secondaryHash = secondaryHash;
    occurences.push_back(firstOccur);
    isEmpty = false;
    numOccur = 1;
}
unsigned int const ModifiedDict::primeArr[27] = {37, 67, 131, 257, 521, 1031, 2053, 4099, 8209, 16411, 32771, 65537, 
                                         131101, 262147, 524309, 1048583, 2097169, 4194319, 8388617, 16777259, 
                                         33554467, 67108879, 134217757, 268435459, 536870923, 1073741827, 
                                         2147483423};

const string puncMarks =
" .,-:!\"\'()?-[]“”‘’˙;@";




#define BASE 4861L
#define MOD 979921L
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

/*
long calculateHash(const string& id){
    long long hashvalue = 0;
    long long len = id.length();
    for(int i=0;i<len;i++)
    {
        hashvalue+=static_cast<int>(id[i]);
        hashvalue+= (hashvalue<<17);
        hashvalue^= (hashvalue<<5);
    }
    //hashvalue += (hashvalue<<13);
    hashvalue   ^= (hashvalue<<3);
    hashvalue   += (hashvalue<<1);
    return abs(hashvalue);
}
*/
/*
long calculateHash(string&& id){
    long long hashvalue = 0;
    long long len = id.length();
    for(int i=0;i<len;i++)
    {
        hashvalue+=static_cast<int>(id[i]);
        hashvalue+= (hashvalue<<17);
        hashvalue^= (hashvalue<<5);
    }
    //hashvalue+= (hashvalue<<13);
    hashvalue^= (hashvalue<<3);
    hashvalue+= (hashvalue<<1);
    return abs(hashvalue);
}
*/

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
ModifiedDict::ModifiedDict(){
    // Implement your function here    
    maxFillFactor = 0.1f;
    primeNumberIndx = 11;
    curCapacity =primeArr[primeNumberIndx];
    nodes = vector<DictNode>(curCapacity);
    distWords = 0;
}
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
ModifiedDict::~ModifiedDict(){}

//returns index of where next word could start
//automatically converts to lower case to
int findNextWord(int start,string& sentence,string& result){
    result = "";
    for(int i=start;i<sentence.size();i++){
        char curChr = sentence[i];
        if(!inPuncMarks(curChr)){
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

void ModifiedDict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    string curWord = "";
    int wordIndx = 0;
    int noWords = 0;
    while(wordIndx != sentence.size()){
        wordIndx = findNextWord(wordIndx,sentence,curWord);
        noWords++;
        if(curWord != ""){
            //cout << "Inserting word:" << curWord << endl;
            insert_word(curWord,book_code,page,paragraph,sentence_no,noWords);
       }
    }
}
//unsuccesful search may take a very long time
DictNode* ModifiedDict::findKey(string& word){
    long hash = calculateHash(word);
    long secondaryHash = calculateSecondaryHash(word);
    return findKey(word,hash,secondaryHash);
}
DictNode* ModifiedDict::findKey(string& word,long& hash,long& secondaryHash){
    unsigned int initIndx = hash%curCapacity;
    unsigned int indx = initIndx;
    unsigned int itreration = 0;
    unsigned int secondaryInc = secondaryHash%curCapacity;
    if(secondaryInc == 0){
        //this print statement should be added only to the insert function
        //cout << "Increment is zero should be rare" << endl;
        secondaryInc = 1;
    }
    while(true){
        //encountered an empty node
        if(nodes[indx].isEmpty){
            break;
        }
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
void ModifiedDict::insert_word(string& word,int book_code,int page,int paragraph,int sentence_no,int offset){
    
    //static bool prevIsDisq = false;
    //if(prevIsDisq){
        //cout << "inserting word:" << word << "distinct words is:" << distWords << endl;
    //    cout << word << endl;
    //}
    
    //if(word == "disqualification"){
        //cout << "inserting word:" << word << "distinct words is:" << distWords << endl;
        //cout << word << " ";
        //prevIsDisq = true;
    //}else{
        //prevIsDisq = false;
    //}
    PortlandStemmer stemmer;
    stemmer.addWord(word);
    word = stemmer.getString();
    long hash = calculateHash(word);
    long secondaryHash = calculateSecondaryHash(word);
    DictNode* node = findKey(word,hash,secondaryHash);
    if(node != nullptr){
        if(node->occurences.back().book_code == book_code && node->occurences.back().paragraph == paragraph && node->occurences.back().page == page && node->occurences.back().sentence_no == sentence_no){
            node->occurences.back().freq++;
        }
        else{
            node->addOccurnece(book_code,page,paragraph,sentence_no,offset);
        }
        node->numOccur++;
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
            if(nodes[indx].isEmpty){
                //TODO can optimize
                OccurNode firstOccur(book_code,page,paragraph,sentence_no,offset);
                nodes[indx] = DictNode(word,hash,secondaryHash,firstOccur);
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
void ModifiedDict::resizeHashTable(){
    if(primeNumberIndx == 26){return;}
    vector<DictNode> newArr = vector<DictNode>(curCapacity);
    for(int i =0;i<nodes.size();i++){
        DictNode& curNode = nodes[i];
        if(curNode.isEmpty){
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
            if(newArr[indx].isEmpty){
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
                // cout << "word count is:" << curNode.occurences.size() << endl;
                // throw runtime_error("Resizing table filled");
                break;
            }
        }
    }
    nodes = std::move(newArr);
}
long long ModifiedDict::get_word_count(string word){
    // Implement your function here  
    long hash = calculateHash(word);
    long secondaryHash = calculateSecondaryHash(word);
    DictNode* location = findKey(word,hash,secondaryHash);
    if(location == nullptr){
        return -1;
    }
    return location->numOccur;
}

void ModifiedDict::dump_dictionary(string filename){
    // Implement your function here  
    //cout << "called dump dictionary" << endl;
    fstream outFile;
    outFile.open(filename,ios::out);
    //delaing with endline for last times
    int distnctWordsHandl = 1;
    for(int i =0;i<curCapacity;i++){
        DictNode& curNode = nodes[i];
        if(!curNode.isEmpty){
            if(distnctWordsHandl == (distWords)){
                //include last end line if needed
                outFile << curNode.key << "," << curNode.numOccur;
            }else{
                outFile <<  curNode.key << "," << curNode.numOccur<< endl;
                distnctWordsHandl++;
            }
        }
    }
    outFile.close();
}

unsigned int const Dict::primeArr[27] = {37, 67, 131, 257, 521, 1031, 2053, 4099, 8209, 16411, 32771, 65537, 
                                         131101, 262147, 524309, 1048583, 2097169, 4194319, 8388617, 16777259, 
                                         33554467, 67108879, 134217757, 268435459, 536870923, 1073741827, 
                                         2147483423};

Dict::Dict(){
    // Implement your function here    
    maxFillFactor = 0.1f;
    primeNumberIndx = 11;
    curCapacity =primeArr[primeNumberIndx];
    nodes = vector<DictNode>(curCapacity);
    distWords = 0;
}

Dict::~Dict(){}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    string curWord = "";
    int wordIndx = 0;
    int noWords = 0;
    while(wordIndx != sentence.size()){
        wordIndx = findNextWord(wordIndx,sentence,curWord);
        noWords++;
        if(curWord != ""){
            //cout << "Inserting word:" << curWord << endl;
            insert_word(curWord,book_code,page,paragraph,sentence_no,noWords);
       }
    }
}
//unsuccesful search may take a very long time
DictNode* Dict::findKey(string& word){
    long hash = calculateHash(word);
    long secondaryHash = calculateSecondaryHash(word);
    return findKey(word,hash,secondaryHash);
}
DictNode* Dict::findKey(string& word,long& hash,long& secondaryHash){
    unsigned int initIndx = hash%curCapacity;
    unsigned int indx = initIndx;
    unsigned int itreration = 0;
    unsigned int secondaryInc = secondaryHash%curCapacity;
    if(secondaryInc == 0){
        //this print statement should be added only to the insert function
        //cout << "Increment is zero should be rare" << endl;
        secondaryInc = 1;
    }
    while(true){
        //encountered an empty node
        if(nodes[indx].isEmpty){
            break;
        }
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
void Dict::insert_word(string& word,int book_code,int page,int paragraph,int sentence_no,int offset){
    
    //static bool prevIsDisq = false;
    //if(prevIsDisq){
        //cout << "inserting word:" << word << "distinct words is:" << distWords << endl;
    //    cout << word << endl;
    //}
    
    //if(word == "disqualification"){
        //cout << "inserting word:" << word << "distinct words is:" << distWords << endl;
        //cout << word << " ";
        //prevIsDisq = true;
    //}else{
        //prevIsDisq = false;
    //}
    long hash = calculateHash(word);
    long secondaryHash = calculateSecondaryHash(word);
    DictNode* node = findKey(word,hash,secondaryHash);
    if(node != nullptr){
        if(node->occurences.back().book_code == book_code && node->occurences.back().paragraph == paragraph && node->occurences.back().page == page){
            node->occurences.back().freq++;
        }
        else{
            node->addOccurnece(book_code,page,paragraph,sentence_no,offset);
        }
        node->numOccur++;
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
            if(nodes[indx].isEmpty){
                //TODO can optimize
                OccurNode firstOccur(book_code,page,paragraph,sentence_no,offset);
                nodes[indx] = DictNode(word,hash,secondaryHash,firstOccur);
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
void Dict::resizeHashTable(){
    if(primeNumberIndx == 26){return;}
    vector<DictNode> newArr = vector<DictNode>(curCapacity);
    for(int i =0;i<nodes.size();i++){
        DictNode& curNode = nodes[i];
        if(curNode.isEmpty){
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
            if(newArr[indx].isEmpty){
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
                // cout << "word count is:" << curNode.occurences.size() << endl;
                // throw runtime_error("Resizing table filled");
                break;
            }
        }
    }
    nodes = std::move(newArr);
}
long long Dict::get_word_count(string word){
    // Implement your function here  
    long hash = calculateHash(word);
    long secondaryHash = calculateSecondaryHash(word);
    DictNode* location = findKey(word,hash,secondaryHash);
    if(location == nullptr){
        return -1;
    }
    return location->numOccur;
}
void Dict::dump_dictionary(string filename){
    // Implement your function here  
    //cout << "called dump dictionary" << endl;
    fstream outFile;
    outFile.open(filename,ios::out);
    //delaing with endline for last times
    int distnctWordsHandl = 1;
    for(int i =0;i<curCapacity;i++){
        DictNode& curNode = nodes[i];
        if(!curNode.isEmpty){
            if(distnctWordsHandl == (distWords)){
                //include last end line if needed
                outFile << curNode.key << "," << curNode.numOccur;
            }else{
                outFile <<  curNode.key << "," << curNode.numOccur<< endl;
                distnctWordsHandl++;
            }
        }
    }
    outFile.close();
}
