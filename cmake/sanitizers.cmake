add_library(sanitizers INTERFACE)

option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
option(ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" FALSE)
option(ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" FALSE)
option(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable undefined behavior sanitizer" FALSE)
option(ENABLE_SANITIZER_THREAD "Enable thread sanitizer" FALSE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR 
   CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(ENABLE_COVERAGE)
        target_compile_options(sanitizers INTERFACE --coverage -O0 -g)
        target_link_libraries(sanitizers INTERFACE --coverage)
    endif()

    set(SANITIZERS "")
    if(ENABLE_SANITIZER_ADDRESS)
        list(APPEND SANITIZERS "address")
    endif()

    if(ENABLE_SANITIZER_MEMORY)
      list(APPEND SANITIZERS "memory")
    endif()

    if(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR)
        list(APPEND SANITIZERS "undefined")
    endif()

    if(ENABLE_SANITIZER_THREAD)
      list(APPEND SANITIZERS "thread")
    endif()

    list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)
    if(NOT "${LIST_OF_SANITIZERS}" STREQUAL "")
        target_compile_options(sanitizers INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
        target_link_libraries(sanitizers INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
    endif()
endif()
