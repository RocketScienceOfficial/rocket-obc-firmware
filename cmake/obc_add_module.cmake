macro(obc_add_module NAME)
    set(SOURCES "")

    foreach(X ${ARGN})
        set(SOURCES "${SOURCES};${CMAKE_CURRENT_LIST_DIR}/src/${X}")
    endforeach()

    add_library(module_${NAME} INTERFACE)

    target_sources(module_${NAME} INTERFACE
        ${SOURCES}
    )

    target_include_directories(module_${NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
endmacro()