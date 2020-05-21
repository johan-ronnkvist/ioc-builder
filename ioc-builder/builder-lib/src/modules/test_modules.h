/*
#pragma once
#include <stdint.h>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <typeindex>

namespace ioc {
template <class base_t>
struct builder {
	template <class object_t, class alias_t = object_t, class... arg_t>
	void register_object(arg_t&&... args) {
		m_registry[std::type_index(typeid(alias_t))] = [args...]() {
			return entry{std::make_unique<object_t>(std::forward<arg_t>(args)...)};
		};
	}

	template <class object_t, class alias_t = object_t, class... arg_t>
	void build_object(arg_t&&... args) {
		register_object<object_t, alias_t>(std::forward<arg_t>(args)...);
		build<alias_t>();
	}

	template <class object_t>
	[[nodiscard]] bool is_registered() const {
		return is_registered(std::type_index(typeid(object_t)));
	}

	template <class object_t>
	[[nodiscard]] bool is_loaded() const {
		return is_loaded(std::type_index(typeid(object_t)));
	}

	template <class object_t>
	[[nodiscard]] object_t& get() {
		return static_cast<object_t&>(*m_entries.at(std::type_index(typeid(object_t))).m_entry);
	}

	template <class object_t>
	[[nodiscard]] const object_t& get() const {
		return static_cast<object_t&>(*m_entries.at(std::type_index(typeid(object_t))).m_entry);
	}

	template <class object_t>
	object_t& build() {
		build(std::type_index(typeid(object_t)));
		return get<object_t>();
	}

	template <class object_t>
	void destroy() {
		destroy(std::type_index(typeid(object_t)));
	}

	template <class functor_t>
	void visit(functor_t&& func) {
		std::for_each(m_entries.begin(), m_entries.end(), [&func](auto& entry) { func(*entry.second.m_module); });
	}

private:
	[[nodiscard]] bool is_loaded(std::type_index id) const {
		return m_entries.find(id) != m_entries.end();
	}

	[[nodiscard]] bool is_registered(std::type_index id) const {
		return m_registry.find(id) != m_registry.end();
	}

	void build(std::type_index id) {
		if (!is_registered(id)) {
			throw std::domain_error("object not registered");
		}

		if (!is_loaded(id)) {
			const auto& module_creator = m_registry.at(id);
			m_entries.insert(std::make_pair(id, module_creator()));
		}
	}

	void destroy(std::type_index id) {
		if (!is_registered(id)) {
			throw std::domain_error("object not registered");
		}

		if (is_loaded(id) && is_registered(id)) {
			m_entries.erase(id);
		}
	}

	struct entry {
		std::unique_ptr<base_t> m_entry;
	};

	std::map<std::type_index, std::function<entry()>> m_registry;
	std::map<std::type_index, const entry> m_entries;
};
}  // namespace ioc

*/