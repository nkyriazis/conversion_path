# conversion_path
Compile time computation of conversion path from given converters

There are cases where using simple types can be confusing during lack of semantics. This can be remedied by using rich type programming. An example of what the problem may be and what a solution is can be seen at https://www.codeproject.com/Articles/723900/Type-Rich-Style-for-Cplusplus.

In such a scenario, there might be several types and several converters which can convert directly from one type to the other. The goal here is, given a set of converters, to automate the conversion path computation for the programmer. Example:

Without loss of generality let's assume a series of types:
```cpp
struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};
```

Let's also assume some converters:
```cpp
B AtoB(A) { std::cout << __FUNCTION__ << std::endl; return {}; }
C BtoC(B) { std::cout << __FUNCTION__ << std::endl; return {}; }
D CtoD(C) { std::cout << __FUNCTION__ << std::endl; return {}; }
E DtoE(D) { std::cout << __FUNCTION__ << std::endl; return {}; }
F EtoF(E) { std::cout << __FUNCTION__ << std::endl; return {}; }
```

If one wanted to convert A to F then one would need to write:

```cpp
F f = EtoF(DtoE(CtoD(BtoC(AtoB(A{})))));
```

If the conversions are limited then perhaps it's not as bad. But, if the types are many, the converters are also expected to be many. And, perhaps, not all paths exist to convert from any type to any other type. This lib helps take the information in the simplest form providable and compute this for the programmer. In this example, with this lib, the specific conversion, or any other conversion, would be written as:

```cpp
F f = convert<F>(A(), std::make_tuple(AtoB, BtoC, CtoD, DtoE, EtoF));
```

I.e., the programmer provides an instance of the source type, the target type and a tuple of all available converters. The method then computes a compile time ordering of a subset of the converters and applies it, or yields an error if the conversion cannot be made.
