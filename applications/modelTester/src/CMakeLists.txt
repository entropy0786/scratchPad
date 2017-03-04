project(modelTester CXX)

file(GLOB_RECURSE MODELTESTER_APP_SRC_FILES  "*.hpp" "*.cpp" "*.h")

FIND_PACKAGE(Boost
    COMPONENTS chrono thread REQUIRED)
set(Boost_USE_STATIC_LIBS ON)

#create executable
add_executable(modelTester ${MODELTESTER_APP_SRC_FILES})
# flags
set_target_properties(modelTester PROPERTIES LINKER_LANGUAGE CXX )
set_target_properties(modelTester PROPERTIES OUTPUT_NAME modelTester )
# dependencies
target_include_directories(modelTester PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR} libModel-static)
target_link_libraries(modelTester PUBLIC ${Boost_LIBRARIES} libModel-static)

INSTALL(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	DESTINATION "include"
	FILES_MATCHING
	PATTERN "*.hpp"
	PATTERN "*.h"
	PATTERN "*.tcc"
)

install(TARGETS modelTester DESTINATION apps/ OPTIONAL)