set(target_name "sample-tests")

set(src_precompiled
	"src/precompiled.h")

set(src_numbers
	"src/numbers/numbers.cpp")

set(src_files 
	${src_precompiled}
	${src_numbers})

add_executable(${target_name} ${src_files} ${res_files})

source_group("src\\precompiled" FILES ${src_precompiled})
source_group("src\\numbers" FILES ${src_numbers})
source_group("res" FILES ${res_files})

target_link_libraries(${target_name} CONAN_PKG::gtest)
target_link_libraries(${target_name} "sample-lib")

target_include_directories(${target_name} PRIVATE "../sample-lib/include")

set_target_properties(${target_name} PROPERTIES FOLDER "sample")

target_precompile_headers(${target_name} PUBLIC ${src_precompiled})

include(GoogleTest)
gtest_discover_tests(${target_name})
