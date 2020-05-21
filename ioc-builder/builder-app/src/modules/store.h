#pragma once
#include "base.h"
#include "tracking.h"

namespace sample::store {
struct product_id {
	std::string m_identifier;
};

struct module : public module_base {
	virtual ~module() = default;
	virtual void attempt_purchase(product_id product) = 0;
};

namespace v1 {
using on_purcase_complete = std::function<void(const product_id&)>;

struct module : public store::module {
	module(network::module& network, tracking::module& tracking, std::string server_url, on_purcase_complete callback)
		: m_network(network)
		, m_tracking(tracking)
		, m_purchase_complete_callback(callback) 
		, m_server_url(server_url) {}

	virtual void attempt_purchase(product_id product) override {
		const auto response = m_network.post(m_server_url, "purchase_" + product.m_identifier);
		if (std::holds_alternative<network::result::failure>(response)) {
			m_tracking.track_data({"purchase failed (" + product.m_identifier + ") " + "extra tracking data"});
		} else {
			m_tracking.track_data({"purchase success (" + product.m_identifier + ")"});
			m_purchase_complete_callback(product);
		}
	}

private:
	network::module& m_network;
	tracking::module& m_tracking;
	on_purcase_complete m_purchase_complete_callback;
	std::string m_server_url;
};
}  // namespace v1

}  // namespace sample::store