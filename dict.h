// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
/*

*/
class PortlandStemmer{
public:
    vector<char> allCharacters;
    int charEndsAt;
    // void printWord(){
    //     for(int i=0;i<=charEndsAt;i++){
    //         cout << allCharacters[i];
    //     }
    //     cout << endl;
    // }
    void stemWord(){
        //cout << "Original word is";
        //printWord();
        Step1();
        Step2();
        Step3();
        Step4();
        Step5();
    }
    string getString(){
        return string(allCharacters.begin(),allCharacters.begin()+charEndsAt + 1);
    }
    void addWord(const string& word){
        allCharacters = vector<char>(word.size());
        for(int i=0;i<word.size();i++){
            allCharacters[i] = word[i];
        }
        charEndsAt = word.size() - 1;
    }
    bool isConsonant(int index){
        switch(allCharacters[index]){
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                return false;
            case 'y':
                if(index ==0){return true;}
                return isConsonant(index-1);
            default:
                return true;
        }
    }
    int findMeasure(int endIndex){
        if(endIndex < 0){
            return 0;
        }
        int firstVowel = endIndex + 1;
        for(int i=0;i<=endIndex;i++){
            if(!isConsonant(i)){
                firstVowel = i;
                break;
            }
        }   
        //cout << "first vowel is:" << firstVowel << endl;     
        int measure = 0;
        int numChanges = 0;
        bool prevIsVowel = true;
        for(int i=firstVowel;i<=endIndex;i++){
            bool curIsVowel = !isConsonant(i);
            //cout << "char is:" << allCharacters[i] << endl;
            if(curIsVowel){
                if(!prevIsVowel){
                    numChanges++;
                }
            }else{
                if(prevIsVowel){
                    numChanges++;
                }
            }
            //cout << "numChanes is:" << numChanges << endl;
            prevIsVowel = curIsVowel;
        }
        if(!isConsonant(endIndex)){
            if(numChanges != 0){
                numChanges--;
            }
        }
        measure = (numChanges+1)/2;
        return measure;
    }
    bool containsVowel(int endIndx){
        for(int i =0;i<= endIndx;i++){
            if(!isConsonant(i)){
                return true;
            }
        }
        return false;
    }
    bool endsWithCC(){
        if(charEndsAt >= 1){
            return isConsonant(charEndsAt) && (allCharacters[charEndsAt] == allCharacters[charEndsAt - 1]);
        }
        return false;
    }
    bool endsWithCVC(int endIndx){
        if(endIndx >= 2){
            bool C2 = isConsonant(endIndx) 
            && (allCharacters[endIndx] != 'w')   
            && (allCharacters[endIndx] != 'x')
            && (allCharacters[endIndx] != 'y');
            bool V = !isConsonant(endIndx - 1);
            bool C1 = isConsonant(endIndx - 2);
            return C1 && V && C2;
        }
        return false;
    }
    bool endsWith(const string& suffix){
        if(charEndsAt + 1 < suffix.length()){return false;}
        for(int i=0;i<suffix.length();i++){
            if(allCharacters[charEndsAt -i] != suffix[suffix.length() - 1 - i]){
                return false;
            }
        }
        return true;
    }

