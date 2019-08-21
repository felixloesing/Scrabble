#include "Trie.h"

TrieSet::TrieSet() {
    mRoot = new TrieNode();
    mRoot->inSet = false;
    mRoot->parent = NULL;

    for (int j = 0; j < 26; j++) {
        mRoot->children[j] = NULL;
    }
}

TrieSet::~TrieSet() {

    if (mRoot == NULL) {
        return;
    } else {
        rm(mRoot);
    }
    delete mRoot;
    mRoot = NULL;
}

void TrieSet::rm(TrieNode* node)
{
    if (node == NULL) {
        return;
    } else {
        for (int i = 0; i < 26; ++i) {
            rm(node->children[i]);
        }
    }
}

void TrieSet::insert(std::string input) {

    std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    TrieNode* current = mRoot;

    for (unsigned int i = 0; i < input.length(); ++i)
    {
        int pos = input.at(i) - 'a';

        if (current->children[pos] == NULL) {
            //create new Node
            current->children[pos] = new TrieNode();
            current->children[pos]->inSet = false;
            current->children[pos]->parent = current;

            for (int j = 0; j < 26; j++) {
                current->children[pos]->children[j] = NULL;
            }

        }

        current = current->children[pos];

    }
    current->inSet = true;

}

void TrieSet::remove(std::string input) {
    if (mRoot == NULL) {
        return;
    }

    if (prefix(input) == NULL) {
        return;
    }

    removeHelper(input, mRoot, 0, input.length());

}

bool TrieSet::removeHelper(std::string input, TrieNode* current, int level, int length) {
    
    if (current == NULL) {
        return false;
    }

    bool deleted = false;

    if (length == level) {

        if (freeNode(current)) {
            current = NULL;
            delete current;
            deleted = true;

        } else {
            current->inSet = false;
            deleted = false;
        }

    } else {

        TrieNode* childNode = current->children[(int)input.at(level)-'a'];
        bool childDeleted = removeHelper(input, childNode, level + 1, length);

        if (childDeleted == true) {
            current->children[(int)input.at(level)-'a'] = NULL;

            if (!freeNode(current) || (leafNode(current))) {
                deleted = false;
            } else {
                current = NULL;
                delete current;
                deleted = true;
            }

        } else if (childDeleted == false) {
            deleted = false;
        }
    }

    return deleted;
}

bool TrieSet::leafNode(TrieNode* node) 
{ 
    if (node->inSet) {
        return true;
    }

    return false; 
}

bool TrieSet::freeNode(TrieNode* n) {
    for (int i = 0; i < 26; i++) { 
        if (n->children[i]) {
            return false;
        } 
    }
    return true;
}

TrieNode* TrieSet::prefix(std::string px) const {
    TrieNode* current = mRoot;

    if (px == "") {
        return mRoot;
    }

    for (unsigned int i = 0; i < px.length(); ++i)
    {
        int pos = px.at(i) - 'a';
        
        if (current->children[pos] == NULL) {
            return NULL;
        }
        current = current->children[pos];
    }

    if (current) {
        return current;
    } else {
        return NULL;
    }
}

TrieNode* TrieSet::search(std::string px) const {
    TrieNode* current = mRoot;

    for (unsigned int i = 0; i < px.length(); ++i)
    {
        int pos = px.at(i) - 'a';
        
        if (current->children[pos] == NULL) {
            return NULL;
        }
        current = current->children[pos];
    }

    if (current->inSet == true) {
        return current;
    } else {
        return NULL;
    }
}