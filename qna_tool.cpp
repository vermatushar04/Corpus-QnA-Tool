#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;
class HeapNode
{
public:
    Node* node;
    float score;

    HeapNode()
    {
        node = nullptr;
        score = 0;
    }

    HeapNode(Node* nod, float scr)
    {
        node = nod;                   //to be checked
        score = scr;
    }
};

class Heap
{
public:
    vector<HeapNode> heap;
    int k;
    Heap(int k){
        this->k = k;
    }
    int get_parent(int i)
    {
        return (i - 1) / 2;
    }

    int get_left(int i)
    {
        return 2 * i + 1;
    }

    int get_right(int i)
    {
        return 2 * i + 2;
    }

    void heapifyDown(int i)
    {
        int leftChild = get_left(i);
        int rightChild = get_right(i);
        if (leftChild >= heap.size()) return;
        int minIndex = i;
        if (heap[minIndex].score > heap[leftChild].score) minIndex = leftChild;

        if (rightChild<heap.size() && heap[minIndex].score>heap[rightChild].score) minIndex = rightChild;

        if (minIndex != i)
        {
            swap(heap[minIndex], heap[i]);
            heapifyDown(minIndex);
        }
    }

    //diff from above function as it takes the size of heap as a parameter
    //used for HeapSort
    void heapifyDown(int i, int N)
    {
        int leftChild = get_left(i);
        int rightChild = get_right(i);
        if (leftChild >= N) return;
        int minIndex = i;
        if (heap[minIndex].score > heap[leftChild].score) minIndex = leftChild;

        if (rightChild<N && heap[minIndex].score>heap[rightChild].score) minIndex = rightChild;

        if (minIndex != i)
        {
            swap(heap[minIndex], heap[i]);
            heapifyDown(minIndex, N);
        }
    }

    void buildHeap(const std::vector<HeapNode> array)
    {
        heap = array;
        int N = heap.size();

        for (int i = N / 2 - 1; i >= 0; i--)
        {
            heapifyDown(i);
        }
    }
    Node* get_min()
    {
        //not checking if the size is at least 1 or not
        return heap[0].node;
    }
    bool empty(){
        return (heap.size() == 0);
    }


    void replace_min(int bookCode,int pageCode,int paraCode, float score)
    {
        if(score < heap[0].score){return;}
        heap[0].node->book_code = bookCode;
        heap[0].node->paragraph = paraCode;
        heap[0].node->page = pageCode;
        heap[0].score = score;
        heapifyDown(0);
    }

    int get_size()
    {
        return heap.size();
    }
    void heapifyUp(int i){
        if(i == 0){return;}
        if(heap[get_parent(i)].score > heap[i].score){
            swap(heap[get_parent(i)],heap[i]);
            heapifyUp(get_parent(i));
        }
    }
    void insert(int bookCode,int pageNumber,int paraCode, float score){
        if(heap.size() == k){
            replace_min(bookCode,pageNumber,paraCode,score);
            return;
        }
        Node* node = new Node(bookCode,pageNumber,paraCode,-1,-1);
        node->right = nullptr;
        node->left = nullptr;
        heap.push_back(HeapNode(node,score));
        heapifyUp(heap.size()-1);
    }


    void deleteMin(){
        swap(heap[0],heap[heap.size()-1]);
        heap.pop_back();
        heapifyDown(0);
    }

    // void printScore(){
    //     for(int i = 0; i < heap.size(); ++i){
    //         cout << heap[i].node->book_code <<  ' ' << heap[i].node->page << ' '  <<  heap[i].node->paragraph << ' ' << heap[i].score << endl;
    //     }
    // }

};

//reads from CSV file and returns the frequency of word in a general corpus
//should it be long long int?
//is there a better search possible?


//takes in string and vector as argument
//returns vector of string which contains words of input string
//in small letters and without any symbols

//END


#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"
#define CSV_FILE "unigram_freq.csv"
#define INT_MAX 2147483647
#define BASE_PATH "mahatma-gandhi-collected-works-volume-"

