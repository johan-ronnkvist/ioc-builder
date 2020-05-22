#include "modules/network.h"
#include "modules/store.h"
#include "modules/tracking.h"

using namespace sample;

int main(int /*argc*/, char** /*argv*/) {
	builder module_builder;
	module_builder.register_type<network::mock::success_network, network::module>();
	module_builder.register_type<tracking::v1::module, tracking::module>(builder::dependency<network::module>{},
																		 "mock.tracking.url");
	module_builder.register_type<store::v1::module, store::module>(
		builder::dependency<network::module>{},
		builder::dependency<tracking::module>{},
		"mock.store.url",
		[](const store::product_id&) { /* add product to invenctory */ });

	auto& store = module_builder.get<store::module>();
	store.attempt_purchase({"sample.product"});
	return 0;
}