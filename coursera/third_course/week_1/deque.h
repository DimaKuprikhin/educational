#pragma once
#include <stdexcept>
#include <vector>
#include <iostream>

template<typename T>
class Deque {
public:
    Deque() {}

    bool Empty() const { return front.empty() && back.empty(); }

    size_t Size() const { return front.size() + back.size(); }

    T& operator[](const size_t index) {
        if(index < front.size()) {
            return front[front.size() - 1 - index];
        }
        return back[index - front.size()];
    }

    const T& operator[](const size_t index) const {
        if(index < front.size()) {
            return front[front.size() - 1 - index];
        }
        return back[index - front.size()];
    }

    T& At(const size_t index) {
        if(index >= Size()) {
            throw std::out_of_range("");
        }
        return this->operator[](index);
    }

    const T& At(const size_t index) const {
        if(index >= Size()) {
            throw std::out_of_range("");
        }
        return this->operator[](index);
    }

    T& Front() {
        if(front.empty()) {
            return back.front();
        }
        return front.back();
    }

    const T& Front() const {
        if(front.empty()) {
            return back.front();
        }
        return front.back();
    }

    T& Back() {
        if(back.empty()) {
            return front.front();
        }
        return back.back();
    }

    const T& Back() const {
        if(back.empty()) {
            return front.front();
        }
        return back.back();
    }

    void PushFront(const T& value) {
        front.push_back(value);
    }

    void PushBack(const T& value) {
        back.push_back(value);
    }

private:
    std::vector<T> front;
    std::vector<T> back;
};
