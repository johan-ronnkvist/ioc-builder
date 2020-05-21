#pragma once

namespace sample {
struct module_base {
	module_base() = default;
	~module_base() = default;
	module_base(const module_base& other) = delete;
	module_base(module_base&& other) = delete;
	module_base& operator=(const module_base& other) = delete;
	module_base& operator=(module_base&& other) = delete;
};

using builder = ioc::builder<sample::module_base>;
}