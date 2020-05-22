#pragma once
#include "base.h"

namespace sample::tracking {
struct message {
	std::string m_data;
};

struct module : public ioc::object {
	virtual ~module() = default;
	virtual void track_data(const message& msg) = 0;
};

namespace v1 {
struct module : public tracking::module {
	module(sample::network::module& network, std::string tracking_server_url)
		: m_network(network) 
		, m_server_url(tracking_server_url)	{}

	void track_data(const message& msg) override {
		const auto response = m_network.post(m_server_url, msg.m_data);
		if (std::holds_alternative<network::result::failure>(response)) {
			throw std::domain_error("failed to track data");
		}
	}

private:
	sample::network::module& m_network;
	std::string m_server_url;
};
}  // namespace mock
}  // namespace sample::tracking