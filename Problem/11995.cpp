#include <fstream>
#include <iostream>
#include <cstdio>
#include <stack>
#include <queue>
using namespace std;
#define PUSH 1
#define POP 2

enum class bagType {
    Stack = 0,
    Queue,
    PriorityQueue,
    None
};

struct AllBag
{
    queue<int> queueBag;
    stack<int> stackBag;
    priority_queue<int> priorityQueueBag;
    bool isStack = true, isQueue = true, isPriorityQueue = true;

    void push(int number)
    {
        if (isStack)
            stackBag.push(number);
        if (isQueue)
            queueBag.push(number);
        if (isPriorityQueue)
            priorityQueueBag.push(number);
    }

    void pop(int number)
    {
        if (isStack)
        {
            if (stackBag.empty() || stackBag.top() != number)
                isStack = false;
            else
                stackBag.pop();
        }
        if (isQueue)
        {
            if (queueBag.empty() || queueBag.front() != number)
                isQueue = false;
            else
                queueBag.pop();
        }
        if (isPriorityQueue)
        {
            if (priorityQueueBag.empty() || priorityQueueBag.top() != number)
                isPriorityQueue = false;
            else
                priorityQueueBag.pop();
        }
    }

    void markWrongBag()
    {
        isStack = isQueue = isPriorityQueue = false;
    }

    void whichBag()
    {
        if (!isStack && !isQueue && !isPriorityQueue)
            cout << "impossible" << endl;
        else if (isPriorityQueue)
        {
            if (!isStack && !isQueue)
                cout << "priority queue" << endl;
            else
                cout << "not sure" << endl;
        }
        else if (isStack == true)
        {
            if (!isQueue)
                cout << "stack" << endl;
            else
                cout << "not sure" << endl;
        }
        else if (isQueue == true)
            cout << "queue" << endl;
    }

    bool isImposible()
    {
        return (!isStack && !isQueue && !isPriorityQueue);
    }
};

int main()
{
    int operationNum = 0;
    while (cin >> operationNum)
    {
        AllBag wholeBag;
        for (int i = 0; i < operationNum; i++)
        {
            int operation = 0;
            int number = 0;
            cin >> operation >> number;

            switch (operation)
            {
            case PUSH:
                wholeBag.push(number);
                break;
            case POP:
                wholeBag.pop(number);
                break;
            default:
                wholeBag.markWrongBag();
                break;
            }
        }
        wholeBag.whichBag();
    }

    return 0;
}