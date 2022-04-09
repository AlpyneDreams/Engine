#pragma once

#include <utility>
#include <iterator>

namespace engine
{
    /** Simple iterable range on interval [first, last)
     * Can be used with e.g. pairs returned by std::multimap::equal_range
     */
    template <std::bidirectional_iterator Iterator>
    struct IteratorRange
    {
        Iterator first, last;

        IteratorRange(std::pair<Iterator, Iterator> pair) : first(pair.first), last(pair.second) {}
        IteratorRange(Iterator first, Iterator last) : first(first), last(last) {}
        
        Iterator begin() const { return first; }
        Iterator end() const { return last; }
    };
}