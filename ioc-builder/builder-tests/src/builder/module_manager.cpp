#include "builder-lib/ioc-builder.h"
#include "gtest/gtest.h"

struct module_base {
	module_base() = default;
	module_base(const module_base& other) = delete;
	module_base(module_base&& other) = delete;
	module_base& operator=(const module_base& other) = delete;
	module_base& operator=(module_base&& other) = delete;
};
struct test_module_1 : public module_base {};
struct test_module_2 : public module_base {};
struct test_module_3 : public module_base {};

using namespace ioc;
using test_manager = module_manager<module_base>;

struct network_module : public module_base {};
struct tracking_module : public module_base {
	tracking_module(network_module& network)
		: m_network(network) {}
	network_module& m_network;
};

struct store_module : public module_base {
	store_module(network_module& network, tracking_module& tracking)
		: m_network(network)
		, m_tracking(tracking) {}
	network_module& m_network;
	tracking_module& m_tracking;
};

TEST(module_manager, register_module) {
	test_manager manager;
	EXPECT_FALSE(manager.is_loaded<test_module_1>());
	EXPECT_FALSE(manager.is_registered<test_module_1>());
	manager.register_module<test_module_1>();
	EXPECT_FALSE(manager.is_loaded<test_module_1>());
	EXPECT_TRUE(manager.is_registered<test_module_1>());
}

TEST(module_manager, insert_module) {
	test_manager manager;
	EXPECT_FALSE(manager.is_loaded<test_module_1>());
	EXPECT_FALSE(manager.is_registered<test_module_1>());
	manager.insert_module<test_module_1>();
	EXPECT_TRUE(manager.is_loaded<test_module_1>());
	EXPECT_TRUE(manager.is_registered<test_module_1>());
}

struct numeric_module : public module_base {
	explicit numeric_module(int number)
		: m_number(number) {}
	int m_number;
};

struct fixed_ten_module : public numeric_module {
	fixed_ten_module()
		: numeric_module(10) {}
};

TEST(module_manager, module_alias) {
	test_manager manager;
	manager.insert_module<fixed_ten_module, numeric_module>();
	auto& sample = manager.get<numeric_module>();
	EXPECT_EQ(sample.m_number, 10);
}

TEST(module_manager, get_module) {
	test_manager manager;
	manager.register_module<test_module_1>();
	manager.load<test_module_1>();
	const auto& sample_module = manager.get<test_module_1>();
	static_assert(std::is_same_v<std::decay_t<decltype(sample_module)>, test_module_1>);
}

TEST(module_manager, visit) {
	test_manager manager;
	manager.register_module<test_module_1>();
	manager.register_module<test_module_2>();
	manager.visit([](module_base&) {});
}

TEST(module_manager, load_single) {
	test_manager manager;
	manager.register_module<test_module_1>();
	manager.register_module<test_module_2>();
	manager.register_module<test_module_3>();
	manager.load<test_module_1>();
	EXPECT_TRUE(manager.is_loaded<test_module_1>());
	EXPECT_FALSE(manager.is_loaded<test_module_2>());
	EXPECT_FALSE(manager.is_loaded<test_module_3>());
}

TEST(module_manager, load_unregistered) {
	test_manager manager;
	try {
		manager.load<test_module_1>();
	} catch (const std::domain_error& error) {
		EXPECT_EQ(error.what(), std::string("module not registered"));
	}
}

TEST(module_manager, unload_single) {
	test_manager manager;
	manager.register_module<test_module_1>();
	manager.register_module<test_module_2>();
	manager.register_module<test_module_3>();
	manager.load<test_module_3>();
	EXPECT_FALSE(manager.is_loaded<test_module_1>());
	EXPECT_FALSE(manager.is_loaded<test_module_2>());
	EXPECT_TRUE(manager.is_loaded<test_module_3>());
	manager.unload<test_module_3>();
}

TEST(module_manager, unload_unregistered) {
	test_manager manager;
	try {
		manager.unload<test_module_1>();
	} catch (const std::domain_error& error) {
		EXPECT_EQ(error.what(), std::string("module not registered"));
	}
}

TEST(module_manager, load_dependency) {
	test_manager manager;
	manager.insert_module<network_module>();
	manager.insert_module<tracking_module>(test_manager::dependency<network_module>{});
	manager.insert_module<store_module>(test_manager::dependency<network_module>{}, test_manager::dependency<tracking_module>{});
}