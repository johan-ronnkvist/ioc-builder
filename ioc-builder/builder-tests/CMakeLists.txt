set(target_name "builder-tests")

set(src_precompiled
	"src/precompiled.h")

set(src_builder
	"src/builder/module_manager.cpp")

set(src_files 
	${src_precompiled}
	${src_builder})

add_executable(${target_name} ${src_files} ${res_files})

source_group("src\\precompiled" FILES ${src_precompiled})
source_group("src\\builder" FILES ${src_builder})
source_group("res" FILES ${res_files})

target_link_libraries(${target_name} CONAN_PKG::gtest)
target_link_libraries(${target_name} "builder-lib")

target_include_directories(${target_name} PRIVATE "../builder-lib/include")

set_target_properties(${target_name} PROPERTIES FOLDER "ioc-builder")

target_precompile_headers(${target_name} PUBLIC ${src_precompiled})

include(GoogleTest)
gtest_discover_tests(${target_name})
