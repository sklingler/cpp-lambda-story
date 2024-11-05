#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <map>
#include <iomanip>
#include <algorithm>

void simpleExample()
{
    // This simple statement replaces having a class/struct with an integer operator that 
    // performs whatever action.
    const std::vector<int> v{1,2,3,4};
    std::for_each(v.cbegin(), v.cend(), 
        [] (int x) {
            std::cout << "X in vec: " << x << std::endl;
        });

    // Why is this better than?
    for( auto i: v)
    {
        std::cout << "second method i: " << i << std::endl;
    }

    // Declaring the lambda as a variable...
    auto simpleLambda = [] (int x) {
            std::cout << "simpleLambda X in vec: " << x << std::endl;
        };
    std::for_each(v.cbegin(), v.cend(), simpleLambda);

    std::cout << "calling simpleLambda manually" << std::endl;
    simpleLambda(33);

    // Lambda w/x by reference. Demonstrates using a variable from
    // outside the lambda. Yes, you could specify more than s1
    std::string s1{"Reference Lambda - "};
    auto refLambda = [&s1] (int xx) {
        //std::cout << "refLambda xx: " << xx << std::endl;
        std::cout << s1 << " xx: " << xx << std::endl;
    };
    std::for_each(v.cbegin(), v.cend(), refLambda);

    // All outside variables by reference, not specified
    // [&] - all variables by reference
    // [=] - all variables copied
    //
    // This captures all automatic storage variables by
    // reference. The author calls it "convenient" to do
    // it this way, but cleaner to be explicit (vs all)
    auto refLambda2 = [&] (int xx) {
        //std::cout << "refLambda xx: " << xx << std::endl;
        std::cout << s1 << " xx: " << xx << std::endl;
    };
    std::for_each(v.cbegin(), v.cend(), refLambda2);

    // Being mutable. We'll capture two variables this time
    // We also increment the integer. If we don't add the
    // reference to the capture for captInt, it's only local
    // and the original value is printed below.
    int captInt{55};
    auto refLambda3 = [&s1,&captInt] (int xx) mutable {
        //std::cout << "refLambda xx: " << xx << std::endl;
        std::cout << s1 << " xx: " << xx << std::endl;
        ++captInt;
    };
    std::for_each(v.cbegin(), v.cend(), refLambda3);
    std::cout << "After lambda3 captInt: " << captInt << std::endl;
}

// Pass a lambda as a parameter
void lambdaParam()
{
    std::cout << std::endl << "lambdaParam section" << std::endl;
    const auto fooPrint = [] (const char* str, int xx, int yy) {
        std::cout << "fooPrint: " << str << "   x: " << xx << "  y: " << yy << std::endl;
    };


    int x{0}, y{10};
    auto foo = [x, &y, &fooPrint] (int vVal) mutable {
        ++x; ++y;
        fooPrint("calling fooPrint", x, y);
    };
    const std::vector<int> v{1,2,3};
    std::for_each(v.cbegin(), v.cend(), foo);
    // I can just call this directly if I take off the vvAl above.
    //foo();

    // The values printed here are different based on whether x/y are reference.
    std::cout << "X after: " << x << "  Y after: " << y << std::endl;
}

// Capturing a member variable
struct Baz
{
    void foo()
    {
        // Capturing "this" is what allows the lambda to get to "s"
        // this->s and "s" seem to be equivalent.
        const auto lMem = [this] () {std::cout << " s: " << /*this->*/s << std::endl;};
        lMem();

        // Note: later in the book on capturing a class member, he showed another way to 
        // accomplish this.
        // const auto lMem = [s=s] ()...
        // const auto lMem = [z=s] ()....
        // Both of the above do the same thing, but name it differently
        // Want it by reference so you can modify s?
        // const auto lMem = [&z=s] () ...
    }

    std::string s{"Member Variable Str"};
};

void memVarExample()
{
    std::cout << std::endl << "memVarExample" << std::endl;
    Baz b;
    b.foo();
}


// auto determining the return type
void returnTypes()
{
    std::cout << std::endl << "returnTypes...." << std::endl;
    // Also show that default parameters work here
    auto fooFunc = [] (int x, int y = 10) {
        return x * y;
    };

    auto f1 = fooFunc(5);
    std::cout << "fooFunc(5) returned: " << f1 << std::endl;

    auto fooFunc2 = [] (int x, float y = 10.4976f) {
        return x * y;
    };
    auto f2 = fooFunc2(20);
    std::cout << "fooFun2(20) returned: " << f2 << std::endl;
}

void captureInitializer()
{
    std::cout << std::endl <<  "captureInitializer()" << std::endl;
    int x{30}, y{32};
    // Capture x and Y into a new z and it's "auto"
    const auto foo = [z = x + y] (int mult = 2) {
        return z * mult;
    };

    auto f1 = foo();
    auto f2 = foo(3);
    std::cout << "captInt f1: " << f1 << "  f2: " << f2 << std::endl;
}

