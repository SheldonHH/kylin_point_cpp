add_executable(klp.token.wasm ${CMAKE_CURRENT_SOURCE_DIR}/src/klp.token.cpp)
target_include_directories(klp.token.wasm
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(klp.token.wasm
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/abi/klp.token.abi" "${CMAKE__CURRENT_BINARY_DIR}" COPYONLY)
