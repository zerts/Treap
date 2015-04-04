#define _CRT_SECURE_NO_WARNINGS

#include "Tree.h"

const size_t ZERO = 0u, MIN_ARRAY_SIZE = 1u, MAX_ARRAY_SIZE = 1000u;
const int LITTLE_MAX_RANGE = 10, MIN_RANGE = 1, MAX_RANGE = 10000;

enum test_type
{
    INSERT,
    ASSIGN,
    GET_SUM,
    NEXT_PERMUTATION,
    OPERATIONS_NUMBER
};

bool isEqual(Treap *tree, SlowStruct *correct)
{
    std::vector <int> checkTree, checkArray;
    checkTree = tree->getArray();
    checkArray = correct->getArray();
    if (checkTree.size() != checkArray.size())
        return false;
    for (size_t i = 0; i < checkArray.size(); i++)
        if (checkTree[i] != checkArray[i])
            return false;
    return true;
}

std::vector <int> generateArray(size_t correctaySize, int minRange, int maxRange)
{
    std::vector <int> result;
    for (size_t i = 0; i < correctaySize; i++)
        result.push_back(rnd.next(minRange, maxRange));
    return result;
}

void doubleInsert(Treap *tree, SlowStruct *correct, int key, size_t index)
{
    tree->insert(key, index);
    correct->insert(key, index);
}

void doubleAssign(Treap *tree, SlowStruct *correct, int minRange, int maxRange)
{
    int key = rnd.next(minRange, maxRange);
    size_t index = rnd.next(ZERO, correct->getSize() - 1u);
    tree->assign(key, index);
    correct->assign(key, index);
}

bool doubleGetSum(Treap *tree, SlowStruct *correct)
{
    int sumTree, sumCorrect;
    size_t l, r;
    l = rnd.next(ZERO, correct->getSize() - 1u);
    r = rnd.next(l + 1u, correct->getSize());
    sumTree = tree->getSegmentSum(l, r);
    sumCorrect = correct->getSegmentSum(l, r);
    return sumTree == sumCorrect;
}

void doubleNextPermutation(Treap *tree, SlowStruct *correct)
{
    tree->nextPermutation();
    correct->nextPermutation();
}

void vectorToTreeAndSlowStruct(std::vector <int> &testArray, Treap *tree, SlowStruct *correct)
{
    for (size_t i = 0; i < testArray.size(); i++)
        doubleInsert(tree, correct, testArray[i], i);
}

size_t generateTestType(size_t size)
{
    if (size > 1u)
        return rnd.next(ZERO, (size_t)OPERATIONS_NUMBER);
    else
        return test_type::INSERT;
}

bool singleTest(Treap *tree, SlowStruct *correct, int minRange, int maxRange)
{
    switch (generateTestType(correct->getSize()))
    {
    case INSERT:
        doubleInsert(tree, correct, rnd.next(rnd.next(minRange, maxRange)), rnd.next(ZERO, correct->getSize()));
    case ASSIGN:
        doubleAssign(tree, correct, minRange, maxRange);
    case GET_SUM:
        if (!doubleGetSum(tree, correct))
            return false;
    case NEXT_PERMUTATION:
        doubleNextPermutation(tree, correct);
    }

    return isEqual(tree, correct);
}

bool testingWithRanges(unsigned int numberOfTests, int minRange, int maxRange)
{
    bool result = true;
    size_t arraySize = rnd.next(MIN_ARRAY_SIZE, MAX_ARRAY_SIZE);
    std::vector <int> testArray = generateArray(arraySize, minRange, maxRange);
    Treap *tree = new Treap(new Node(0));
    SlowStruct *correct = new SlowStruct;
    correct->insert(0, 0);
    vectorToTreeAndSlowStruct(testArray, tree, correct);

    for (unsigned int i = 0; i < numberOfTests; i++)
        result &= singleTest(tree, correct, minRange, maxRange);

    delete correct;
    delete tree;
    return result;
}

bool testing(unsigned int numberOfTests)
{
    bool result = true;
    result &= testingWithRanges(numberOfTests, MIN_RANGE, MAX_RANGE);
    result &= testingWithRanges(numberOfTests, MIN_RANGE, LITTLE_MAX_RANGE);
    return result;
}
