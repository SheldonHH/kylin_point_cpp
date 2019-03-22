add_executable(klp.connector.wasm ${CMAKE_CURRENT_SOURCE_DIR}/src/klp.connector.cpp)
target_include_directories(klp.connector.wasm
   PUBLIC 
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(klp.connector.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/abi/klp.connector.abi" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${WASM_ROOT}/eosio.wasmsdk/include)
