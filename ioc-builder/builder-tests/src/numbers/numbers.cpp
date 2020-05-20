#include "gtest/gtest.h"
#include "builder-lib/builder.h"

TEST(fibbonaci, compile_time_calculation) {
	static_assert(sample::fibbonaci<7>() == 13, "calculations should be possible at compile-time");
}

TEST(fibbonaci, expected_range) {
	static_assert(sample::fibbonaci<0>() == 0);
	static_assert(sample::fibbonaci<1>() == 1);
	static_assert(sample::fibbonaci<20>() == 6765);
}