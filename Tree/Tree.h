//
// Created by ready on 11.05.2020.
//

#ifndef ARCHRONIS_TREE_H
#define ARCHRONIS_TREE_H

#include <assert.h>

template <class Data>
class Tree {
public:
    struct Node {
        Data data;
        Node *leftChild;
        Node *rightChild;

        explicit Node(const Data & newData)
                :data(newData),
                 leftChild(nullptr),
                 rightChild(nullptr) {}
    };

    explicit Tree(const Data &);
    ~Tree();
    Tree(const Tree &) = delete;
    const Tree & operator=(const Tree &) = delete;

    Data getRootData() const;
    Node *getRoot() const;

    void setRootData(const Data & data);
    void setRoot(Node *node);
    void setLeft(Node *node);
    void setRight(Node *node);

    bool operator>(const Tree &) const;

private:

    Node *root;

    void deleteSubTree(Node *node);

};

#endif //ARCHRONIS_TREE_H


