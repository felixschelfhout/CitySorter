option(ENABLE_CLANG_TIDY "Enable clang-tidy during builds" ON)
option(ENABLE_CPPCHECK "Enable cppcheck during builds" ON)
option(ENABLE_SANITIZERS "Enable Address/Undefined sanitizers" ON)
option(ENABLE_FORMAT_TARGET "Enable clang-format target" ON)
option(ENABLE_CODESPELL_TARGET "Enable codespell target" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy-18 clang-tidy)
    if(CLANG_TIDY_EXE)
        set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY_EXE})
        
        # Add target to auto-fix clang-tidy warnings
        file(GLOB_RECURSE TIDY_SOURCES CONFIGURE_DEPENDS
            ${PROJECT_SOURCE_DIR}/source/*.c
            ${PROJECT_SOURCE_DIR}/source/*.h
            ${PROJECT_SOURCE_DIR}/include/*.h
            ${PROJECT_SOURCE_DIR}/tests/*.c
            ${PROJECT_SOURCE_DIR}/tests/*.h
        )
        add_custom_target(tidy-fix
            COMMAND ${CLANG_TIDY_EXE} 
                -p ${PROJECT_BINARY_DIR}
                --fix 
                --fix-errors
                ${TIDY_SOURCES}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Auto-fixing clang-tidy warnings"
        )
    else()
        message(WARNING "clang-tidy not found; disabling clang-tidy integration")
    endif()
endif()

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK_EXE NAMES cppcheck)
    if(CPPCHECK_EXE)
        set(CMAKE_C_CPPCHECK
            ${CPPCHECK_EXE}
            --enable=warning,style,performance,portability
            --inline-suppr
            --std=c99
            --suppress=missingIncludeSystem
        )
    else()
        message(WARNING "cppcheck not found; disabling cppcheck integration")
    endif()
endif()

function(enable_sanitizers target_name)
    if(ENABLE_SANITIZERS)
        target_compile_options(${target_name} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${target_name} PRIVATE -fsanitize=address,undefined)
    endif()
endfunction()

if(ENABLE_FORMAT_TARGET)
    find_program(CLANG_FORMAT_EXE NAMES clang-format-18 clang-format)
    if(CLANG_FORMAT_EXE)
        file(GLOB_RECURSE FORMAT_SOURCES CONFIGURE_DEPENDS
            ${PROJECT_SOURCE_DIR}/source/*.c
            ${PROJECT_SOURCE_DIR}/source/*.h
            ${PROJECT_SOURCE_DIR}/include/*.h
            ${PROJECT_SOURCE_DIR}/tests/*.c
            ${PROJECT_SOURCE_DIR}/tests/*.h
        )
        add_custom_target(format
            COMMAND ${CLANG_FORMAT_EXE} -i ${FORMAT_SOURCES}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Formatting source files with clang-format"
        )
    else()
        message(WARNING "clang-format not found; format target will not be available")
    endif()
endif()

if(ENABLE_CODESPELL_TARGET)
    find_program(CODESPELL_EXE NAMES codespell)
    if(CODESPELL_EXE)
        add_custom_target(codespell
            COMMAND ${CODESPELL_EXE} -q 3 --config ${PROJECT_SOURCE_DIR}/.codespellrc ${PROJECT_SOURCE_DIR}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Running codespell"
        )
    else()
        message(WARNING "codespell not found; codespell target will not be available")
    endif()
endif()
