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

struct allBag()
{
    bagType type;
    bool isValid = true;
}

int main() 
{
    int operationNum, operation, number;
    while (scanf("%d", &operationNum) != EOF) 
    {
        queue<int> queueBag;
        stack<int> stackBag;
        priority_queue<int> priorityQueueBag;
        bool isStack = true, isQueue = true, isPriorityQueue = true;

        for (int i = 0; i < operationNum; i++)
        {
            scanf("%d %d", &operation, &number);

            switch (operation)
            {
            case PUSH:
                if (isStack)
                    stackBag.push(number);
                if (isQueue)
                    queueBag.push(number);
                if (isPriorityQueue)
                    priorityQueueBag.push(number);
                break;
            case POP:
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
                break;
            default:
                isStack = isQueue = isPriorityQueue = false;
                break;
            }
        }

        if (!isStack && !isQueue && !isPriorityQueue)
            cout << "impossible" << endl;
        else if (isPriorityQueue)
        {
            if (!isStack&& !isQueue)
                cout << "priority queue" << endl;
            else
                cout << "not sure" << endl;
        }
        else if (isStack == true)
            cout << "stack" << endl;
        else if (isQueue == true)
            cout << "queue" << endl;
    }
}