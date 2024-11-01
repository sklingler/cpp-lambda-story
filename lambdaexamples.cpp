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

int main()
{
    simpleExample();
    lambdaParam();
    memVarExample();
    returnTypes();
    captureInitializer();

    return 0;
}

