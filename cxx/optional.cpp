#include <optional>
#include <iostream>
// #include <utility>
#include <functional>  // error: invalid use of incomplete type ‘class std::reference_wrapper<const X>’

// #define AAA
// #define BBB
#define CCC

class Y
{
public:
    void foo() const {}
};

#ifdef AAA
template <typename P>
void foo(const int i, std::optional<P> p = std::nullopt)
{}
#endif


#ifdef BBB
template <typename P>
size_t fooa(const int i, std::optional<P> & p = std::nullopt)
{
    if (p.has_value())
        return sizeof(P);
    else
        return 0;
}
#endif

#ifdef CCC
class X {

public:
    X()
    {
        std::cout << "Constructor" << std::endl;
    }

    ~X()
    {
        std::cout << "Destructor" << std::endl;
    }

    void foo() const
    {
        std::cout << "Foo" << std::endl;
    }

    X(const X &x)
    {
        std::cout << "Copy constructor" << std::endl;
    }

    X &operator=(const X &)
    {
        std::cout << "operator=" << std::endl;
        return *this;
    }
};

template <typename P>
void baz(const int i, const std::optional<std::reference_wrapper<const P>> arg)
{
    if (arg)
        arg->get().foo();
    if (arg.has_value())      ;
    //    arg.value().foo();  // error: ‘class std::reference_wrapper<const X>’ has no member named ‘foo’
}
#endif

int main(int argc, char ** argv)
{
#ifdef AAA
    foo(0);
    foo(0, 3);
#endif
#ifdef BBB
    Y bar;
    foo<decltype(bar)>(0, {bar});             // ok
    foo(0, {bar});                            // ok
    foo<std::nullopt_t>(0);                   // err
    // foo<std::nullopt_t>(0, std::nullopt);  // err
#endif
#ifdef CCC
    X x;
    baz<X>(0, std::nullopt);
    baz<X>(0, x);
    baz<Y>(0, std::nullopt);
#endif
    return 0;
}