    void Step1(){
        if(endsWith("sses") || endsWith("ies")){
            charEndsAt = charEndsAt - 2;
        }else if(endsWith("ss")){
            charEndsAt = charEndsAt;
        }else if(endsWith("s")){
            charEndsAt = charEndsAt - 1;
        }
        if(endsWith("eed")){
            if(findMeasure(charEndsAt -3) > 0 ){
                charEndsAt = charEndsAt - 1;        
            }else if(endsWith("ed") && containsVowel(charEndsAt - 2)){

            }
        }else if(endsWith("ed") && containsVowel(charEndsAt - 2)){
            //cout << "remove two characters at end for:";
            //printWord();
            charEndsAt = charEndsAt - 2;
            //cout << "remove two charEndsAt is:" << charEndsAt << " last character is:" << allCharacters[charEndsAt] << endl;
            if(endsWith("at") || endsWith("bl") || endsWith("iz")){
                charEndsAt = charEndsAt + 1;
                allCharacters[charEndsAt] = 'e';
            }else if(endsWithCC() && 
                (allCharacters[charEndsAt] != 'l')&&
                (allCharacters[charEndsAt] != 's') && 
                (allCharacters[charEndsAt] != 'z')){
                charEndsAt = charEndsAt - 1;
                //cout << "remove one here" << endl;
                }else if(findMeasure(charEndsAt) == 1 && endsWithCVC(charEndsAt)){
                    charEndsAt = charEndsAt + 1;
                    allCharacters[charEndsAt] = 'e';
                }
        }
        if(endsWith("ing") && containsVowel(charEndsAt - 3)){
            charEndsAt = charEndsAt - 3;
            //cout << "remove three charEnds is:" << charEndsAt << " last char is:" << allCharacters[charEndsAt]<< endl;
            if(endsWith("at") || endsWith("bl") || endsWith("iz")){
                charEndsAt = charEndsAt + 1;
                allCharacters[charEndsAt] = 'e';
            }else if(endsWithCC() && 
            (allCharacters[charEndsAt] != 'l')&&
            (allCharacters[charEndsAt] != 's') && 
            (allCharacters[charEndsAt] != 'z')){
                charEndsAt = charEndsAt - 1;
                //cout << "remove one here" << endl;
            }else if(findMeasure(charEndsAt) == 1 && endsWithCVC(charEndsAt)){
                charEndsAt = charEndsAt + 1;
                allCharacters[charEndsAt] = 'e';
            }
        }
        if(allCharacters[charEndsAt] == 'y' && containsVowel(charEndsAt - 1)){
            allCharacters[charEndsAt] = 'i';
        }
        //cout << "word after step1 is:";
        //printWord();
    }
    void Step2(){
        bool measure7 = findMeasure(charEndsAt - 7) > 0;
        bool measure6 = findMeasure(charEndsAt - 6) > 0;
        bool measure5 = findMeasure(charEndsAt - 5) > 0;
        bool measure4 = findMeasure(charEndsAt - 4) > 0;
        //if(findMeasure() > 0){
            if(endsWith("ational") && measure7){
                //cout << "changing word for";
                //printWord();
                charEndsAt = charEndsAt -4;
                allCharacters[charEndsAt] = 'e';
            }else if( (endsWith("tional") && measure6) 
            || (endsWith("alli") && measure4 ) 
            || (endsWith("entli") && measure5 ) 
            || (endsWith("eli") && findMeasure(charEndsAt - 3)) 
            || (endsWith("ousli") && measure5 ) ){
                //cout << "changing word for";
                //printWord();
                //cout << endl;
                charEndsAt = charEndsAt -2;
            }else if( (endsWith("enci") && measure4) 
            || (endsWith("anci") && measure4)
            || (endsWith("abli") && measure4) ){
                allCharacters[charEndsAt] = 'e';
            }else if(endsWith("izer")&& measure4){
                charEndsAt = charEndsAt - 1;
            }else if(endsWith("ization") && measure7){
                charEndsAt = charEndsAt - 4;
                allCharacters[charEndsAt] = 'e';
            }else if(endsWith("ation") && measure5){
                charEndsAt = charEndsAt - 2;
                allCharacters[charEndsAt] = 'e';
            }else if(endsWith("ator") && measure4){
                charEndsAt = charEndsAt - 1;
                allCharacters[charEndsAt] = 'e';
            }else if((endsWith("alism") && measure5)
            || (endsWith("aliti") && measure5) ){
                charEndsAt = charEndsAt - 3;
            }else if((endsWith("iveness") && measure7)
            || (endsWith("fulness") && measure7)
            || (endsWith("ousness") && measure7)){
                charEndsAt = charEndsAt - 4;
            }else if(endsWith("aliti") && measure5){
                charEndsAt  = charEndsAt - 3;
            }else if(endsWith("iviti") && measure5){
                charEndsAt = charEndsAt - 2;
                allCharacters[charEndsAt] = 'e';
            }else if(endsWith("biliti") && measure6){
                charEndsAt = charEndsAt - 3;
                allCharacters[charEndsAt] = 'e';
                allCharacters[charEndsAt - 1] = 'l';
            }
        //}
        //cout << "After spte2 word is:" ;
        //printWord();
    }
    void Step3(){
        bool measure5 = findMeasure(charEndsAt - 5) > 0;
        bool measure4 = findMeasure(charEndsAt - 4) > 0;
        bool measure3 = findMeasure(charEndsAt - 3) > 0;
        if(endsWith("icate") && measure5){
            charEndsAt = charEndsAt - 3;
        }else if(endsWith("ative") && measure5){
            charEndsAt = charEndsAt - 5;
        }else if(endsWith("alize") && measure5){
            charEndsAt = charEndsAt - 3;
        }else if(endsWith("iciti") && measure5){
            charEndsAt = charEndsAt - 3;
        }else if(endsWith("ical") && measure4){
            charEndsAt = charEndsAt - 2;
        }else if(endsWith("ful") && measure3 ){
            charEndsAt = charEndsAt - 3;
        }else if(endsWith("ness") && measure4){
            charEndsAt = charEndsAt - 4;
        }
        //cout << "Word after step3 is:";
        //printWord();
    }
    void Step4(){
        bool measure2 = findMeasure(charEndsAt - 2) > 1;
        bool measure3 = findMeasure(charEndsAt - 3) > 1;
        bool measure4 = findMeasure(charEndsAt - 4) > 1;
        bool measure5 = findMeasure(charEndsAt - 5) > 1;
        if( ( endsWith("al") || endsWith("er") || endsWith("ic")
        || endsWith("ou"))
        && measure2){
            charEndsAt = charEndsAt - 2;
        }else if((endsWith("ance") ||endsWith("ence")
        ||endsWith("able") || endsWith("ible")
        ||endsWith("ment"))&& measure4){
            charEndsAt = charEndsAt - 4;
        }else if((endsWith("ant") || endsWith("ent")
        || endsWith("ism") || endsWith("ate")
        || endsWith("iti") || endsWith("ous")
        || endsWith("ive") || endsWith("ize")) && measure3){
            charEndsAt = charEndsAt - 3;
        }else if(endsWith("ion") && measure3 && (allCharacters[charEndsAt-3] == 's' || allCharacters[charEndsAt-3] == 't')){
            charEndsAt = charEndsAt - 3;
        }else if(endsWith("ement") && measure5){
            charEndsAt = charEndsAt - 5;
        }
        //cout << "Word after step4 is:";
        //printWord();
    }
    void Step5(){
        int measure1Val = findMeasure(charEndsAt - 1);
        if(measure1Val > 1 && (allCharacters[charEndsAt] == 'e')){
            //cout << "Entered 5.1" << endl;
            charEndsAt = charEndsAt - 1;
        }else if(measure1Val == 1 && (!endsWithCVC(charEndsAt - 1)) && allCharacters[charEndsAt] == 'e'){
            //cout << "Entered 5.2" << endl;
            charEndsAt = charEndsAt - 1;
        }else if(measure1Val > 1 && endsWithCC() && allCharacters[charEndsAt] == 'l'){
            //cout << "Entered 5.3" << endl;
            charEndsAt = charEndsAt - 1;
        }
        //cout << "Word after step5 is:";
        //printWord();
    }
};


