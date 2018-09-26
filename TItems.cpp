#include <algorithm>
#include <ctime>
#include <vector>

#include <assert.h>

struct TItem {
    int value;
    time_t timestamp;

    TItem(int v)
        : value(v)
        , timestamp(std::time(0))
    { }

};

using Items = std::vector<TItem>;

template <int... Values>
Items MakeItemsSimple()
{
    Items ret {TItem(Values)...};
    return ret;
}

auto MakePredicate(const Items& items)
{
    return [&items](int value) {
        auto it = std::find_if(items.begin(), items.end(),
                [value](TItem item){return item.value == value;});
        return it != items.end();
    };
}

int main() {
    Items items = MakeItemsSimple<0, 1, 2,  4, 5, 6>();
    Items newItems = MakeItemsSimple<7, 15, 1>();
    auto isFound = MakePredicate(items);
    auto isFoundNew = MakePredicate(newItems);
    assert(isFound(0) == true);
    assert(isFound(7) == false);
    assert(isFoundNew(7) == true);
    assert(isFoundNew(6) == false);
    return 0;
}
