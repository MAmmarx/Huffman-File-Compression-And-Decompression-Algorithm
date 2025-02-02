#include <iostream>
#include <cstring>
#include <string>
using namespace std;

class CTNode {

public:
    
    char ch;
    CTNode* pLeft;
    CTNode* pRight;
};

class CSNode {

public:
    
    int info;
    char charc;
    CSNode* pNext;
    CTNode* PDown;
};

class CSList {

public:
    
    CSNode* pHead;

    CSList() {
        
        pHead = NULL;
    }

    ~CSList() {
        
        while (pHead != NULL) {
            
            CSNode* temp = pHead;
            pHead = pHead->pNext;
            delete temp;
        }
    }

    void insert(CSNode* pnn) {
        
        if (pHead == NULL || pnn->info < pHead->info) {
            
            pnn->pNext = pHead;
            pHead = pnn;
        } 
        
        else {
            
            CSNode* pTrav = pHead;
            
            while (pTrav->pNext != NULL && pTrav->pNext->info <= pnn->info) {
                
                pTrav = pTrav->pNext;
            }
            
            pnn->pNext = pTrav->pNext;
            pTrav->pNext = pnn;
        }
    }
};

void countcharacters(char input[], char characters[], int frequencies[], int& encsize) {
    
    encsize = 0;
    
    for (int i = 0; input[i] != '\0'; i++) {
        
        int found = 0;
        
        for (int j = 0; j < encsize; j++) {
            
            if (characters[j] == input[i]) {
                
                frequencies[j]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            
            characters[encsize] = input[i];
            frequencies[encsize] = 1;
            encsize++;
        }
    }
}

CSNode* SmallestNode(CSList& list) {
    
    if (list.pHead == NULL){ 
        
        return NULL;
    }

    CSNode* smallest = list.pHead;
    list.pHead = list.pHead->pNext;
    smallest->pNext = NULL;
    return smallest;
}

void HuffmanTree(CSList& list) {
    
    while (list.pHead != NULL && list.pHead->pNext != NULL) {
        
        CSNode* smallest1 = SmallestNode(list);
        CSNode* smallest2 = SmallestNode(list);

        CTNode* parentCTNode = new CTNode();
        parentCTNode->pLeft = smallest1->PDown;
        parentCTNode->pRight = smallest2->PDown;

        CSNode* parentCSNode = new CSNode();
        parentCSNode->info = smallest1->info + smallest2->info;
        parentCSNode->PDown = parentCTNode;

        list.insert(parentCSNode);

        delete smallest1;
        delete smallest2;
    }
}

void encoding(CTNode* ptrav, string code, char characters[], string encodings[], int& encsize) {
    
    if (ptrav == NULL){
        
        return;
    }

    if (ptrav->pLeft == NULL && ptrav->pRight == NULL) {
        
        characters[encsize] = ptrav->ch;
        encodings[encsize] = code;
        encsize++;
        return;
    }
    
    encoding(ptrav->pLeft, code + "0", characters, encodings, encsize);
    encoding(ptrav->pRight, code + "1", characters, encodings, encsize);
}

void compress(char input[], char characters[], string encodings[], int encsize, char DC[], int& ct, int& totalBits) {
    
    char temp = 0; 
    char m = 1;   
    int bitPosition = 7; 
    ct = 0;   
    totalBits = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        
        string code = "";

        for (int j = 0; j < encsize; j++) {
            
            if (characters[j] == input[i]) {
                code = encodings[j];
                break;
            }
        }

        for (char bit : code) {
            
            if (bit == '1') {
                temp = temp | (m << bitPosition);
            }
            
            bitPosition--; 
            totalBits++;

            if (bitPosition < 0) {
                DC[ct++] = temp;
                temp = 0;
                bitPosition = 7;
            }
        }
    }

    if (bitPosition != 7) {
        
        DC[ct++] = temp;
    }
}

void decompress(char DC[], int ct, char characters[], string encodings[], int encsize, int totalBits) {
    
    char tmp = 0;
    string cancode = "";
    char DD[100];
    int DDIndex = 0;
    int DCIndex = 0;
    int bitsProcessed = 0;

    while (DCIndex < ct && bitsProcessed < totalBits) {
        
        tmp = DC[DCIndex];
        DCIndex++;

        for (int i = 7; i >= 0; i--) {
            
            if (bitsProcessed >= totalBits){ 
                
                break;
            }
            int R = tmp & (1 << i);
            cancode += (R != 0) ? "1" : "0";
            bitsProcessed++;

            for (int j = 0; j < encsize; j++) {
               
               if (cancode == encodings[j]) {
                    
                    DD[DDIndex++] = characters[j];
                    cancode = "";
                    break;
                }
            }
        }
    }

    DD[DDIndex] = '\0';
    cout << endl << "Decompressed data = " << DD << endl;
}

int main() {
    
    char input[100];
    char characters[100];
    int frequencies[100];
    int encsize;
    CSList L;

    cout << "Enter a string = ";
    cin.getline(input, 100);

    countcharacters(input, characters, frequencies, encsize);

    for (int i = 0; i < encsize; i++) {
        
        CSNode* newNode = new CSNode();
        newNode->info = frequencies[i];
        newNode->charc = characters[i];
        newNode->PDown = new CTNode();
        newNode->PDown->ch = characters[i];
        L.insert(newNode);
    }

    HuffmanTree(L);

    if (L.pHead && L.pHead->PDown) {
        
        char encCharacters[100];
        string encodings[100];
        int encSize = 0;

        encoding(L.pHead->PDown, "", encCharacters, encodings, encSize);

        cout << "Huffman encodings = " << endl;

        for (int i = 0; i < encSize; i++) {
            
            cout << encCharacters[i] << " = " << encodings[i] << endl;
        }

        char DC[100];  
        int ct = 0;
        int totalBits = 0;

        compress(input, encCharacters, encodings, encSize, DC, ct, totalBits);

        cout << "Compressed data = " << endl;       
        
        for (int i = 0; i < ct; i++) {
            
            cout << DC[i]; 
        }

        decompress(DC, ct, encCharacters, encodings, encSize, totalBits);
    }

    return 0;
}
