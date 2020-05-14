//
// Created by ready on 11.05.2020.
//

#include "Tree.h"

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
    return (root->data > T.root->data);
}