template<typename T>
auto sum(T x) {return x;}

template<typename T1, typename... T>
auto sum(T1 s, T... ts) { return s+ sum(ts...);}

// for forwarding
void foofwd(const std::string& s) {std::cout << "foofwd(const string&)" << std::endl;}
void foofwd(std::string&& s) {std::cout << "foofwd(std::string&&)" << std::endl; }

void genericLambdas()
{
    // C++ 14
    // This is kinda like a template. x takes the type of whatever is passed in.
    const auto foo = [](auto x, int y) {
        std::cout << "foo x: " << x << "  y: " << y << std::endl;
    };

    foo(10, 1);
    foo(100.123, 2);
    foo("hello world string", 3);

    // A variadic example
    const auto sumGeneric = [](auto... args) {
        std::cout << "sum of: " << sizeof...(args) << " numbers" << std::endl;
        return sum(args...);
    };

    std::cout << "sum 1: " << sumGeneric(1.1, 2.2, 3.3) << std::endl;
    std::cout << "sum 2: " << sumGeneric(4, 5, 6, 7) << std::endl;

    // C++17 has an update to variadic generic lambdas
    // I didn't need the 2 templates for sum() like above.
    const auto sumGeneric17 = [](auto... args) {
        std::cout << "sumgeneric17 of: " << sizeof...(args) << " numbers" << std::endl;
        return(args + ... + 0);
    };
    std::cout << "sumGeneric17(1.1, 2.2, 3.3)" << sumGeneric17(1.1, 2.2, 3.3) << std::endl;

    const auto printer = [](auto... args) {
        (std::cout << ... << args) << std::endl;
    };
    printer(1,2,3,"hello", 10.5f);

    // add spaces, nested lambda
    const auto printer2 = [](auto... args) {
        const auto printElem = [](auto elem) {
            std::cout << elem << ", ";
        };
        (printElem(args), ...);
        std::cout << std::endl;
    };
    printer2(1,2,3,"hello", 10.5f);

    // and then shorten it
    const auto printer3 = [](auto... args) {
        ((std::cout<< args << ", "), ...);
        std::cout << std::endl;
    };
    printer3(1,2,3,"hello", 10.5f);

    // ... and then drop last comma
    const auto printer4 = [](auto first, auto... args) {
        std::cout << first;
        ((std::cout << ", " << args), ...);
        std::cout << std::endl;
    };
    printer4(1,2,3,"hello", 10.5f);

    // C++20 template lambdas

    // perfect forwarding...
    const auto callFoo = [](auto&& s) {
        std::cout << "Calling foofwd() on: " << s << std::endl;
        foofwd(std::forward<decltype(s)>(s));
    };
    const std::string s{"Hello World"};
    callFoo(s);
    callFoo("Hello World Ref Ref");
}

// The ugliness from bind1st(), bind2nd(), mem_fun(), mem_fun_ref(), etc... were hell in c++98/03.
void binding()
{
    // C++11. Better than above functions that were flat out hard at times
    using std::placeholders::_1;
    const auto onePlus = std::bind(std::plus<int>(), _1, 1);
    const auto minusOne = std::bind(std::minus<int>(), _1, 1);

    std::cout << "onePlus(10): " << onePlus(10) << ", minusOne(10): " << minusOne(10) << std::endl;

    // ... and now a lambda version
    auto lamOnePlus = [](int b) {
        return 1 + b;
    };
    auto lamMinusOne = [](int b) {
        return b - 1;
    };
    std::cout << "lamOnePlus(10): " << lamOnePlus(10) << ", lamMinusOne(10): " << lamMinusOne(10) << std::endl;

    // use a capture initializer
    auto lamOnePlusA = [a=1](int b) {
        return a + b;
    };

    std::cout << "lamOnePlusA(10): " << lamOnePlusA(10) << std::endl;

    // use a capture initializer
    int c{2};
    auto lamOnePlusC = [c](int b) {
        return c + b;
    };

    std::cout << "lamOnePlusC(10): " << lamOnePlusC(10) << std::endl;

    // nested example, c++11
    const std::vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const auto val = std::count_if(v.begin(), v.end(),
        std::bind(std::logical_and<bool>(), std::bind(std::greater<int>(), _1, 2),
        std::bind(std::less<int>(), _1, 6)));
    std::cout << "count_if v: " << val << std::endl;

    // lambda version... and I broke it into 2 lambdas...
    const auto more2less6compare = [](int x){ return x > 2 && x < 6;};
    const auto more2less6 = std::count_if(v.begin(), v.end(),
        //[](int x) {return x > 2 && x < 6;});
        more2less6compare);
    std::cout << "count_if v lambda: << " << more2less6 << std::endl;
}

int main()
{
    simpleExample();
    lambdaParam();
    memVarExample();
    returnTypes();
    captureInitializer();
    genericLambdas();
    binding();

    return 0;
}

