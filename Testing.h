#define _CRT_SECURE_NO_WARNINGS

#include "Tree.h"

const size_t ZERO = 0u, FOUR = 4u;

enum TEST_TYPE
{
    INSERT,
    ASSIGN,
    GET_SUM,
    NEXT_PERM
};

bool isEqual(Treap *tree, Arr *arr)
{
    std::vector <int> checkTree, checkArray;
    checkTree = tree->getArray();
    checkArray = arr->getArray();
    if (checkTree.size() != checkArray.size())
        return false;
    for (size_t i = 0; i < checkArray.size(); i++)
        if (checkTree[i] != checkArray[i])
            return false;
    return true;
}

std::vector <int> generateArray(size_t arraySize, int minRange, int maxRange)
{
    std::vector <int> result;
    for (size_t i = 0; i < arraySize; i++)
        result.push_back(rnd.next(minRange, maxRange));
    return result;
}

void doubleInsert(Treap *tree, Arr &arr, int key, size_t index)
{
    tree->insert(key, index);
    arr.insert(key, index);
}

void doubleAssign(Treap *tree, Arr &arr, int minRange, int maxRange)
{
    int key = rnd.next(minRange, maxRange);
    size_t index = rnd.next(ZERO, arr.getSize() - 1u);
    tree->assign(key, index);
    arr.assign(key, index);
}

bool doubleGetSum(Treap *tree, Arr &arr)
{
    int sumTree, sumArr;
    size_t l, r;
    l = rnd.next(ZERO, arr.getSize() - 1u);
    r = rnd.next(l, arr.getSize());
    sumTree = tree->getSegmentSum(l, r);
    sumArr = arr.getSegmentSum(l, r);
    return  sumTree == sumArr;
}

void doubleNextPerm(Treap *tree, Arr &arr)
{
    tree->nextPermutation();
    arr.nextPermutation();
}

void vectorToTree(std::vector <int> &testArray, Treap *tree, Arr &arr)
{
    for (size_t i = 0; i < testArray.size(); i++)
        doubleInsert(tree, arr, testArray[i], i);
}

bool testing(unsigned int numberOfTests, int minRange, int maxRange)
{
    size_t arraySize = rnd.next(1, 100);
    std::vector <int> testArray = generateArray(arraySize, minRange, maxRange);
    Node *treeRoot = new Node(0);
    Treap *tree = new Treap(treeRoot);
    Arr arr;
    arr.insert(0, 0);
    vectorToTree(testArray, tree, arr);
    bool rightResult = true;
    TEST_TYPE currTestType;
    size_t type;
    for (unsigned int i = 0; i < numberOfTests; i++)
    {
        if (arr.getSize() > 10)
        {
            type = rnd.next(ZERO, FOUR);
            switch (type)
            {
            case 0:
                currTestType = TEST_TYPE::INSERT;
            case 1:
                currTestType = TEST_TYPE::ASSIGN;
            case 2:
                currTestType = TEST_TYPE::GET_SUM;
            default:
                currTestType = TEST_TYPE::NEXT_PERM;
            }
        }
        else
        {
            currTestType = TEST_TYPE::INSERT;
        }
        switch (currTestType)
        {
        case INSERT:
            doubleInsert(tree, arr, rnd.next(rnd.next(minRange, maxRange)), rnd.next(ZERO, arr.getSize()));
        case ASSIGN:
            doubleAssign(tree, arr, minRange, maxRange);
        case GET_SUM:
            if (!doubleGetSum(tree, arr))
                rightResult = false;
        case NEXT_PERM:
            doubleNextPerm(tree, arr);
        }
        if (!isEqual(tree, &arr))
            rightResult = false;
    }
    return rightResult;
}
