#ifndef CHESS_PRIORITYQUEUE_H
#define CHESS_PRIORITYQUEUE_H

#include <list>
#include <algorithm>
#include <mutex>
#include <vector>

template<typename T>
struct MyComparator {
    bool operator()(const T& a, const T& b) const {
        return a.first < b.first; // max-heap behavior
    }
};

template<typename T, typename Comparator = MyComparator<T>>
class PriorityQueue {
    std::list<T> queue;
    Comparator comp;
    mutable std::mutex m_mutex; // Added
    static const size_t MAX_SIZE = 5;

public:
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(m_mutex); // Added lock
        auto it = queue.begin();
        while (it != queue.end() && comp(*it, value)) {
            ++it;
        }
        queue.insert(it, value);
        if (queue.size() > MAX_SIZE) {
            queue.pop_back();
        }
    }

    T top() const {
        std::lock_guard<std::mutex> lock(m_mutex); // Added lock
        return queue.front();
    }

    void pop() {
        std::lock_guard<std::mutex> lock(m_mutex); // Added lock
        if (!queue.empty()) {
            queue.pop_front();
        }
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m_mutex); // Added lock
        return queue.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(m_mutex); // Added lock
        return queue.size();
    }

    std::vector<T> to_vector() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return std::vector<T>(queue.begin(), queue.end());
    }
};

#endif //CHESS_PRIORITYQUEUE_H