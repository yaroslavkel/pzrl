#include "RBT.h"
#include <iostream>
#include <algorithm>

static void* g_lastRoot = nullptr;

BinarySearchTree::Node::Node(Key key, Value value, bool color_,
    Node *parent_, Node *left_, Node *right_)
    : keyValuePair{key, value}, parent(parent_), left(left_), right(right_), color(color_)
{}

BinarySearchTree::Node::Node(const Node &other)
    : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr), color(other.color)
{}

bool BinarySearchTree::Node::operator==(const Node &other) const
{
    return keyValuePair == other.keyValuePair && color == other.color;
}

void BinarySearchTree::Node::output_node_tree() const
{
    if (left) {
        left->output_node_tree();
    }
    std::cout << "[" << keyValuePair.first << " : " << keyValuePair.second
              << " (" << (color ? "R" : "B") << ")]\n";
    if (right) {
        right->output_node_tree();
    }
}

void BinarySearchTree::Node::insert(const Key &, const Value &) {}
void BinarySearchTree::Node::erase(const Key &) {}

BinarySearchTree::Node* BinarySearchTree::_cloneSubtree(const Node *src, Node *parent, const Node *sentinel)
{
    if (!src || src == sentinel) {
        return nullptr;
    }
    Node *n = new Node(src->keyValuePair.first, src->keyValuePair.second, src->color, parent);
    n->left = _cloneSubtree(src->left, n, sentinel);
    n->right = _cloneSubtree(src->right, n, sentinel);
    return n;
}

void BinarySearchTree::_destroySubtree(Node *node)
{
    if (!node) {
        return;
    }
    _destroySubtree(node->left);
    _destroySubtree(node->right);
    delete node;
}