vector<string> fileNames;
// void printVector(vector<float>& scores){
//     for(int i=0;i<scores.size();i++){
//         cout << scores[i] << ",";
//     }
//     cout << endl;
// }
// void printLinkedList(Node* head){
//     Node* curNode = head;
//     while(curNode){
//         cout << "(" << curNode->book_code << "," << curNode->page << "," << curNode->paragraph << ")" << endl;
//         curNode = curNode->right;
//     }
// }
string numToString(int x){
    string result = "";
    if(x == 0){
        return "0";
    }
    while(x){
        result =  (char)('0' + (x%10)) + result;
        x = x/10;
    }
    return result;
}
QNA_tool::QNA_tool(){
    initStemmed = false;
    
    for(int i=1;i<=98;i++){
        fileNames.push_back(BASE_PATH+numToString(i)+".txt");
    }

    // Implement your function here  
    ifstream csvFile(CSV_FILE,ios::in); 
    string word;
    string occ;
    getline(csvFile,word);
    while (std::getline(csvFile,word,',' ) && std::getline(csvFile, occ)) {
        long long tempOcc = stoll(occ);
        d.insert_word(word,tempOcc);
    }
    csvFile.close();
    /*
    for(string& fileName:fileNames){
        ifstream inputFile(fileName,ios::in);
        string tuple;
        string sentence;
        vector<int> metadata(4,0);
        while(getline(inputFile,tuple,')') && getline(inputFile,sentence)){
            tuple += ')';
            //int metadata[4];
            std::istringstream iss(tuple);
            std::string token;
            iss.ignore(1);
            int idx = 0;
            while (std::getline(iss, token, ',')) {
                // Trim leading and trailing white spaces
                size_t start = token.find_first_not_of(" ");
                size_t end = token.find_last_not_of(" ");
                if (start != std::string::npos && end != std::string::npos) {
                    token = token.substr(start, end - start + 1);
                }
               s
                // Check if the element is a number or a string
                if (token[0] == '\'') {
                    // Remove the single quotes and convert to integer
                    int num = std::stoi(token.substr(1, token.length() - 2));
                    metadata[idx] = num;
                } else {
                    // Convert the element to integer
                    int num = std::stoi(token);
                    metadata[idx] = num;
                }
                idx++;
            }
            allNotStemData.insert_sentence(metadata[0],metadata[1],metadata[2],metadata[3],sentence);
        }
    }
    */
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
}

//also important
void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    allNotStemData.insert_sentence(book_code,page,paragraph,sentence_no,sentence);
    return;
}
namespace QnaHelper{
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
};
namespace QnaHelper{
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
};

//shouldn't have modified original version should have created a different version of the function instead
vector<string> findAllWords(string& question){
    int wordIndx = 0;
    string curWord;
    vector<string> result;
    while(wordIndx != question.size()){
        wordIndx = QnaHelper::findNextWord(wordIndx,question,curWord);
        if(curWord != ""){
            result.push_back(curWord);
        }
    }
    return result;
}

vector<string> extraStopWords = {"gandhi","mahatma"};

vector<string> findAllWordsNoStop(string& question){
    PortlandStemmer stemmer;
    int wordIndx = 0;
    string curWord;
    vector<string> result;
    while(wordIndx != question.size()){
        wordIndx = QnaHelper::findNextWord(wordIndx,question,curWord);
        if(curWord != ""){
            stemmer.addWord(curWord);
            string tempWord = stemmer.getString();
            bool shouldAdd = true;
            for(string& word:extraStopWords){
                if(tempWord == word){
                    shouldAdd = false;
                    break;
                }
            }
            if(shouldAdd){
                result.push_back(stemmer.getString());
            }
        }
    }
    return result;
}

int getBookCode(DictNode* node,int occurPtr){
    if(node == nullptr){
        return INT_MAX;
    }
    if(occurPtr >= node->occurences.size()){
        return INT_MAX;
    }
    int book_code = node->occurences[occurPtr].book_code;
    return book_code;
}
int getPageCode(DictNode* node,int occurPtr,int bookCode){
    if(node == nullptr){
        return INT_MAX;
    }
    if(bookCode == INT_MAX){
        return INT_MAX;
    }
    if(occurPtr >= node->occurences.size()){
        return INT_MAX;
    }
    if(node->occurences[occurPtr].book_code != bookCode){
        return INT_MAX;
    }
    int pageCode = node->occurences[occurPtr].page;
    return pageCode;
}
int getParaCode(DictNode* node,int occurPtr,int bookCode,int pageCode){
    if(node == nullptr){
        return INT_MAX;
    }
    if(bookCode == INT_MAX || pageCode == INT_MAX){
        return INT_MAX;
    }
    if(occurPtr >= node->occurences.size()){
        return INT_MAX;
    }
    if(node->occurences[occurPtr].book_code != bookCode || node->occurences[occurPtr].page != pageCode){
        return INT_MAX;
    }
    int paragraph = node->occurences[occurPtr].paragraph;
    return paragraph;
}

