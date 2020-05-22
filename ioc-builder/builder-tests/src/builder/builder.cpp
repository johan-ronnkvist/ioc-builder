#include "builder-lib/ioc-builder.h"
#include "gtest/gtest.h"

using base_t = ioc::object;
using test_builder = ioc::builder;

struct logging_module : public base_t {};
struct network_module : public base_t {};
struct tracking_module : public base_t {
	tracking_module(network_module& network)
		: m_network(network) {}
	network_module& m_network;
};

struct store_module : public base_t {
	store_module(network_module& network, tracking_module& tracking)
		: m_network(network)
		, m_tracking(tracking) {}
	network_module& m_network;
	tracking_module& m_tracking;
};

TEST(builder, insert) {
	test_builder builder;
	EXPECT_FALSE(builder.is_registered<logging_module>());
	builder.register_type<logging_module>();
	EXPECT_FALSE(builder.is_constructed<logging_module>());
}

TEST(builder, construct) {
	test_builder builder;
	builder.register_type<logging_module>();
	EXPECT_FALSE(builder.is_constructed<logging_module>());
	builder.construct<logging_module>();
	EXPECT_TRUE(builder.is_registered<logging_module>());
}

struct numeric_module : public base_t {
	explicit numeric_module(int number)
		: m_number(number) {}
	int m_number;
};

struct fixed_ten_module : public numeric_module {
	fixed_ten_module()
		: numeric_module(10) {}
};

TEST(builder, module_alias) {
	test_builder builder;
	builder.register_type<fixed_ten_module, numeric_module>();
	auto& sample = builder.get<numeric_module>();
	EXPECT_EQ(sample.m_number, 10);
}

TEST(builder, get_module) {
	test_builder builder;
	builder.register_type<logging_module>();
	builder.construct<logging_module>();
	const auto& sample_module = builder.get<logging_module>();
	static_assert(std::is_same_v<std::decay_t<decltype(sample_module)>, logging_module>);
}

TEST(builder, visit) {
	test_builder builder;
	builder.register_type<logging_module>();
	builder.register_type<network_module>();
	builder.visit([](base_t&) {});
}

TEST(builder, construct_unregistered_type) {
	test_builder builder;
	try {
		builder.construct<logging_module>();
	} catch (const std::domain_error& error) {
		EXPECT_EQ(error.what(), std::string("unknown type, unable to construct"));
	}
}

TEST(builder, destroy_single_type) {
	test_builder builder;
	builder.register_type<logging_module>();
	builder.register_type<network_module>();
	builder.construct<network_module>();
	EXPECT_FALSE(builder.is_constructed<logging_module>());
	EXPECT_TRUE(builder.is_constructed<network_module>());
	builder.destroy<network_module>();
	EXPECT_FALSE(builder.is_constructed<network_module>());
}

TEST(builder, unload_unregistered) {
	test_builder builder;
	try {
		builder.destroy<logging_module>();
	} catch (const std::domain_error& error) {
		EXPECT_EQ(error.what(), std::string("unknown type, unable to unload"));
	}
}

TEST(builder, construct_with_dependencies) {
	test_builder builder;
	builder.register_type<network_module>();
	builder.register_type<tracking_module>(test_builder::dependency<network_module>{});
	builder.register_type<store_module>(test_builder::dependency<network_module>{}, test_builder::dependency<tracking_module>{});

	builder.construct<store_module>();
}