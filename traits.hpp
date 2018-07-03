#pragma once

#include <type_traits>

namespace traits
{
template<typename Function, typename... Args>
struct function_supports_arguments_impl
{
    template<typename Function, typename... Args>
    static constexpr std::true_type test(decltype(std::declval<Function>()(std::declval<Args>()...)) *);
    template<typename Function, typename... Args>
    static constexpr std::false_type test(...);
    using type = decltype(test<Function, Args...>(nullptr));
};

template<typename Function, typename... Args>
using function_supports_arguments = typename function_supports_arguments_impl<Function, Args...>::type;
}