using namespace std;

#define INIT_SIZE 99877 

struct OccurNode{
    int book_code;
    int page;
    int paragraph;
    int sentence_no;
    long long freq;
    //is the number of words passed not the number of characters passed
    int offset;
    OccurNode(int book_code,int page,int paragraph,int sentence_no,int offset)
    :book_code(book_code),page(page),paragraph(paragraph),sentence_no(sentence_no),offset(offset){
        freq = 1;
    }
};

struct DictNode{
    vector<OccurNode> occurences;    
    string key;
    long hashVal;
    long secondaryHash;
    bool isEmpty;
    long long numOccur;
    DictNode();
    DictNode(string key,long hashVal,long secondaryHash,OccurNode& firstOccur);
    void addOccurnece(int book_code,int page,int paragraph,int sentence_no,int offset){
        occurences.push_back(OccurNode(book_code,page,paragraph,sentence_no,offset));
    }
};
class ModifiedDict{
private:
    // You can add attributes/helper functions here
    vector<DictNode> nodes;
    static unsigned int const primeArr[];
    float maxFillFactor;
    int primeNumberIndx;
    unsigned int distWords;
    unsigned int curCapacity;
public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    ModifiedDict();

    ~ModifiedDict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
    void insert_word(string& word,int book_code,int page,int paragraph,int sentence_no,int offset);
    DictNode* findKey(string& word,long& hash,long& secondaryHash);
    DictNode* findKey(string& word);
    void resizeHashTable();
};

class Dict{
private:
    // You can add attributes/helper functions here
    vector<DictNode> nodes;
    static unsigned int const primeArr[];
    float maxFillFactor;
    int primeNumberIndx;
    unsigned int distWords;
    unsigned int curCapacity;
public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
    void insert_word(string& word,int book_code,int page,int paragraph,int sentence_no,int offset);
    DictNode* findKey(string& word,long& hash,long& secondaryHash);
    DictNode* findKey(string& word);
    void resizeHashTable();
};