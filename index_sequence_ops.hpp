#pragma once

#include <type_traits>

namespace index_sequence_ops
{

template<typename Previous, typename Rest, std::size_t remove>
struct remove_impl;

template<std::size_t... previous, std::size_t remove>
struct remove_impl<std::index_sequence<previous...>, std::index_sequence<>, remove>
{
    using type = std::index_sequence<previous...>;
};

template<std::size_t... previous, std::size_t current, std::size_t... rest, std::size_t remove>
struct remove_impl<std::index_sequence<previous...>, std::index_sequence<current, rest...>, remove>
{
    using type = typename remove_impl<std::conditional_t<current == remove,
                                                         std::index_sequence<previous...>,
                                                         std::index_sequence<previous..., current>>,
                                      std::index_sequence<rest...>,
                                      remove>::type;
};

template<typename Indices, std::size_t index>
struct remove_index : remove_impl<std::index_sequence<>, Indices, index>
{
};

template<typename Indices, std::size_t index>
struct push_back;

template<std::size_t... index, std::size_t tail>
struct push_back<std::index_sequence<index...>, tail>
{
    using type = std::index_sequence<index..., tail>;
};

template<typename Indices>
struct reverse_indices;

template<>
struct reverse_indices<std::index_sequence<>>
{
    using type = std::index_sequence<>;
};

template<std::size_t head, std::size_t... index>
struct reverse_indices<std::index_sequence<head, index...>>
{
    using type = typename push_back<typename reverse_indices<std::index_sequence<index...>>::type, head>::type;
};

template<typename Indices, typename State, typename Op>
struct fold_impl;

template<typename State, typename Op>
struct fold_impl<std::index_sequence<>, State, Op>
{
    using type = State;
};

template<std::size_t head, std::size_t... rest, typename State, typename Op>
struct fold_impl<std::index_sequence<head, rest...>, State, Op>
{
    using type =
        typename Op::template apply<typename fold_impl<std::index_sequence<rest...>, State, Op>::type, head>::type;
};

template<typename Indices, typename State, typename Op>
struct fold : fold_impl<typename reverse_indices<Indices>::type, State, Op>
{
};
}