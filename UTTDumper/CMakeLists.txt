# CMakeList.txt : CMake project for UTTDumper, include source and define
# project specific logic here.
#

# Add source to this project's executable.
set(HEADER_DIR "include")
set(SOURCE_DIR "lib")
set(SOURCE_FILES
	${SOURCE_DIR}/info.cpp
	${SOURCE_DIR}/rtti.cpp
	${SOURCE_DIR}/init.cpp
	${SOURCE_DIR}/dumper.cpp
	${SOURCE_DIR}/engine.cpp
	${SOURCE_DIR}/version.cpp
	${SOURCE_DIR}/typetree.cpp
	${SOURCE_DIR}/common_string.cpp
	${SOURCE_DIR}/native_object.cpp
)

if (WIN32)
	set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_DIR}/windll.cpp)
else()
	set(SOURCE_FILES ${SOURCE_FILES} ${SOURCE_DIR}/libso.cpp)
endif()

add_library(UTTDumper SHARED ${SOURCE_FILES})
set_target_properties(UTTDumper PROPERTIES CXX_STANDARD 20)
target_include_directories(UTTDumper PRIVATE ${HEADER_DIR})
target_compile_definitions(UTTDumper PRIVATE TOML_EXCEPTIONS=1)

include(FetchContent)

FetchContent_Declare(
	tomlplusplus
	GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)

FetchContent_Declare(
	nlohmann_json
	GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)

FetchContent_MakeAvailable(tomlplusplus)
FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(UTTDumper PRIVATE tomlplusplus::tomlplusplus)
target_link_libraries(UTTDumper PRIVATE nlohmann_json::nlohmann_json)

if (ANDROID)
	find_library(loglib log)
	target_link_libraries(UTTDumper PRIVATE ${loglib})
	target_compile_definitions(UTTDumper PRIVATE ANDROID_TAG="UTTDumper")
endif()

add_custom_command(
	TARGET UTTDumper
	COMMAND ${CMAKE_COMMAND} -E copy 
	${CMAKE_CURRENT_SOURCE_DIR}/config.toml
	$<TARGET_FILE_DIR:UTTDumper>
)