#pragma once
#include <cstddef>

template<typename Key, typename Value>
class SplayTree {
public:
    SplayTree() : root_(nullptr), size_(0) {}

    ~SplayTree() {
        destroyTree(root_);
    }

    void insert(const Key& key, const Value& value) {
        if (!root_) {
            root_ = new Node(key, value);
            ++size_;
            return;
        }
        Node* curr = root_;
        while (true) {
            if (key < curr->key) {
                if (!curr->left) {
                    curr->left = new Node(key, value);
                    curr->left->parent = curr;
                    ++size_;
                    splay(curr->left);
                    return;
                }
                curr = curr->left;
            } else if (curr->key < key) {
                if (!curr->right) {
                    curr->right = new Node(key, value);
                    curr->right->parent = curr;
                    ++size_;
                    splay(curr->right);
                    return;
                }
                curr = curr->right;
            } else {
                curr->value = value;
                splay(curr);
                return;
            }
        }
    }

    bool remove(const Key& key) {
        if (!findNode(key)) return false;
        Node* toDelete = root_;
        if (!root_->left) {
            root_ = root_->right;
            if (root_) root_->parent = nullptr;
        } else {
            Node* leftTree = root_->left;
            leftTree->parent = nullptr;
            Node* rightTree = root_->right;
            if (rightTree) rightTree->parent = nullptr;
            root_ = leftTree;
            Node* maxLeft = leftTree;
            while (maxLeft->right) maxLeft = maxLeft->right;
            splay(maxLeft);
            root_->right = rightTree;
            if (rightTree) rightTree->parent = root_;
        }
        delete toDelete;
        --size_;
        return true;
    }

    Value* search(const Key& key) {
        Node* found = findNode(key);
        return found ? &found->value : nullptr;
    }

    const Value* search(const Key& key) const {
        Node* curr = root_;
        while (curr) {
            if (key < curr->key) curr = curr->left;
            else if (curr->key < key) curr = curr->right;
            else                      return &curr->value;
        }
        return nullptr;
    }

    bool isValidBST() const {
        return isValidBSTHelper(root_, nullptr, nullptr);
    }

    size_t size() const { return size_; }
    bool empty() const  { return size_ == 0; }

private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        Node* parent;
        Node(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root_;
    size_t size_;

    void destroyTree(Node* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root_ = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right) y->right->parent = x;
        y->parent = x->parent;
        if (!x->parent) root_ = y;
        else if (x == x->parent->right) x->parent->right = y;
        else x->parent->left  = y;
        y->right  = x;
        x->parent = y;
    }

    void splay(Node* x) {
        while (x->parent) {
            Node* p = x->parent;
            Node* g = p->parent;
            if (!g) {
                if (x == p->left) rotateRight(p);
                else rotateLeft(p);
            } else if (x == p->left && p == g->left) {
                rotateRight(g);
                rotateRight(p);
            } else if (x == p->right && p == g->right) {
                rotateLeft(g);
                rotateLeft(p);
            } else if (x == p->right && p == g->left) {
                rotateLeft(p);
                rotateRight(g);
            } else {
                rotateRight(p);
                rotateLeft(g);
            }
        }
    }

    Node* findNode(const Key& key) {
        Node* curr = root_;
        Node* last = nullptr;
        while (curr) {
            last = curr;
            if (key < curr->key)      curr = curr->left;
            else if (curr->key < key) curr = curr->right;
            else break;
        }
        if (last) splay(last);
        return (root_ && !(root_->key < key) && !(key < root_->key)) ? root_ : nullptr;
    }

    bool isValidBSTHelper(Node* node, const Key* minKey, const Key* maxKey) const {
        if (!node) return true;
        if (minKey && !((*minKey) < node->key)) return false;
        if (maxKey && !(node->key < (*maxKey))) return false;
        return isValidBSTHelper(node->left, minKey, &node->key) &&
               isValidBSTHelper(node->right, &node->key, maxKey);
    }
};
