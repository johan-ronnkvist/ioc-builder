#pragma once
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>

namespace ioc {
template <class module_base_t>
struct module_manager {
	template <class object_t>
	struct dependency {};

	template <class module_t, class alias_t = module_t, class... arg_t>
	void register_module(arg_t&&... args) {
		m_registry[typeid(alias_t)] = [this, args...]() {
			return module_entry{std::make_unique<module_t>(resolve(args)...)};
		};
	}

	template <class module_t, class alias_t = module_t, class... arg_t>
	void insert_module(arg_t&&... args) {
		register_module<module_t, alias_t>(std::forward<arg_t>(args)...);
		load<alias_t>();
	}

	template <class module_t>
	[[nodiscard]] bool is_registered() const {
		return is_registered(typeid(module_t));
	}

	template <class module_t>
	[[nodiscard]] bool is_loaded() const {
		return is_loaded(typeid(module_t));
	}

	template <class module_t>
	module_t& get() {
		return static_cast<module_t&>(*m_modules.at(typeid(module_t)).m_module);
	}

	template <class module_t>
	const module_t& get() const {
		return static_cast<module_t&>(*m_modules.at(typeid(module_t)).m_module);
	}

	template <class module_t>
	void load() {
		load(typeid(module_t));
	}

	template <class module_t>
	void unload() {
		unload(typeid(module_t));
	}

	template <class functor_t>
	void visit(functor_t&& func) {
		std::for_each(m_modules.begin(), m_modules.end(), [&func](auto& entry) { func(*entry.second.m_module); });
	}

	template <class object_t>
	object_t& resolve(const object_t& object) {
		return object;
	}

	template <class object_t>
	object_t& resolve(const dependency<object_t>&) {
		return get<object_t>();
	}

	template <class object_t>
	object_t resolve(object_t&& object) {
		return object;
	}

	template <class object_t>
	object_t& resolve(dependency<object_t>&&) {
		return get<object_t>();
	}

private:
	[[nodiscard]] bool is_loaded(std::type_index id) const {
		return m_modules.find(id) != m_modules.end();
	}

	[[nodiscard]] bool is_registered(std::type_index id) const {
		return m_registry.find(id) != m_registry.end();
	}

	void load(std::type_index id) {
		if (!is_registered(id)) {
			throw std::domain_error("module not registered");
		}

		if (!is_loaded(id)) {
			const auto& module_creator = m_registry.at(id);
			m_modules.insert(std::make_pair(id, module_creator()));
		}
	}

	void unload(std::type_index id) {
		if (!is_registered(id)) {
			throw std::domain_error("module not registered");
		}

		if (is_loaded(id) && is_registered(id)) {
			m_modules.erase(id);
		}
	}

	struct module_entry {
		std::unique_ptr<module_base_t> m_module;
	};

	std::map<std::type_index, std::function<module_entry()>> m_registry;
	std::map<std::type_index, const module_entry> m_modules;
};
}  // namespace ioc