//
//  avl-tree.hpp
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

#include <cstdint>
#include <utility>

using byte = std::int8_t;

template <typename T>
class AvlNode : public Node<T> {
    const T data;
    byte height;
    AvlNode<T>* left;
    AvlNode<T>* right;

public:
    AvlNode<T>(const T* const data)
        : data {*data}, left {nullptr}, right {nullptr}, height {1} {}
    ~AvlNode<T>() override {
        delete left;
        delete right;
    }
    const T* getData() const override {
        return &data;
    }
    std::pair<AvlNode<T>*, bool> insert(const T* const data) {
        if (*data < this->data) {
            if (!left) {
                left = new AvlNode<T> {data};
                fixHeight();
                return std::make_pair(this, true);
            } else {
                auto res = left->insert(data);
                left = std::get<0>(res);
                return std::make_pair(balance(), std::get<1>(res));
            }
        } else if (*data > this->data) {
            if (!right) {
                right = new AvlNode<T> {data};
                fixHeight();
                return std::make_pair(this, true);
            } else {
                auto res = right->insert(data);
                right = std::get<0>(res);
                return std::make_pair(balance(), std::get<1>(res));
            }
        }
        return std::make_pair(this, false);
    }
    bool search(const T& searchFor) const {
        auto curr = this;
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
    void fixHeight() {
        unsigned char hl = left ? left->height : 0;
        unsigned char hr = right ? right->height : 0;
        height = (hl > hr ? hl : hr) + 1;
    }
    byte getBalancingFactor() const {
        return (right ? right->height : 0) - (left ? left->height : 0);
    }
    AvlNode<T>* rotateLeft() {
        auto p = right;
        right = p->left;
        p->left = this;
        fixHeight();
        p->fixHeight();
        return p;
    }
    AvlNode<T>* rotateRight() {
        auto q = left;
        left = q->right;
        q->right = this;
        fixHeight();
        q->fixHeight();
        return q;
    }
    AvlNode<T>* balance() {
        fixHeight();
        if (getBalancingFactor() == 2)
        {
            if (right->getBalancingFactor() < 0)
                right = right->rotateRight();
            return rotateLeft();
        }
        if (getBalancingFactor() == -2)
        {
            if (left->getBalancingFactor() > 0)
                left = left->rotateLeft();
            return rotateRight();
        }
        return this;
    }
};

template <typename T>
class AvlTree : public Tree<T> {
    AvlNode<T>* root;

public:
    AvlTree<T>() : root {nullptr} {}
    ~AvlTree<T>() override {
        delete root;
    }
    bool insert(const T* const data) override {
        if (!root) {
            root = new AvlNode<T> {data};
            return true;
        } else if (*data != *root->getData()) {
            auto ret = root->insert(data);
            root = std::get<0>(ret);
            return std::get<1>(ret);
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
