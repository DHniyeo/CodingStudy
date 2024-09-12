#include <iostream>
#include <vector>
#include "Vector.h"
#include <list>
#include <algorithm>
#include "List.h"
#include "Stack.h"
#include <queue>
#include "Queue.h"
#include "PriorityQueue.h"

using namespace std;



int main()
{
    // Vector
    {
        cout << "Vector" << endl;
        Vector<int> v;
        vector<int> v2;
        
        //v.reserve(100);

        for (int i = 0; i < 100; i++)
        {
            v.push_back(i);
            cout << v.size() << " " << v.capacity() << endl;
        }

        for (int i = 0; i < v.size(); i++)
        {
            cout << v[i] << endl;
        }

        cout << "------------------" << endl;

        for (Vector<int>::iterator it = v.begin(); it != v.end(); ++it)
        {
            cout << (*it) << endl;
        }
    }

    //List
    {
        cout << "List" << endl;

        List<int> li;

        List<int>::iterator eraseIt;

        for (int i = 0; i < 10; i++)
        {
            if (i == 5)
            {
                eraseIt = li.insert(li.end(), i);
            }
            else
            {
                li.push_back(i);
            }
        }

        li.pop_back();

        li.erase(eraseIt);

        for (List<int>::iterator it = li.begin(); it != li.end(); ++it)
        {
            cout << (*it) << endl;
        }
    }
    
    //stack
    {
        cout << "stack" << endl;
        Stack<int,list<int>> s;

        // 삽입
        s.push(1);
        s.push(2);
        s.push(3);
        

        // 비었나?
        while (s.empty() == false)
        {
            // 최상위 원소
            int data = s.top();
            // 최상위 원소 삭제
            s.pop();

            cout << data << endl;
        }

        auto size = s.size();
    }

    // queue
    {
        cout << "queue" << endl;

        //ArrayQueue<int> q;
        Queue<int,deque<int>> q;

        for (int i = 0; i < 100; i++)
            q.push(i);

        while (q.empty() == false)
        {
            int value = q.front();
            q.pop();
            cout << value << endl;
        }

        int size = q.size();
    }
    cout << "PriorityQueue" << endl;
    {
        PriorityQueue<int, vector<int>, greater<int>> pq;

        pq.push(100);
        pq.push(300);
        pq.push(200);
        pq.push(500);
        pq.push(400);

        while (pq.empty() == false)
        {
            int value = pq.top();
            pq.pop();

            cout << value << endl;
        }




    }


    return 0;
}


