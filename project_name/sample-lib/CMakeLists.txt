set(target_name "sample-lib")

set(src_precompiled
    "src/precompiled.h")

set(src_include
	"include/sample-lib/library.h")

set(src_dummy
	"src/dummy.cpp")

set(src_library
	"src/library/numbers.h")

set(src_files
    ${src_precompiled}
	${src_dummy}
	${src_library}
	${src_include})

add_library(${target_name} STATIC ${src_files})

source_group("src\\precompiled" FILES ${src_precompiled})
source_group("src\\include" FILES ${src_include})
source_group("src\\library" FILES ${src_library})
source_group("src\\dummy" FILES ${src_dummy})

target_link_libraries(${target_name} PUBLIC "compiler_warnings")
target_link_libraries(${target_name} PUBLIC "compiler_features")
target_link_libraries(${target_name} PUBLIC "sanitizers")

set_target_properties(${target_name} PROPERTIES FOLDER "sample")
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)

target_precompile_headers(${target_name} PUBLIC ${src_precompiled})