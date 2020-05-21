#pragma once
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>

namespace ioc {
template <class object_base_t>
struct builder {
	template <class object_t>
	struct dependency {};

	template <class object_t, class alias_t = object_t, class... arg_t>
	void register_type(arg_t&&... args) {
		auto create_func = [this, args...]() {
			return entry {
				std::make_unique<object_t>(resolve(args)...)
			};
		};
		m_registry.insert({typeid(alias_t), create_func});
	}

	template <class object_t>
	[[nodiscard]] bool is_registered() const {
		return is_registered(typeid(object_t));
	}

	template <class object_t>
	[[nodiscard]] bool is_constructed() const {
		return is_constructed(typeid(object_t));
	}

	template <class object_t>
	object_t& get() {
		if (!is_constructed<object_t>()) {
			construct<object_t>();
		}
		return static_cast<object_t&>(*m_entries.at(typeid(object_t)).m_module);
	}

	template <class object_t>
	const object_t& get() const {
		if (!is_constructed<object_t>()) {
			construct<object_t>();
		}
		return static_cast<object_t&>(*m_entries.at(typeid(object_t)).m_module);
	}

	template <class object_t>
	void construct() {
		construct(typeid(object_t));
	}

	template <class object_t>
	void destroy() {
		destroy(typeid(object_t));
	}

	template <class functor_t>
	void visit(functor_t&& func) {
		std::for_each(m_entries.begin(), m_entries.end(), [&func](auto& entry) { func(*entry.second.m_module); });
	}

private:
	template <class object_t>
	const object_t& resolve(const object_t& object) const {
		return object;
	}

	template <class object_t>
	object_t& resolve(object_t& object) {
		return object;
	}


	template <class object_t>
	object_t& resolve(const dependency<object_t>&) {
		return get<object_t>();
	}

	template <class object_t>
	object_t resolve(object_t&& object) {
		return std::forward<object_t>(object);
	}

	template <class object_t>
	object_t& resolve(dependency<object_t>&&) {
		return get<object_t>();
	}

	[[nodiscard]] bool is_constructed(std::type_index id) const {
		return m_entries.find(id) != m_entries.end();
	}

	[[nodiscard]] bool is_registered(std::type_index id) const {
		return m_registry.find(id) != m_registry.end();
	}

	void construct(std::type_index id) {
		if (!is_registered(id)) {
			throw std::domain_error("unknown type, unable to construct");
		}

		if (!is_constructed(id)) {
			const auto& module_creator = m_registry.at(id);
			m_entries.insert(std::make_pair(id, module_creator()));
		}
	}

	void destroy(std::type_index id) {
		if (!is_registered(id)) {
			throw std::domain_error("unknown type, unable to unload");
		}

		if (is_constructed(id) && is_registered(id)) {
			m_entries.erase(id);
		}
	}

	struct entry {
		std::unique_ptr<object_base_t> m_module;
	};

	std::map<std::type_index, std::function<entry()>> m_registry;
	std::map<std::type_index, const entry> m_entries;
};
}  // namespace ioc