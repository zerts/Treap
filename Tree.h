#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric> 
#include "testlib.h"

const int _ZERO = 0, _INF = 10000000;

class ITree
{
public:
    virtual void insert(int, size_t) = 0;
    virtual void assign(int, size_t) = 0;
    virtual int getSegmentSum(size_t, size_t) = 0;
    virtual bool nextPermutation() = 0;
    virtual std::vector<int> getArray() = 0;
};

class Node
{
private:
    int key, priority, sum, leftestKey, rightestKey;
    unsigned int longestIncreasingBegin, longestDecreasingEnd;
    bool reverseFlag;
    size_t size;
public:
    Node *left, *right;
    int getKey()
    {
        return key;
    }
    int getSum()
    {
        return sum;
    }
    int getPriority()
    {
        return priority;
    }
    size_t getSize()
    {
        return size;
    }
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
    int getLeftestKey()
    {
        if (reverseFlag)
            return rightestKey;
        return leftestKey;
    }
    int getRightestKey()
    {
        if (reverseFlag)
            return leftestKey;
        return rightestKey;
    }
    void updateLeftestKey()
    {
        leftestKey = key;
        if (left != NULL)
            leftestKey = left->getLeftestKey();
    }
    void updateRightestKey()
    {
        rightestKey = key;
        if (right != NULL)
            rightestKey = right->getRightestKey();
    }
    unsigned int getLongestIncreasingBegin()
    {
        if (reverseFlag)
            return longestDecreasingEnd;
        return longestIncreasingBegin;
    }
    unsigned int getLongestDecreasingEnd()
    {
        if (reverseFlag)
            return longestIncreasingBegin;
        return longestDecreasingEnd;
    }
    void updateLongestBeginOrEnd(int key, unsigned int getFirstKey, unsigned int &longestFirstEnd, unsigned int getLongestSecondEnd, 
        unsigned int getLongestSecondBegin, size_t size, unsigned int &longestSecondEnd)
    {
        if (getFirstKey >= key)
            longestFirstEnd += getLongestSecondEnd;
        if (getLongestSecondBegin == size && getFirstKey <= key)
            longestSecondEnd += getLongestSecondBegin;
        else
            longestSecondEnd = getLongestSecondBegin;
    }
    void updateLongest()
    {
        longestDecreasingEnd = 1u;
        longestIncreasingBegin = 1u;
        if (left != NULL)
        {
            updateLongestBeginOrEnd(key, left->getRightestKey(), longestDecreasingEnd, left->getLongestDecreasingEnd(), 
                left->getLongestIncreasingBegin(), left->size, longestIncreasingBegin);
        }
        if (right != NULL)
        {
            updateLongestBeginOrEnd(key, right->getLeftestKey(), longestIncreasingBegin, right->getLongestIncreasingBegin(),
                right->getLongestDecreasingEnd(), right->size, longestDecreasingEnd);
        }
    }
    void push()
    {
        if (this == NULL)
            return;
        if (reverseFlag)
        {
            reverseFlag = false;
            std::swap(longestIncreasingBegin, longestDecreasingEnd);
            std::swap(leftestKey, rightestKey);
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
        updateLeftestKey();
        updateRightestKey();
        updateSize();
        updateSum();
        updateLongest();
    }
    ~Node()
    {
        if (right != NULL)
            delete right;
        if (left != NULL)
            delete left;
    };
    Node()
    {
        key = 0;
        priority = rnd.next(_ZERO, _INF);
        sum = 0;
        size = 1u;
        reverseFlag = false;
        longestIncreasingBegin = 1u;
        longestDecreasingEnd = 1u;
        leftestKey = 0;
        rightestKey = 0;
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
        longestIncreasingBegin = 1u;
        longestDecreasingEnd = 1u;
        leftestKey = k;
        rightestKey = k;
        left = NULL;
        right = NULL;
    }
};

void split(Node *source, size_t index, Node *&_left, Node *&_right)
{
    if (source == NULL)
    {
        _left = NULL;
        _right = NULL;
        return;
    }
    source->push();
    if (source->getSize() < index)
    {
        _left = source;
        _right = NULL;
        return;
    }
    if (index == 0u)
    {
        _left = NULL;
        _right = source;
        return;
    }
    if (source->getIndex() < index)
    {
        _left = source;
        split(source->right, index - source->getIndex() - 1, _left->right, _right);
        _left->update();
    }
    else
    {
        _right = source;
        split(source->left, index, _left, _right->left);
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
    if (_left->getPriority() > _right->getPriority())
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
        delete root;
    };
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
        split(_middle, l, _left, _middle);
        int result = _middle->getSum();
        _left = merge(_left, _middle);
        root = merge(_left, _right);
        return result;
    }

    bool nextPermutation()
    {
        Node *_left = NULL, *_right = NULL;
        if (root->getLongestDecreasingEnd() == root->getSize())
        {
            root->reverse();
            return false;
        }
        split(root, root->getSize() - root->getLongestDecreasingEnd(), _left, _right);
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
            if (_changableFromRight->getKey() <= _lastFromLeft->getKey())
            {
                _changableFromRight = _changableFromRight->left;
            }
            else if (_changableFromRight->getKey() > _lastFromLeft->getKey())
            {
                indexToRight += _changableFromRight->getIndex() + 1u;
                if (_changableFromRight->right == NULL || _changableFromRight->right->getLeftestKey() <= _lastFromLeft->getKey())
                    break;
                _changableFromRight = _changableFromRight->right;
            }
        } while (true);
        int changeKeyFirst = _lastFromLeft->getKey(), changeKeySecond = _changableFromRight->getKey();
        Treap * l = new Treap(_left), * r = new Treap(_right);
        l->assign(changeKeySecond, indexToLeft - 1u);
        r->assign(changeKeyFirst, indexToRight - 1u);
        r->root->reverse();
        root = merge(l->root, r->root);
        l->root = NULL;
        r->root = NULL;
        delete l;
        delete r;
        return true;
    }

    std::vector <int> getArray()
    {
        std::vector <int> result(root->getSize());
        for (size_t i = 0; i < root->getSize(); i++)
        {
            Node *_left = NULL, *_middle = NULL, *_right = NULL;
            split(root, i + 1, _middle, _right);
            split(_middle, i, _left, _middle);
            result[i] = _middle->getKey();
            _left = merge(_left, _middle);
            root = merge(_left, _right);
        }
        return result;
    }
};

class SlowStruct : public ITree
{
private:
    std::vector <int> body;
public:
    SlowStruct(){};
    ~SlowStruct(){};
    int getSegmentSum(size_t l, size_t r)
    {
        return std::accumulate(body.begin() + l, body.begin() + r, 0LL);
    }
    void insert(int key, size_t index)
    {
        body.insert(body.begin() + index, key);
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
