#include <ranges>
#include <algorithm>
#include <iostream>

#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <boost/container_hash/hash.hpp>


template <typename T>
struct TypePrinter
{
    const T& x;
    friend std::ostream& operator<<(std::ostream& os, const TypePrinter& p)
    {
        return os << p.x;
    }
};

template <typename Os, std::ranges::range T>
Os& print(Os& os, T const& range)
{
    if (std::empty(range)) return os << "{}";
    auto it = std::begin(range);
    os << '{' << TypePrinter{*it};
    for (++it; it != std::end(range); ++it) os << ',' << TypePrinter{*it};
    return os << '}';
}

template <typename T1, typename T2>
struct TypePrinter<std::pair<T1, T2>>
{
    const std::pair<T1, T2>& x;
    friend std::ostream& operator<<(std::ostream& os, const TypePrinter& p)
    {
        return os << '{' << TypePrinter<T1>{p.x.first} << ',' << TypePrinter<T2>{p.x.second} << '}';
    }
};


struct S {
    S(int a, int b = 0) : a(a), b(b) {}

    friend std::ostream& operator<<(std::ostream& os, const S& s)
    {
        return os << 'S' << '{' << s.a << ',' << s.b << '}';
    }

    bool operator==(const S&) const = default;

    int a;
    int b;
};

template <> struct std::hash<S>
{
    std::size_t operator()(const S& s) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.a);
        boost::hash_combine(seed, s.b);
        return seed;
    }
};

int main() {
    std::unordered_map<S, int> a{{1,1},{2,2}};
    print(std::cout, a) << '\n';
    a[3] = 7;
    print(std::cout, a) << '\n';

    a[3] = 7;
    print(std::cout, a) << '\n';
}


template <typename T>
struct vector_ {

    vector_(size_t size) : begin_(new T[size]), end_(begin_), capacity_(begin_ + size) {}

    T* begin_;
    T* end_;
    T* capacity_;

    bool empty() const { return begin_ == end_; }
    bool size() const { return end_ - begin_; }
};


template <typename T>
struct list_
{
    struct node_base_
    {
        node_base_* next_;
        node_base_* prev_;
    };
    struct node_ : node_base_
    {
        alignas(T) std::byte value_[sizeof(T)];
    };  

    node_base_ head_;
};

template <typename T, std::size_t N>
struct array_
{
    T data_[N];
    T* begin() const { return data_; }
    T* end() const { return data_ + N; }
};

template <typename T>
struct deque_
{
    using chunk_ = array_<T, 8>;
    vector_<chunk_*> chunks_;
};