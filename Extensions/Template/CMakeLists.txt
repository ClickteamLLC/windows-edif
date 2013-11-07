
cmake_minimum_required(VERSION 2.8.11)

project(EXTENSION)

set(EXT_NAME Template)

#Grab source & header files
file(GLOB EXTENSION_SOURCES "./*.c*")
file(GLOB EXTENSION_HEADERS "./*.h*")
file(GLOB SDK_SOURCES "../../Lib/*.c*")
file(GLOB SDK_HEADERS "../../Inc/*.h*")
set(EXTENSION_SOURCES ${EXTENSION_SOURCES} ${SDK_SOURCES})
set(EXTENSION_HEADERS ${EXTENSION_HEADERS} ${SDK_HEADERS})
include_directories("../../Inc/" "./")
link_directories("./" "../../Lib/")

#Fix VC++ options
if(MSVC)
    message(STATUS "Note: Building with MSVC (recommended)")
    foreach(flag
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
        endif()
    endforeach()
endif()

list(APPEND EXT_MFX_TYPE "Plain")
list(APPEND EXT_MFX_TYPE "HWA")
list(APPEND EXT_MFX_TYPE "Unicode")
list(APPEND EXT_BUILD_TYPE "Debug")
list(APPEND EXT_BUILD_TYPE "Release")
list(APPEND EXT_BUILD_TYPE "RunOnly")

foreach(curr_mfx_t ${EXT_MFX_TYPE})
    foreach(curr_build_t ${EXT_BUILD_TYPE})
        set(curr_name ${curr_mfx_t}${curr_build_t})

        add_library(${curr_name} SHARED ${EXTENSION_SOURCES})

        set(curr_defs "_WINDLL;COXSDK;IN_EXT_VERSION2;_CRT_SECURE_NO_WARNINGS;_CRT_SECURE_NO_DEPRECATE;_WINDOWS")

        if(curr_build_t STREQUAL "Release")
           set(out_dir "./MFX/Extensions/")
           list(APPEND curr_defs "EDITOR")

        elseif(curr_build_t STREQUAL "RunOnly")
           set(out_dir "./MFX/Data/Runtime/")
           list(APPEND curr_defs "RUN_ONLY")

        else()
            set(out_dir "./MFX_Debug/Extensions/")
            list(APPEND curr_defs "EDITOR;DEBUG;_DEBUG")
        endif()

        set(curr_libs "kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib")

        if(curr_mfx_t STREQUAL "HWA")
           set(out_dir "${out_dir}HWA/")
           set(curr_libs "${curr_libs} mmfs2hwa.lib")
           list(APPEND curr_defs "HWABETA")

        elseif(curr_mfx_t STREQUAL "Unicode")
           set(out_dir "${out_dir}Unicode/")
           set(curr_libs "${curr_libs} mmfs2u.lib")
           list(APPEND curr_defs "_UNICODE;UNICODE")

        else()
            set(curr_libs "${curr_libs} mmfs2.lib")
        endif()

        set(curr_link "")
        if(MSVC)
            set(curr_link "/DLL /MANIFEST:NO /DEF:\"${CMAKE_SOURCE_DIR}/Ext.def\" /MACHINE:X86")
        endif()

        target_link_libraries(${curr_name} ${curr_libs})
        SET_TARGET_PROPERTIES(${curr_mfx_t}${curr_build_t} PROPERTIES
            LIBRARY_OUTPUT_NAME "${EXT_NAME}.mfx"
            LIBRARY_OUTPUT_DIRECTORY "${out_dir}"
            COMPILE_DEFINITIONS "${curr_defs}"
            LINK_FLAGS "${curr_link}"
        )
    endforeach()
endforeach()