void getParagraphEval(vector<DictNode*>& wordNodes,vector<int>& occurPtrs,int& minBook,int& minPages,int& minPara){
    int min_book_code = INT_MAX;
    for(int i=0;i<occurPtrs.size();i++){
        min_book_code = min(min_book_code,getBookCode(wordNodes[i],occurPtrs[i]));
    }
    int min_page_code = INT_MAX;
    for(int i=0;i<occurPtrs.size();i++){
        min_page_code = min(min_page_code,getPageCode(wordNodes[i],occurPtrs[i],min_book_code));
    }
    int min_paragraphCode = INT_MAX;
    //int currInd = 0;
    //for(int i=0;i<occurPtrs.size();i++){
    //    int gpc = getParaCode(wordNodes[i],occurPtrs[i],min_book_code);
    //    if(gpc < min_paragraphCode){
    //        currInd = i;
    //        min_paragraphCode = gpc;
    //    }
    //}
    for(int i=0;i<occurPtrs.size();i++){
        min_paragraphCode = min(getParaCode(wordNodes[i],occurPtrs[i],min_book_code,min_page_code),min_paragraphCode);
    }  
    minBook = min_book_code;
    minPages = min_page_code;
    minPara = min_paragraphCode;
}

void getBookPageParaSent(DictNode* n,int occur,int& bookCode,int& pageCode,int & paraCode,int & sentCode){
    if(n == nullptr){bookCode = INT_MAX; paraCode = INT_MAX; pageCode = INT_MAX;return;}
    if(n->occurences.size() <= occur){
        bookCode = INT_MAX; paraCode = INT_MAX; pageCode = INT_MAX; return;
    }
    bookCode = n->occurences[occur].book_code;
    pageCode = n->occurences[occur].page;
    paraCode = n->occurences[occur].paragraph;
    sentCode = n->occurences[occur].sentence_no;
}
//vector<string> skippedWords = {"mahatma","gandhi"};
// void printVector(vector<string>& arr){
//     for(int i=0;i<arr.size();i++){
//         cout << arr[i] << " ";
//     }
//     cout << endl;
// }

void getBookPagePara(DictNode* n,int occur,int& bookCode,int& pageCode,int & paraCode){
    if(n == nullptr){bookCode = INT_MAX; paraCode = INT_MAX; pageCode = INT_MAX;return;}
    if(n->occurences.size() <= occur){
        bookCode = INT_MAX; paraCode = INT_MAX; pageCode = INT_MAX; return;
    }
    bookCode = n->occurences[occur].book_code;
    pageCode = n->occurences[occur].page;
    paraCode = n->occurences[occur].paragraph;
}

