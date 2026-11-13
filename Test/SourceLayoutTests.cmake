find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_test(NAME source_layout_test
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_LIST_DIR}/source_layout_test.py" "${CMAKE_C_COMPILER}")
add_executable(optional_protocol_canonical_test optional_protocol_layout_test.c)
target_link_libraries(optional_protocol_canonical_test PRIVATE STM32Tools::CoreMQTT STM32Tools::Nanopb)
add_test(NAME optional_protocol_canonical_test COMMAND optional_protocol_canonical_test)
add_executable(optional_protocol_legacy_test optional_protocol_layout_test.c
    ../Src/MQTT/core_mqtt.c ../Src/MQTT/core_mqtt_serializer.c ../Src/MQTT/core_mqtt_state.c
    ../Src/pb/pb_common.c ../Src/pb/pb_encode.c ../Src/pb/pb_decode.c)
target_include_directories(optional_protocol_legacy_test PRIVATE ../Inc ../Inc/MQTT ../Inc/pb)
add_test(NAME optional_protocol_legacy_test COMMAND optional_protocol_legacy_test)
foreach(target optional_protocol_canonical_test optional_protocol_legacy_test)
    target_compile_options(${target} PRIVATE
        $<$<C_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Werror;-UNDEBUG>)
endforeach()
