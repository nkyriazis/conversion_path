#pragma once

#include "index_sequence_ops.hpp"
#include "traits.hpp"
#include <functional>

namespace conversion
{
template<typename Source, typename Target, typename Functions, typename Indices, typename Path>
struct conversion_path_impl;

template<typename Source, typename Target, typename Functions, typename Indices, std::size_t... path>
struct conversion_path_impl<Source, Target, Functions, Indices, std::index_sequence<path...>>
{
    struct op
    {
        template<typename Path, std::size_t index>
        struct apply
        {
            using function = typename std::tuple_element<index, Functions>::type;

            template<bool supports>
            struct proceed
            {
                using type =
                    typename conversion_path_impl<decltype(std::declval<function>()(std::declval<Source>())),
                                                  Target,
                                                  Functions,
                                                  typename index_sequence_ops::remove_index<Indices, index>::type,
                                                  std::index_sequence<path..., index>>::type;
            };

            template<>
            struct proceed<false>
            {
                using type = std::index_sequence<>;
            };
            using explore_this_option =
                typename proceed<traits::function_supports_arguments<function, Source>::value>::type;
            using type = std::conditional_t<(Path::size() < explore_this_option::size()), explore_this_option, Path>;
        };
    };
    using type = std::conditional_t<std::is_same<Source, Target>::value,
                                    std::index_sequence<path...>,
                                    typename index_sequence_ops::fold<Indices, std::index_sequence<>, op>::type>;
};

template<typename Source, typename Target, typename Functions>
struct compute_path
    : conversion_path_impl<Source,
                           Target,
                           Functions,
                           std::make_index_sequence<std::tuple_size<Functions>::value>,
                           std::index_sequence<>>
{
};

template<typename Source, typename Target, typename Functions>
struct converter
{
    using path = typename compute_path<Source, Target, Functions>::type;
    struct state
    {
        static Source convert(Source t, const Functions&) { return t; }
    };
    struct op
    {
        template<typename previous_state, std::size_t index>
        struct apply
        {
            struct type
            {
                template<typename T>
                static auto convert(T v, const Functions& fns)
                {
                    const auto& deepest_fun = std::get<index>(fns);
                    return deepest_fun(previous_state::convert(v, fns));
                }
            };
        };
    };
    using type = typename index_sequence_ops::fold<path, state, op>::type;
};

template<typename Target, typename Source, typename Functions>
Target
convert(Source&& source, const Functions& converters)
{
    using conv = typename converter<Source, Target, Functions>::type;
    return conv::convert(source, converters);
}
}