Node* QNA_tool::get_top_k_para(string question,int k){
    vector<string> allWords = findAllWords(question);
    vector<long long> noOccurInEng(allWords.size());
    vector<long long> noOccurInCorp(allWords.size());
    vector<DictNode*> wordNodes(allWords.size());
    vector<int> occurPtrs(allWords.size());
    Heap h(k);
    vector<float> wordScores(allWords.size());
    for(int i=0;i<allWords.size();i++){
        DictNode* curNode = allNotStemData.findKey(allWords[i]);
        OldDictNode* csvNode = d.findKey(allWords[i]);
        if(csvNode){
            noOccurInEng[i] = csvNode->count;
        }else{
            noOccurInEng[i] = 0;
        }
        if(curNode){
            noOccurInCorp[i] = curNode->numOccur;
        }
        else{
            noOccurInCorp[i] = 0;
        }
        wordNodes[i] = curNode;
        if(curNode == nullptr){
            occurPtrs[i] = -1;
        }else{
            occurPtrs[i] = 0;
        }
        wordScores[i] = ((float)(noOccurInCorp[i] + 1))/((float)(noOccurInEng[i] + 1));
    }
    int curPara,curPage,curBook;
    float maxParaVal = -12;
    getParagraphEval(wordNodes,occurPtrs,curBook,curPage,curPara);
    while(curBook != INT_MAX){
        float paraVal = 0;
        for(int i = 0 ; i < allWords.size(); ++i){
            int tempBookCode,tempPageCode,tempParaCode;
            getBookPagePara(wordNodes[i],occurPtrs[i],tempBookCode,tempPageCode,tempParaCode);
            if(tempBookCode != curBook || tempParaCode != curPara || tempPageCode != curPage){continue;}
            paraVal += wordScores[i]*wordNodes[i]->occurences[occurPtrs[i]].freq;
            occurPtrs[i]++;
            //cout << "Entered loop at some point" << endl;
        }
        if(paraVal > maxParaVal){
            maxParaVal = paraVal;
        }
        h.insert(curBook,curPage,curPara,paraVal);
        getParagraphEval(wordNodes,occurPtrs,curBook,curPage,curPara);
    }
    Node* head = h.get_min();
    h.deleteMin();
    int numTerms = 0;
    while(!h.empty()){
        numTerms++;
        head->left = h.get_min();
        //cout << "head->right is:" <<"(" << h.get_min()->book_code << "," << h.get_min()->page << "," << h.get_min()->paragraph << ")" << endl;
        head->left->right = head;
        head = head->left;
        h.deleteMin();
    }
    //cout << "numTerms:" << numTerms << endl;
    return head;
}

Node* QNA_tool::get_top_k_para_modified(string question, int k) {
    // Implement your function here

    vector<string> allWords = findAllWordsNoStop(question);
    // cout << "the words array is:" << endl;
    // printVector(allWords);
    vector<long long> noOccurInEng(allWords.size());
    vector<long long> noOccurInCorp(allWords.size());
    vector<DictNode*> wordNodes(allWords.size());
    vector<int> occurPtrs(allWords.size());
    Heap h(k);
    vector<float> wordScores(allWords.size());
    for(int i=0;i<allWords.size();i++){
        DictNode* curNode = allData.findKey(allWords[i]);
        OldDictNode* csvNode = d.findKey(allWords[i]);
        if(csvNode){
            noOccurInEng[i] = csvNode->count;
        }else{
            noOccurInEng[i] = 0;
        }
        if(curNode){
            noOccurInCorp[i] = curNode->numOccur;
        }
        else{
            noOccurInCorp[i] = 0;
        }
        wordNodes[i] = curNode;
        if(curNode == nullptr){
            occurPtrs[i] = -1;
        }else{
            occurPtrs[i] = 0;
        }
        float denom = noOccurInEng[i] + 1;
        //denom = denom*denom*denom*denom;
        float num = noOccurInCorp[i] + 1;
        //float tempVal = num/denom;
        float tempVal = 1000000000/(num*denom);
        //wordScores[i] = tempVal*tempVal*tempVal*1000;
        wordScores[i] = tempVal;
    }
    // cout << "Word scores are:" << endl;
    // printVector(wordScores);
    int curPara,curPage,curBook;
    float maxParaVal = -12;
    getParagraphEval(wordNodes,occurPtrs,curBook,curPage,curPara);
    while(curBook != INT_MAX){
        //getParagraphEval(wordNodes,occurPtrs,curBook,curPage,curPara);
        float paraVal = 0;
        bool samePara = 1;
        while(samePara){
            samePara = 0;
            int curSent = -1;
            for(int i = 0 ; i < allWords.size(); ++i){
                int tempBookCode,tempPageCode,tempParaCode,tempSentCode;
                getBookPageParaSent(wordNodes[i],occurPtrs[i],tempBookCode,tempPageCode,tempParaCode,tempSentCode);
                if(tempBookCode != curBook || tempParaCode != curPara || tempPageCode != curPage || (tempSentCode != curSent && curSent != -1)){continue;}
                if(curSent == -1){
                    curSent = tempSentCode;
                    samePara = 1;
                    paraVal += wordScores[i]*wordNodes[i]->occurences[occurPtrs[i]].freq;
                }
                else{
                    //will not be considering stop words here
                    paraVal += 20*wordScores[i]*wordNodes[i]->occurences[occurPtrs[i]].freq;
                }
                occurPtrs[i]++;
                //cout << "Entered loop at some point" << endl;
            }
        }
        if(paraVal > maxParaVal){
            maxParaVal = paraVal;
        }
        //cout << "Currently handling " <<"(" << curBook << "," << curPage << "," << curPara << ")" << endl;
        h.insert(curBook,curPage,curPara,paraVal);
        getParagraphEval(wordNodes,occurPtrs,curBook,curPage,curPara);
    }
    //cout << "max para val is:" << maxParaVal << endl;
    Node* head = h.get_min();
    h.deleteMin();
    int numTerms = 0;
    while(!h.empty()){
        numTerms++;
        head->left = h.get_min();
        //cout << "head->right is:" <<"(" << h.get_min()->book_code << "," << h.get_min()->page << "," << h.get_min()->paragraph << ")" << endl;
        head->left->right = head;
        head = head->left;
        h.deleteMin();
    }
    //cout << "numTerms:" << numTerms << endl;
    return head;
}


