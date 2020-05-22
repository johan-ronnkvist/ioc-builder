#pragma once
#include "base.h"

namespace sample::network {
namespace result {
	struct success {};
	struct failure {};
}

using response = std::variant<result::success, result::failure>;

struct module : public ioc::object {
	virtual ~module() = default;
	virtual response post(std::string url, std::string data, uint16_t port = 80) = 0;
};

namespace mock {
struct failing_network : public network::module {
	response post(std::string /*url*/, std::string /*data*/, uint16_t /*port = 80*/) override {
		return result::failure{};
	}
};

struct success_network : public network::module {
	response post(std::string /*url*/, std::string /*data*/, uint16_t /*port = 80*/) override {
		return result::success{};
	}
};

struct random_network : public network::module {
	response post(std::string /*url*/, std::string /*data*/, uint16_t /*port = 80*/) override {
		const bool success = std::rand() % 1000 == 0;
		if (success) {
			return result::success{};
		} else {
			return result::failure{};
		}
	}
};

}  // namespace mock
}  // namespace sample::network