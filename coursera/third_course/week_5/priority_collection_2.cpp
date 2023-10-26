#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_map>
#include <memory>
#include <list>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection
{
public:
    using Id = size_t;

    Id Add(T object) {
        values.push_back(std::move(object));
        auto valueIter = values.end();
        --valueIter;
        valueById[id] = valueIter;
        valueByPriority[{ 0, currentTime, id }] = valueIter;
        priorityById[id] = { 0, currentTime++, id };
        return id++;
    }

    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
        for(auto it = range_begin; it != range_end; ++it) {
            *ids_begin++ = Add(std::move(*it));
        }
    }

    bool IsValid(Id id) const {
        return valueById.find(id) != valueById.end();
    }

    const T &Get(Id id) const {
        return *valueById.at(id);
    }

    void Promote(Id id) {
        auto priority = priorityById[id];
        const auto valueIter = valueByPriority[priority];
        valueByPriority.erase(priority);
        ++priority.priority;
        ++priorityById[id].priority;
        valueByPriority[priority] = valueIter;
    }

    pair<const T &, int> GetMax() const {
        uint priority = static_cast<int>(((*valueByPriority.begin()).first).priority);
        return { *(valueByPriority.begin()->second), priority };
    }

    pair<T, int> PopMax() {
        auto valueIter = valueByPriority.begin()->second;
        auto id = valueByPriority.begin()->first.id;
        pair<T, int> result(std::move(*valueIter), valueByPriority.begin()->first.priority);
        valueByPriority.erase(valueByPriority.begin());
        values.erase(valueIter);
        priorityById.erase(id);
        valueById.erase(id);
        return result;
    }

private:
    std::list<T> values;
    std::unordered_map<Id, typename std::list<T>::iterator> valueById;
    struct PriorityTime {
        uint64_t priority;
        uint64_t time;
        Id id;
    };
    struct PriorityTimeLess {
        bool operator()(const PriorityTime& lhs, const PriorityTime& rhs) const {
            return lhs.priority > rhs.priority || (lhs.priority == rhs.priority && lhs.time > rhs.time);
        } 
    };
    std::map<PriorityTime, typename std::list<T>::iterator, PriorityTimeLess> valueByPriority;
    std::unordered_map<Id, PriorityTime> priorityById;
    uint64_t id = 0;
    uint64_t currentTime = 0;

};

class StringNonCopyable : public string
{
public:
    using string::string; // РџРѕР·РІРѕР»СЏРµС‚ РёСЃРїРѕР»СЊР·РѕРІР°С‚СЊ РєРѕРЅСЃС‚СЂСѓРєС‚РѕСЂС‹ СЃС‚СЂРѕРєРё
    StringNonCopyable(const StringNonCopyable &) = delete;
    StringNonCopyable(StringNonCopyable &&) = default;
    StringNonCopyable &operator=(const StringNonCopyable &) = delete;
    StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

void TestNoCopy()
{
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i)
    {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}