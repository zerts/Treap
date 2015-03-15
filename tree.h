#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <iterator>
#include <vector>
#include "testlib.h"

const int _ZERO = 0, _INF = 10000000;

class ITree
{
public:
    virtual void insert(int, size_t) = 0;
    virtual void assign(int, size_t) = 0;
    virtual int getSegmentSum(size_t, size_t) = 0;
    virtual bool nextPermutation() = 0;
};

class Node
{
public:
    int key, priority, sum, leftKey, rightKey;
    size_t size;
    unsigned longestBeg, longestEnd;
    bool reverseFlag;
    Node *left, *right;
    size_t getIndex()
    {
        if (right == NULL)
            return size - 1;
        return size - right->size - 1;
    }
    void updateSize()
    {
        size = 1u;
        if (left != NULL)
            size += left->size;
        if (right != NULL)
            size += right->size;
    }
    void updateSum()
    {
        sum = key;
        if (left != NULL)
            sum += left->sum;
        if (right != NULL)
            sum += right->sum;
    }
    void reverse()
    {
        reverseFlag ^= 1;
    }
    int getLeft()
    {
        if (reverseFlag)
            return rightKey;
        return leftKey;
    }
    int getRight()
    {
        if (reverseFlag)
            return leftKey;
        return rightKey;
    }
    void updateLeft()
    {
        leftKey = key;
        if (left != NULL)
            leftKey = left->getLeft();
    }
    void updateRight()
    {
        rightKey = key;
        if (right != NULL)
            rightKey = right->getRight();
    }
    unsigned int getLongestBeg()
    {
        if (reverseFlag)
            return longestEnd;
        return longestBeg;
    }
    unsigned int getLongestEnd()
    {
        if (reverseFlag)
            return longestBeg;
        return longestEnd;
    }
    void updateLongest()
    {
        if (right == NULL && left == NULL)
        {
            longestBeg = 1u;
            longestEnd = 1u;
        }
        else if (right == NULL)
        {
            longestEnd = 1u;
            if (left->getRight() >= key)
                longestEnd += left->getLongestEnd();
            if (left->getLongestBeg() == left->size && left->getRight() <= key)
                longestBeg = left->getLongestBeg() + 1u;
            else
                longestBeg = left->getLongestBeg();
        }
        else if (left == NULL)
        {
            longestBeg = 1u;
            if (right->getLeft() >= key)
                longestBeg += right->getLongestBeg();
            if (right->getLongestEnd() == right->size && right->getLeft() <= key)
                longestEnd = right->getLongestEnd() + 1u;
            else
                longestEnd = right->getLongestEnd();
        }
        else
        {
            if (right->getLongestEnd() < right->size)
                longestEnd = right->getLongestEnd();
            else
            {
                if (key >= right->getLeft())
                {
                    longestEnd = right->size + 1u;
                    if (key <= left->getRight())
                        longestEnd += left->getLongestEnd();
                }
                else
                    longestEnd = right->getLongestEnd();
            }
            if (left->getLongestBeg() < left->size)
                longestBeg = left->getLongestBeg();
            else
            {
                if (key >= left->getRight())
                {
                    longestBeg = left->size + 1u;
                    if (key <= right->getLeft())
                        longestBeg += right->getLongestBeg();
                }
                else
                    longestBeg = left->getLongestBeg();
            }
        }
    }
    void push()
    {
        if (this == NULL)
            return;
        if (reverseFlag)
        {
            reverseFlag = false;
            std::swap(longestBeg, longestEnd);
            std::swap(leftKey, rightKey);
            Node *extra = left;
            left = right;
            right = extra;
            if (left != NULL)
                left->reverse();
            if (right != NULL)
                right->reverse();
        }
    }
    void update()
    {
        if (this == NULL)
            return;
        push();
        updateLeft();
        updateRight();
        updateSize();
        updateSum();
        updateLongest();
    }
    ~Node()
    {
        if (left != NULL)
            delete(left);
        if (right != NULL)
            delete(right);
    };
    Node()
    {
        key = 0;
        priority = rnd.next(_ZERO, _INF);
        sum = 0;
        size = 1u;
        reverseFlag = false;
        longestBeg = 1u;
        longestEnd = 1u;
        leftKey = 0;
        rightKey = 0;
        left = NULL;
        right = NULL;
    };
    Node(int k)
    {
        key = k;
        priority = rnd.next(_ZERO, _INF);
        sum = k;
        size = 1u;
        reverseFlag = false;
        longestBeg = 1u;
        longestEnd = 1u;
        leftKey = k;
        rightKey = k;
        left = NULL;
        right = NULL;
    }
};

