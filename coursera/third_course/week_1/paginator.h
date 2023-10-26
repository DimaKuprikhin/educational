#pragma once
#include <vector>


template<typename Iterator>
class Paginator {
public:
    class Page {
    public:
        Page(Iterator first, Iterator last, size_t size)
            : first(first), last(last), size_(size) {}
        auto begin() const { return first; }
        auto end() const { return last; }
        size_t size() const { return size_; }
    private:
        const Iterator first;
        const Iterator last;
        const size_t size_;
    };

    Paginator(Iterator begin, Iterator end, size_t size) {
        if(begin == end) {
            return;
        }
        auto currBegin = begin;
        while(true) {
            auto currEnd = currBegin;
            for(size_t currSize = 0 ; currSize < size; ++currSize) {
                if(++currEnd == end) {
                    pages.push_back({ currBegin, currEnd, currSize + 1 });
                    return;
                }
            }
            pages.push_back({ currBegin, currEnd, size });
            currBegin = currEnd;
        }
    }
    
    auto begin() const { return pages.begin(); }
    auto end() const { return pages.end(); }
    
    size_t size() const {
        return pages.size();
    }

private:
    std::vector<Page> pages;
};

template<typename Container>
auto Paginate(const Container& container, const size_t size) {
    return Paginator(container.begin(), container.end(), size);
}

template<typename Container>
auto Paginate(Container& container, const size_t size) {
    return Paginator(container.begin(), container.end(), size);
}
