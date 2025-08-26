#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
using namespace std;

int main() {
    //Vector
    vector<int> v = {1, 2, 3, 4, 5};
    // Accessing elements
    cout << "Vector: ";
    for (int i : v) cout << i << " ";
    cout << "\n";

    // Adding elements
    v.push_back(6);

    // Removing elements
    v.pop_back();


    //List
    list<int> lst = {1, 2, 3, 4, 5};
    // Accessing elements
    cout << "List: ";
    for (int i : lst) cout << i << " ";
    cout << "\n";

    // Adding elements
    lst.push_back(6);
    lst.push_front(0);

    // Removing elements
    lst.pop_back();
    lst.pop_front();

    //Dequeue
    deque<int> dq = {1, 2, 3, 4, 5};
    // Accessing elements
    cout << "Deque: ";
    for (int i : dq) cout << i << " ";
    cout << "\n";

    // Adding elements
    dq.push_back(6);
    dq.push_front(0);

    // Removing elements
    dq.pop_back();
    dq.pop_front();

    //Set
    set<int> s = {1, 2, 3, 4, 5};
    // Accessing elements
    cout << "Set: ";
    for (int i : s) cout << i << " ";
    cout << "\n";

    // Adding elements
    s.insert(6);

    // Removing elements
    s.erase(1);

    //Map
    map<string, int> m = {{"apple", 1}, {"banana", 2}, {"cherry", 3}};
    // Accessing elements
    cout << "Map: ";
    for (auto &p : m) cout << "(" << p.first << ", " << p.second << ") ";
    cout << "\n";

    // Adding elements
    m["date"] = 4;

    // Removing elements
    m.erase("apple");

    //Unordered Map
    unordered_map<string, int> um = {{"apple", 1}, {"banana", 2}, {"cherry", 3}};
    // Accessing elements
    cout << "Unordered Map: ";
    for (auto &p : um) cout << "(" << p.first << ", " << p.second << ") ";
    cout << "\n";

    // Adding elements
    um["date"] = 4;

    // Removing elements
    um.erase("apple");
    return 0;
}