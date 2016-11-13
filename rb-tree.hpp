//
//  rb-tree.hpp
//  search-trees
//
//  MIT License
//
//  Copyright (c) 2016 Oliver Old
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//

#pragma once

#include "tree.hpp"

template <typename T>
class RedBlackNode : public Node<T> {
    const T data;
    bool isRed;
    RedBlackNode<T>* parent;
    RedBlackNode<T>* left;
    RedBlackNode<T>* right;

public:
    RedBlackNode<T>(const T* const data,
                    const bool isRed = false,
                    RedBlackNode<T>* const parent = nullptr)
        : data {*data}, isRed {isRed}, parent {parent}, left {nullptr},
          right {nullptr} {}
    ~RedBlackNode<T>() override {
        delete left;
        delete right;
    }
    const T* getData() const override {
        return &data;
    }
    RedBlackNode<T>* insert(const T* const data) {
        if (*data < this->data) {
            if (!left) {
                left = new RedBlackNode<T> {data, true, this};
                return left->balance();
            } else {
                return left->insert(data);
            }
        } else if (*data > this->data) {
            if (!right) {
                right = new RedBlackNode<T> {data, true, this};
                return right->balance();
            } else {
                return right->insert(data);
            }
        }
        return nullptr;
    }
    bool search(const T& searchFor) const {
        auto curr {this};
        while (true) {
            if (searchFor == curr->data) {
                return true;
            } else if (searchFor < curr->data) {
                if (!curr->left) {
                    return false;
                }
                curr = curr->left;
            } else /* if (searchFor > curr->data) */ {
                if (!curr->right) {
                    return false;
                }
                curr = curr->right;
            }
        }
    }

private:
    RedBlackNode<T>* rotateLeft() {
        auto p = right;
        right->parent = this->parent;
        right = p->left;
        if (p->left) {
            p->left->parent = this;
        }
        p->left = this;
        this->parent = p;
        return p;
    }
    RedBlackNode<T>* rotateRight() {
        auto q = left;
        left->parent = this->parent;
        left = q->right;
        if (q->right) {
            q->right->parent = this;
        }
        q->right = this;
        this->parent = q;
        return q;
    }
    RedBlackNode<T>* balance() {
        const auto k = this;
        const auto p = k->parent;

        if (!p) {
            k->isRed = false;
            return k;
        }

        if (p->isRed) {
            const auto g = p->parent;
            const auto gParent = g->parent;
            const bool isKLeft = k == p->left;
            const bool isPLeft = p == g->left;
            RedBlackNode<T>* s;
            isPLeft ? s = g->right : s = g->left;

            if (!s || !s->isRed) {
                g->isRed = true;
                if (isKLeft) {
                    p->isRed = false;
                }
                if (isPLeft) {
                    if (!isKLeft) {
                        k->isRed = false;
                        g->left = p->rotateLeft();
                    } else /* if (isKLeft) */ {
                        p->isRed = false;
                    }
                    if (gParent) {
                        if (g == gParent->left) {
                            gParent->left = g->rotateRight();
                        } else /* if (g == g->parent->right) */ {
                            gParent->right = g->rotateRight();
                        }
                    } else {
                        return g->rotateRight();
                    }
                } else /* if (!isPLeft) */ {
                    if (isKLeft) {
                        k->isRed = false;
                        g->right = p->rotateRight();
                    } else /* if (!isKLeft) */ {
                        p->isRed = false;
                    }
                    if (gParent) {
                        if (g == gParent->left) {
                            gParent->left = g->rotateLeft();
                        } else /* if (g == g->parent->right) */ {
                            gParent->right = g->rotateLeft();
                        }
                    } else {
                        return g->rotateLeft();
                    }
                }
                // Should only be reached if gParent != nullptr
                auto newRoot = gParent;
                while (true) {
                    if (newRoot->parent) {
                        newRoot = newRoot->parent;
                    } else {
                        return newRoot;
                    }
                }
            } else /* if (s->isRed) */ {
                p->isRed = false;
                s->isRed = false;
                g->isRed = true;
                return g->balance();
            }
        }
        // Find root
        auto newRoot = p;
        while (true) {
            if (newRoot->parent) {
                newRoot = newRoot->parent;
            } else {
                return newRoot;
            }
        }
    }
};

template <typename T>
class RedBlackTree : public Tree<T> {
    RedBlackNode<T>* root;

public:
    RedBlackTree<T>() : root {nullptr} {}
    ~RedBlackTree<T>() override {
        delete root;
    }
    bool insert(const T* const data) override {
        if (!root) {
            root = new RedBlackNode<T> {data};
            return true;
        } else if (*data != *root->getData()) {
            auto res = root->insert(data);
            if (res) {
                root = res;
            }
            return res;
        }
        return false;
    }
    bool search(const T& searchFor) const override {
        if (!root) {
            return false;
        } else {
            return root->search(searchFor);
        }
    }
};
