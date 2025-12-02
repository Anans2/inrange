// The MIT License (MIT)

// Copyright (c) 2023 Anans

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

#pragma once
#ifndef __IN_RANGE__
#define __IN_RANGE__

namespace inrange
{
    // 惰性求值列表中的仿迭代器
    class CounterIterator
    {
    public:
        using counter_t = long long int;
    private:
        counter_t number;
        const counter_t step; // 包含符号的步长
    public:
        constexpr CounterIterator(const counter_t, const counter_t);
        constexpr CounterIterator &operator++();
        constexpr CounterIterator operator++(int);
        constexpr counter_t operator*() const;
        constexpr bool operator==(const CounterIterator &) const;
        constexpr bool operator!=(const CounterIterator &) const;
        ~CounterIterator() = default;
    };

    constexpr CounterIterator::CounterIterator(const counter_t _number, const counter_t _step)
        : step(_step), number(_number) {}

    constexpr CounterIterator &CounterIterator::operator++()
    {
        number += step;
        return *this;
    }

    constexpr CounterIterator CounterIterator::operator++(int)
    {
        auto ret = *this;
        number += step;
        return ret;
    }

    constexpr CounterIterator::counter_t CounterIterator::operator*() const
    {
        return number;
    }

    constexpr bool CounterIterator::operator==(const CounterIterator &another) const
    {
        return step > 0 ? (number - another.number >= 0) ^ (number - step - another.number > 0)
                        : (number - another.number <= 0) ^ (number - step - another.number < 0);
    }

    constexpr bool CounterIterator::operator!=(const CounterIterator &another) const
    {
        return !((*this) == another);
    }

    // 惰性求值列表,实现了一个仿序列
    class LazyEvaluationList
    {
    public:
        using value_type = CounterIterator::counter_t;
        using iterator = CounterIterator;
        using const_iterator = const CounterIterator;
        using step_t = unsigned int; // 不带符号的步长类型
    private:
        const_iterator begin_iter, end_iter;
    public:
        constexpr LazyEvaluationList(const value_type, const value_type, const step_t);
        constexpr iterator begin() const;
        constexpr iterator end() const;
        constexpr const_iterator &cbegin() const;
        constexpr const_iterator &cend() const;
        ~LazyEvaluationList() = default;
    };

    constexpr LazyEvaluationList::LazyEvaluationList(const value_type _begin_num,
                                                     const value_type _end_num,
                                                     const step_t _step_long)
        : begin_iter(_begin_num, (_begin_num <= _end_num ? _step_long : -CounterIterator::counter_t(_step_long))),
          end_iter(_end_num, (_begin_num <= _end_num ? _step_long : -CounterIterator::counter_t(_step_long))) {}
    // note:如果不转换为有符号的较宽类型,则步长可能溢出

    constexpr LazyEvaluationList::iterator LazyEvaluationList::begin() const
    {
        return begin_iter;
    }

    constexpr LazyEvaluationList::iterator LazyEvaluationList::end() const
    {
        return end_iter;
    }

    constexpr LazyEvaluationList::const_iterator &LazyEvaluationList::cbegin() const
    {
        return begin_iter;
    }

    constexpr LazyEvaluationList::const_iterator &LazyEvaluationList::cend() const
    {
        return end_iter;
    }
}

// for接口
constexpr inline inrange::LazyEvaluationList in(const inrange::LazyEvaluationList::value_type begin,
                                                const inrange::LazyEvaluationList::value_type end,
                                                const inrange::LazyEvaluationList::step_t step = 1)
{
    return inrange::LazyEvaluationList(begin, end, step);
}

#endif

