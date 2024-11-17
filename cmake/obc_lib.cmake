macro(obc_add_lib NAME)
    set(SOURCES "")

    foreach(X ${ARGN})
        set(SOURCES "${SOURCES};${CMAKE_CURRENT_SOURCE_DIR}/src/${X}")
    endforeach()

    set(LIB_NAME lib_${NAME})

    add_library(${LIB_NAME} INTERFACE)

    target_sources(${LIB_NAME} INTERFACE
        ${SOURCES}
    )

    target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endmacro()