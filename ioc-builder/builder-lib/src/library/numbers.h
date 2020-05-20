#pragma once
#include <stdint.h>

namespace sample {
template <uint32_t N>
constexpr auto fibbonaci() {
	return fibbonaci<N - 1>() + fibbonaci<N - 2>();
}

template <>
constexpr auto fibbonaci<0>() {
	return 0;
}

template <>
constexpr auto fibbonaci<1>() {
	return 1;
}

}  // namespace sample