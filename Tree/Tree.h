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


template <class Data>
Tree<Data>::Tree(const Data & d) : root(new Node(d)) {}

template <class Data>
Tree<Data>::~Tree() {
    deleteSubTree(root);
    root = nullptr;
}

template <class Data>
void Tree<Data>::deleteSubTree(Tree<Data>::Node *node) {
    if (node) {
        deleteSubTree(node->leftChild);
        deleteSubTree(node->rightChild);
        delete node;
    }
};

template <class Data>
Data Tree<Data>::getRootData() const {
    assert(root);
    return root->data;
}

template <class Data>
typename Tree<Data>::Node *Tree<Data>::getRoot() const {
    return root;
}

template <class Data>
void Tree<Data>::setRootData(const Data & data) {
    assert(root);
    root->data = data;
}

template <class Data>
void Tree<Data>::setRoot(Tree<Data>::Node *node) {
    root = node;
}

template <class Data>
void Tree<Data>::setLeft(Tree<Data>::Node *node) {
    assert(root);
    root->leftChild = node;
}

template <class Data>
void Tree<Data>::setRight(Tree<Data>::Node *node) {
    assert(root);
    root->rightChild = node;
}

template <class Data>
bool Tree<Data>::operator>(const Tree & T) const {
    assert(root);
    assert(T.root);
    return (root->data > T.root->data);
}

#endif //ARCHRONIS_TREE_H


