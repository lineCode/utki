#pragma once

#include <vector>

template <class T, template <class, class> class C = std::vector, class A = std::allocator<T> > class tree :
        public C<tree<T, C, A>, A>
{
    T value;
public:

    typedef C<tree, A> container_type;

    tree() = default;

    tree(const tree&) = default;

    tree& operator=(const tree&) = default;

    tree(tree&& t) = default;

    tree(std::initializer_list<tree> l) :
            C<tree, A>(l)
    {}

    tree(const T& value, std::initializer_list<tree> l) :
            C<tree, A>(l),
            value(value)
    {}

    tree(const T& value) :
            value(value)
    {}

    tree(T&& value) :
            value(std::move(value))
    {}

    T& get(){
        return this->value;
    }

    const T& get()const{
        return this->value;
    }
};
