#ifndef CHESS_PRIORITYQUEUE_H
#define CHESS_PRIORITYQUEUE_H

#include <list>
#include <algorithm>
using namespace std;

template<typename T>
struct MyComparator {
    bool operator()(const T& a, const T& b) const {
        return a.first < b.first; // max-heap behavior: higher score = better
    }
};

template<typename T, typename Comparator = MyComparator<T>>
class PriorityQueue {
    list<T> queue;
    Comparator comp;
    static const size_t MAX_SIZE = 5;
public:
    void push(const T& value);
    T top() const;
    void pop();
    void clear();
    bool empty() const;
    size_t size() const;
};

template<typename T, typename Comparator>
void PriorityQueue<T, Comparator>::clear() {
    queue.clear();
}

template<typename T, typename Comparator>
bool PriorityQueue<T, Comparator>::empty() const {
    return queue.empty();
}

template<typename T, typename Comparator>
T PriorityQueue<T, Comparator>::top() const {
    return queue.front();
}

template<typename T, typename Comparator>
void PriorityQueue<T, Comparator>::pop() {
    if (!queue.empty())
        queue.pop_front();
}

template<typename T, typename Comparator>
size_t PriorityQueue<T, Comparator>::size() const {
    return queue.size();
}

template<typename T, typename Comparator>
void PriorityQueue<T, Comparator>::push(const T& value) {
    auto it = queue.begin();
    while (it != queue.end() && comp(*it, value)) {
        ++it;
    }
    queue.insert(it, value);
    if (queue.size() > MAX_SIZE)
        queue.pop_back();
}

#endif //CHESS_PRIORITYQUEUE_H