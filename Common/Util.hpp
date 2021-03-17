#pragma once

#include <Windows.h>

#include <sstream>
#include <concepts>
#include <functional>
#include <utility>



#define TRY(exp) { SetLastError(0); exp; DWORD err{ GetLastError() }; if (err) {\
std::string str{ comm::errorAsString(err) }; throw std::runtime_error{ str };} }


namespace comm
{
template <typename ... Args> struct match : Args... { using Args::operator()...; };
template <typename ... Args> match(Args...) -> match<Args...>;


template <typename T, typename From, typename To>
concept Converter = requires (T converter, From from)
{
	{ converter(from) } -> std::same_as<To>;
};

template <typename From, typename To>
To defaultConverter(From from)
{
	return To{ from };
}

template <typename From, typename To>
using DefaultConverter = decltype(&defaultConverter<From, To>);


std::string errorAsString(DWORD error = S_OK);


template <typename ... Args>
std::wstring toWString(Args&&... args)
{
	std::wstringstream stream;
	stream << std::boolalpha;
	(stream << ... << std::forward<Args>(args));

	return std::move(stream.str());
}

template <typename ... Args>
std::string toString(Args&&... args)
{
	std::stringstream stream;
	stream << std::boolalpha;
	(stream << ... << std::forward<Args>(args));

	return std::move(stream.str());
}


double distance(POINT a, POINT b);
}
