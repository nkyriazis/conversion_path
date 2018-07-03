#include "conversion.hpp"
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

struct A
{
};
struct B
{
};
struct C
{
};
struct D
{
};
struct E
{
};
struct F
{
};

B AtoB(A)
{
    std::cout << __FUNCTION__ << std::endl;
    return {};
}
C BtoC(B)
{
    std::cout << __FUNCTION__ << std::endl;
    return {};
}
D CtoD(C)
{
    std::cout << __FUNCTION__ << std::endl;
    return {};
}
E DtoE(D)
{
    std::cout << __FUNCTION__ << std::endl;
    return {};
}
F EtoF(E)
{
    std::cout << __FUNCTION__ << std::endl;
    return {};
}

F f = EtoF(DtoE(CtoD(BtoC(AtoB(A{})))));

int
main(int argc, char** argv)
{
    try
    {
        auto f0 = [](A) -> B {
            std::cout << "A->B" << std::endl;
            return {};
        };
        auto f1 = [](B) -> C {
            std::cout << "B->C" << std::endl;
            return {};
        };
        auto f2 = [](C) -> D {
            std::cout << "C->D" << std::endl;
            return {};
        };
        auto f3 = [](D) -> E {
            std::cout << "D->E" << std::endl;
            return {};
        };
        auto f4 = [](E) -> F {
            std::cout << "E->F" << std::endl;
            return {};
        };
        auto f5 = [](A) -> F {
            std::cout << "A->F" << std::endl;
            return {};
        };

        conversion::convert<B>(A(), std::make_tuple(AtoB, BtoC, CtoD, DtoE, EtoF));
        // convert<D>(B(), std::make_tuple(f0, f1, f2, f3, f4));
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