void split(Node *sourse, size_t index, Node *&_left, Node *&_right)
{
    if (sourse == NULL)
    {
        _left = NULL;
        _right = NULL;
        return;
    }
    sourse->push();
    if (sourse->size < index)
    {
        _left = sourse;
        _right = NULL;
        return;
    }
    if (index == 0u)
    {
        _left = NULL;
        _right = sourse;
        return;
    }
    if (sourse->getIndex() < index)
    {
        _left = sourse;
        split(sourse->right, index - sourse->getIndex() - 1, _left->right, _right);
        _left->update();
    }
    else
    {
        _right = sourse;
        split(sourse->left, index, _left, _right->left);
        _right->update();
    }
}

Node * merge(Node *_left, Node *_right)
{
    if (_left == NULL)
        return _right;
    if (_right == NULL)
        return _left;
    _left->push();
    _right->push();
    if (_left->priority > _right->priority)
    {
        _left->right = merge(_left->right, _right);
        _left->update();
        return _left;
    }
    _right->left = merge(_left, _right->left);
    _right->update();
    return _right;
}

class Treap : public ITree
{
public:
    Node *root;
    Treap(){};
    Treap(Node *curr)
    {
        root = curr;
    }
    ~Treap()
    {
        delete(root);
    }
    void addNode(Node *_left, Node *_right, int _key)
    {
        Node *newNode = new Node(_key);
        _left = merge(_left, newNode);
        root = merge(_left, _right);
    }

    void insert(int _key, size_t _index)
    {
        Node *_left = NULL, *_right = NULL;
        split(root, _index, _left, _right);
        addNode(_left, _right, _key);
    }

    void assign(int _key, size_t _index)
    {
        Node *_left = NULL, *_right = NULL, *_bad = NULL;
        split(root, _index + 1u, _bad, _right);
        split(_bad, _index, _left, _bad);
        delete(_bad);
        addNode(_left, _right, _key);
    }

    int getSegmentSum(size_t l, size_t r)
    {
        Node *_left = NULL, *_middle = NULL, *_right = NULL;
        split(root, r, _middle, _right);
        split(_middle, l - 1, _left, _middle);
        int result = _middle->sum;
        _left = merge(_left, _middle);
        root = merge(_left, _right);
        return result;
    }

    bool nextPermutation()
    {
        Node *_left = NULL, *_right = NULL;
        if (root->getLongestEnd() == root->size)
        {
            root->reverse();
            return false;
        }
        split(root, root->size - root->getLongestEnd(), _left, _right);
        Node *_lastFromLeft = _left, *_changableFromRight = _right;
        _lastFromLeft->push();
        size_t indexToLeft = _lastFromLeft->getIndex() + 1u, indexToRight = 0u;
        while (_lastFromLeft->right != NULL)
        {
            _lastFromLeft = _lastFromLeft->right;
            _lastFromLeft->push();
            indexToLeft += _lastFromLeft->getIndex() + 1u;
        }
        do
        {
            _changableFromRight->push();
            if (_changableFromRight->key < _lastFromLeft->key)
            {
                _changableFromRight = _changableFromRight->left;
            }
            else if (_changableFromRight->key >= _lastFromLeft->key)
            {
                indexToRight += _changableFromRight->getIndex() + 1u;
                if (_changableFromRight->right == NULL || _changableFromRight->right->getLeft() < _lastFromLeft->key)
                    break;
                _changableFromRight = _changableFromRight->right;
            }
        } while (true);
        int changeKeyFirst = _lastFromLeft->key, changeKeySecond = _changableFromRight->key;
        Treap l = Treap(_left), r = Treap(_right);
        l.assign(changeKeySecond, indexToLeft - 1u);
        r.assign(changeKeyFirst, indexToRight - 1u);
        r.root->reverse();
        root = merge(l.root, r.root);
        return true;
    }

    std::vector <int> getArray()
    {
        std::vector <int> result(root->size);
        for (size_t i = 0; i < root->size; i++)
        {
            Node *_left = new Node(0), *_middle = new Node(0), *_right = new Node(0);
            split(root, i + 1, _middle, _right);
            split(_middle, i, _left, _middle);
            result[i] = _middle->key;
            _left = merge(_left, _middle);
            root = merge(_left, _right);
        }
        return result;
    }
};

class Arr : public ITree
{
private:
    std::vector <int> body;
public:
    Arr(){};
    ~Arr(){};
    int getSegmentSum(size_t l, size_t r)
    {
        int result = 0;
        for (size_t i = l; i <= r; i++)
            result += body[i];
        return result;
    }
    void insert(int key, size_t index)
    {
        if (body.size() == 0)
        {
            body.push_back(key);
            return;
        }
        body.push_back(body.back());
        for (size_t i = body.size() - 2; i > index; i--)
            body[i] = body[i - 1];
        body[index] = key;
    }
    void assign(int key, size_t index)
    {
        body[index] = key;
    }
    bool nextPermutation()
    {
        return std::next_permutation(body.begin(), body.end());
    }
    std::vector <int> getArray()
    {
        std::vector <int> result;
        result = body;
        return  result;
    }
    size_t getSize()
    {
        return body.size();
    }
};