//this is the actual function to implement
void QNA_tool::query(string question, string filename){
    // Implement your function here
    //auto start = chrono::high_resolution_clock::now();  
    if(!initStemmed){
        // auto start = chrono::high_resolution_clock::now();
        initStemmed = false;
        initStemmedDict();
    }   
    int k = 10;
    string API_KEY = "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";
    query_llm("api_call.py",get_top_k_para_modified(question,k),k , API_KEY ,question,filename);
    return;
}

void get_paragraph(int book_code, int page, int paragraph, int sentence_no, string &res){

    std::ifstream inputFile(FILENAME);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file." << std::endl;
        exit(1);
    }

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[4];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph) &&
            (metadata[3] == sentence_no)
        ){
            res = sentence;
            return;
        }

        res = "$I$N$V$A$L$I$D$";
        return;
    }

}

// void printParaGraphLinkedList(QNA_tool& tool,Node* head){
//     Node* curNode = head;
//     while(curNode){
//         int bookCode = curNode->book_code;
//         int pageCode = curNode->page;
//         int paragraphCode = curNode->paragraph;
//         cout << "(" << bookCode << "," << pageCode << "," << paragraphCode << ")" << endl;
//         cout << tool.get_paragraph(bookCode,pageCode,paragraphCode) << endl;
//         cout << endl;
//         curNode = curNode->right;
//     }
// }

void deleteLinkedList(Node* head){
    Node* prevNode = head;
    Node* curNode = nullptr;
    while(prevNode){
        curNode = prevNode->right;
        delete prevNode;
        prevNode = curNode;
    }
}
std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    // cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = BASE_PATH;
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += '\n';
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}
void QNA_tool::initStemmedDict(){
    for(string& fileName:fileNames){
        ifstream inputFile(fileName,ios::in);
        string tuple;
        string sentence;
        vector<int> metadata(4,0);
        while(getline(inputFile,tuple,')') && getline(inputFile,sentence)){
            tuple += ')';
            //int metadata[4];
            std::istringstream iss(tuple);
            std::string token;
            iss.ignore(1);
            int idx = 0;
            while (std::getline(iss, token, ',')) {
                // Trim leading and trailing white spaces
                size_t start = token.find_first_not_of(" ");
                size_t end = token.find_last_not_of(" ");
                if (start != std::string::npos && end != std::string::npos) {
                    token = token.substr(start, end - start + 1);
                }
                
                // Check if the element is a number or a string
                if (token[0] == '\'') {
                    // Remove the single quotes and convert to integer
                    int num = std::stoi(token.substr(1, token.length() - 2));
                    metadata[idx] = num;
                } else {
                    // Convert the element to integer
                    int num = std::stoi(token);
                    metadata[idx] = num;
                }
                idx++;
            }
            allData.insert_sentence(metadata[0],metadata[1],metadata[2],metadata[3],sentence);
        }
    }
}
void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question,string outFile){

    // first write the k paragraphs into different files
    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "Could you please answer the question in the next line" << endl;
    outfile << question << endl;
    outfile << "By only using the following exercepts from Mahatma Gandhi's books given below" << endl;
    outfile << "Also include quotes from the given exercepts for context" << endl;
    outfile << "Incase the paragraphs seem irrelevant please look into them again" << endl;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";
    command += " ";
    command += outFile;

    //temporarily removed interacting with the line
    //cout << "about to call python command " << endl;
    system(command.c_str());
    //cout << "called the python command" << endl;
    return;
}

