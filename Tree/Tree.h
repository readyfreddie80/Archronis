//
// Created by ready on 11.05.2020.
//

#ifndef ARCHRONIS_TREE_H
#define ARCHRONIS_TREE_H

#include <assert.h>

template <class Data>
class Tree {
private:
    struct Node {
        Data data;
        Node *leftChild;
        Node *rightChild;

        explicit Node(Data newData)
            :data(newData),
             leftChild(nullptr),
             rightChild(nullptr) {}
    };

    Node *root;

    void deleteSubTree(Node *node);

public:
    Tree();
    explicit Tree(Node *node);
    ~Tree();
    Tree(const Tree &) = delete;
    const Tree & operator=(const Tree &) = delete;

    Data getRootData() const;
    Node *getRoot() const;

    void setRootData(Data data);
    void setRoot(Node *node);
    void setLeft(Node *node);
    void setRight(Node *node);

    bool operator>(const Tree &) const;
};

#endif //ARCHRONIS_TREE_H