BinarySearchTree::Node* BinarySearchTree::_subtreeMin(Node *node)
{
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

BinarySearchTree::Node* BinarySearchTree::_subtreeMax(Node *node)
{
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

BinarySearchTree::Node* BinarySearchTree::_successor(Node *node)
{
    if (!node) {
        return nullptr;
    }
    if (node->right) {
        return _subtreeMin(node->right);
    }
    Node *p = node->parent;
    while (p && node == p->right) {
        node = p;
        p = p->parent;
    }
    return p;
}

BinarySearchTree::Node* BinarySearchTree::_predecessor(Node *node)
{
    if (!node) {
        return _subtreeMax(static_cast<Node*>(g_lastRoot));
    }
    if (node->left) {
        return _subtreeMax(node->left);
    }
    Node *p = node->parent;
    while (p && node == p->left) {
        node = p;
        p = p->parent;
    }
    return p;
}

size_t BinarySearchTree::_subtreeHeight(const Node *node)
{
    if (!node) {
        return 0;
    }
    return 1 + std::max(_subtreeHeight(node->left), _subtreeHeight(node->right));
}

void BinarySearchTree::_rotateLeft(Node *x)
{
    Node *y = x->right;
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (!x->parent) {
        _root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void BinarySearchTree::_rotateRight(Node *y)
{
    Node *x = y->left;
    y->left = x->right;
    if (x->right) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (!y->parent) {
        _root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

void BinarySearchTree::_insertFixup(Node *z)
{
    while (z->parent && z->parent->color == true) {
        Node *gp = z->parent->parent;
        if (!gp) {
            break;
        }
        if (z->parent == gp->left) {
            Node *u = gp->right;
            if (u && u->color == true) {
                z->parent->color = false;
                u->color = false;
                gp->color = true;
                z = gp;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    _rotateLeft(z);
                }
                z->parent->color = false;
                z->parent->parent->color = true;
                _rotateRight(z->parent->parent);
            }
        } else {
            Node *u = gp->left;
            if (u && u->color == true) {
                z->parent->color = false;
                u->color = false;
                gp->color = true;
                z = gp;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    _rotateRight(z);
                }
                z->parent->color = false;
                z->parent->parent->color = true;
                _rotateLeft(z->parent->parent);
            }
        }
    }
    _root->color = false;
}

void BinarySearchTree::_transplant(Node *u, Node *v)
{
    if (!u->parent) {
        _root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) {
        v->parent = u->parent;
    }
}

void BinarySearchTree::_eraseFixup(Node *x, Node *xParent)
{
    while (x != _root && (!x || x->color == false)) {
        if (x == xParent->left) {
            Node *w = xParent->right;
            if (w && w->color == true) {
                w->color = false;
                xParent->color = true;
                _rotateLeft(xParent);
                w = xParent->right;
            }
            if (!w) {
                break;
            }
            if ((!w->left || w->left->color == false) &&
                (!w->right || w->right->color == false)) {
                w->color = true;
                x = xParent;
                xParent = x->parent;
            } else {
                if (!w->right || w->right->color == false) {
                    if (w->left) {
                        w->left->color = false;
                    }
                    w->color = true;
                    _rotateRight(w);
                    w = xParent->right;
                }
                w->color = xParent->color;
                xParent->color = false;
                if (w->right) {
                    w->right->color = false;
                }
                _rotateLeft(xParent);
                x = _root;
                xParent = nullptr;
            }
        } else {
            Node *w = xParent->left;
            if (w && w->color == true) {
                w->color = false;
                xParent->color = true;
                _rotateRight(xParent);
                w = xParent->left;
            }
            if (!w) {
                break;
            }
            if ((!w->right || w->right->color == false) &&
                (!w->left || w->left->color == false)) {
                w->color = true;
                x = xParent;
                xParent = x->parent;
            } else {
                if (!w->left || w->left->color == false) {
                    if (w->right) {
                        w->right->color = false;
                    }
                    w->color = true;
                    _rotateLeft(w);
                    w = xParent->left;
                }
                w->color = xParent->color;
                xParent->color = false;
                if (w->left) {
                    w->left->color = false;
                }
                _rotateRight(xParent);
                x = _root;
                xParent = nullptr;
            }
        }
    }
    if (x) {
        x->color = false;
    }
}

void BinarySearchTree::_eraseNode(Node *z)
{
    Node *y = z;
    bool yOrig = y->color;
    Node *x = nullptr;
    Node *xParent = nullptr;
    if (!z->left) {
        x = z->right;
        xParent = z->parent;
        _transplant(z, z->right);
    } else if (!z->right) {
        x = z->left;
        xParent = z->parent;
        _transplant(z, z->left);
    } else {
        y = _subtreeMin(z->right);
        yOrig = y->color;
        x = y->right;
        if (y->parent == z) {
            xParent = y;
        } else {
            xParent = y->parent;
            _transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        _transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    --_size;
    if (yOrig == false) {
        _eraseFixup(x, xParent);
    }
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other)
    : _size(other._size)
    , _root(_cloneSubtree(other._root, nullptr, nullptr))
{}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree &other)
{
    if (this != &other) {
        _destroySubtree(_root);
        _root = _cloneSubtree(other._root, nullptr, nullptr);
        _size = other._size;
    }
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept
    : _size(other._size), _root(other._root)
{
    other._root = nullptr;
    other._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept
{
    if (this != &other) {
        _destroySubtree(_root);
        _root = other._root;
        _size = other._size;
        other._root = nullptr;
        other._size = 0;
    }
    return *this;
}

BinarySearchTree::~BinarySearchTree()
{
    _destroySubtree(_root);
}

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*()
{
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const
{
    return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->()
{
    return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const
{
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++()
{
    _node = _successor(_node);
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int)
{
    Iterator t = *this;
    _node = _successor(_node);
    return t;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--()
{
    _node = _predecessor(_node);
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int)
{
    Iterator t = *this;
    _node = _predecessor(_node);
    return t;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &o) const
{
    return _node == o._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &o) const
{
    return _node != o._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const
{
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const
{
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++()
{
    _node = _successor(const_cast<Node*>(_node));
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int)
{
    ConstIterator t = *this;
    _node = _successor(const_cast<Node*>(_node));
    return t;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--()
{
    _node = _predecessor(const_cast<Node*>(_node));
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int)
{
    ConstIterator t = *this;
    _node = _predecessor(const_cast<Node*>(_node));
    return t;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &o) const
{
    return _node == o._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &o) const
{
    return _node != o._node;
}

void BinarySearchTree::insert(const Key &key, const Value &value)
{
    Node *z = new Node(key, value, true);
    Node *y = nullptr;
    Node *x = _root;
    while (x) {
        y = x;
        x = (key < x->keyValuePair.first) ? x->left : x->right;
    }
    z->parent = y;
    if (!y) {
        _root = z;
    } else if (key < y->keyValuePair.first) {
        y->left = z;
    } else {
        y->right = z;
    }
    ++_size;
    _insertFixup(z);
}

void BinarySearchTree::erase(const Key &key)
{
    bool found = true;
    while (found) {
        found = false;
        Node *cur = _root;
        while (cur) {
            if (key < cur->keyValuePair.first) {
                cur = cur->left;
            } else if (key > cur->keyValuePair.first) {
                cur = cur->right;
            } else {
                _eraseNode(cur);
                found = true;
                break;
            }
        }
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const
{
    Node *cur = _root;
    while (cur) {
        if (key == cur->keyValuePair.first) {
            return ConstIterator(cur);
        } else if (key < cur->keyValuePair.first) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key)
{
    Node *cur = _root;
    while (cur) {
        if (key == cur->keyValuePair.first) {
            return Iterator(cur);
        } else if (key < cur->keyValuePair.first) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator>
BinarySearchTree::equalRange(const Key &key)
{
    Iterator first = find(key);
    Iterator last = first;
    while (last != end() && last->first == key) {
        ++last;
    }
    return {first, last};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator>
BinarySearchTree::equalRange(const Key &key) const
{
    ConstIterator first = find(key);
    ConstIterator last = first;
    while (last != cend() && last->first == key) {
        ++last;
    }
    return {first, last};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const
{
    return ConstIterator(_subtreeMin(_root));
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const
{
    return ConstIterator(_subtreeMax(_root));
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const
{
    auto [first, last] = equalRange(key);
    if (first == last) {
        return cend();
    }
    ConstIterator res = first;
    for (auto it = first; it != last; ++it) {
        if (it->second < res->second) {
            res = it;
        }
    }
    return res;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const
{
    auto [first, last] = equalRange(key);
    if (first == last) {
        return cend();
    }
    ConstIterator res = first;
    for (auto it = first; it != last; ++it) {
        if (it->second > res->second) {
            res = it;
        }
    }
    return res;
}

BinarySearchTree::Iterator BinarySearchTree::begin()
{
    g_lastRoot = static_cast<void*>(_root);
    return Iterator(_subtreeMin(_root));
}

BinarySearchTree::Iterator BinarySearchTree::end()
{
    g_lastRoot = static_cast<void*>(_root);
    return Iterator(nullptr);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const
{
    g_lastRoot = static_cast<void*>(_root);
    return ConstIterator(_subtreeMin(_root));
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const
{
    g_lastRoot = static_cast<void*>(_root);
    return ConstIterator(nullptr);
}

size_t BinarySearchTree::size() const
{
    return _size;
}

size_t BinarySearchTree::max_height() const
{
    return _subtreeHeight(_root);
}

void BinarySearchTree::output_tree()
{
    if (_root) {
        _root->output_node_tree();
    } else {
        std::cout << "(empty tree)\n";
    }
}