// void printLinkedList(Node* head){
//     Node* curNode = head;
//     while(curNode){
//         cout << "(" << curNode->book_code << "," << curNode->page << "," << curNode->paragraph << ")" << endl;
//         curNode = curNode->right;
//     }
// }
/*
int main(){
    auto start = chrono::high_resolution_clock::now();
    QNA_tool tool;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    
    cout << "Constructor finished" << endl;
    cout << "Time taken for constructor is:" << duration.count() << endl;

    
    string question1 = "What were the views of Mahatma Gandhi on the partition of India?";
    string question2 = "Who was Mahatma Gandhi?";
    string question3 = "What were Gandhi's views on the manner of how one should eat?";
    string question4 = "How was Kasturba related to Mahatma Gandhi?";
    string question5 = "What did Gandhi think of Nehru known as Nehruji";
    string question6 = "What were Mahatma Gandhi's view on religion?";
    
    string queryQuestion1 = "What were the views of Mahatma Gandhi on the Partition of India?";
    string queryQuestion2 = "Who was Mahatma Gandhi?";
    string queryQuestion3 = "What were Gandhi's views on the manner of how one should eat?";
    string queryQuestion4 = "Was Gandhi a lawyer?";
    string queryQuestion5 = "Is Gandhi alive?";
    string queryQuestion6 = "Waht were Gandhi's thoughts on religion?";
    int k1 = 4;
    string API_KEY = "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";
    tool.query(queryQuestion1,"out.txt");
    cout << "===========================================================================" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "Finished first question in the second question now" << endl;
    cout << "===========================================================================" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    tool.query(queryQuestion2,"ou2.txt");
    cout << "===========================================================================" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "Finished second question in the third question now" << endl;
    cout << "===========================================================================" << endl;
    tool.query(queryQuestion3,"out3.txt");
    cout << "===========================================================================" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "Finished third question fourth question now" << endl;
    cout << "===========================================================================" << endl;
    tool.query(queryQuestion4,"out4.txt");
    cout << endl;
    cout << endl;
    cout << endl;
    tool.query(queryQuestion6,"out6.txt");

    //tool.query(queryQuestion2,"out.txt");
    
    Node* node1 = tool.get_top_k_para(question1,4);
    cout << "for question1 as follows:" << endl;
    cout << question1 << endl;
    printLinkedList(node1);
    deleteLinkedList(node1);
    Node* node2 = tool.get_top_k_para(question2,7);
    cout << "for question2 as follows:" << endl;
    cout << question2 << endl;
    printLinkedList(node2);
    deleteLinkedList(node2);
    Node* node3 = tool.get_top_k_para(question3,8);
    cout << "for question3 as follows:" << endl;
    cout << question3 << endl;
    printLinkedList(node3);
    deleteLinkedList(node3);

    //(string filename, Node* root, int k, string API_KEY, string question
    
    cout << question1 << endl;
    Node* node1 = tool.get_top_k_para_modified(question1,4);
    printLinkedList(node1);
    //printParaGraphLinkedList(tool,node1);
    deleteLinkedList(node1);
    cout << endl;

    
    cout << question2 << endl;
    Node* node2 = tool.get_top_k_para_modified(question2,7);
    printLinkedList(node2);
    //printParaGraphLinkedList(tool,node2);
    deleteLinkedList(node2);
    cout << endl;


    cout << question3 << endl;
    Node* node3 = tool.get_top_k_para_modified(question3,8);
    printLinkedList(node3);
    //printParaGraphLinkedList(tool,node3);
    deleteLinkedList(node3);
    cout << endl;

    cout << question4 << endl;
    Node* node4 = tool.get_top_k_para_modified(question4,8);
    printLinkedList(node4);
    //printParaGraphLinkedList(tool,node4);
    deleteLinkedList(node4);
    cout << endl;

    cout << question5 << endl;
    Node* node5 = tool.get_top_k_para_modified(question5,8);
    printLinkedList(node5);
    //printParaGraphLinkedList(tool,node5);
    deleteLinkedList(node5);
    cout << endl;  
}
*/

