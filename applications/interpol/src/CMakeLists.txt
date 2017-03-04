project(interpol CXX)

file(GLOB_RECURSE INTERPOL_APP_SRC_FILES  "*.hpp" "*.cpp" "*.h")

FIND_PACKAGE(Boost
    COMPONENTS chrono thread REQUIRED)
set(Boost_USE_STATIC_LIBS ON)

#create executable
add_executable(interpol ${INTERPOL_APP_SRC_FILES})
# flags
set_target_properties(interpol PROPERTIES LINKER_LANGUAGE CXX )
set_target_properties(interpol PROPERTIES OUTPUT_NAME interpol )
# dependencies
target_include_directories(interpol PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR} libImage-static libModel-static)
target_link_libraries(interpol PUBLIC ${Boost_LIBRARIES} libImage-static libModel-static)

INSTALL(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	DESTINATION "include"
	FILES_MATCHING
	PATTERN "*.hpp"
	PATTERN "*.h"
	PATTERN "*.tcc"
)

install(TARGETS interpol DESTINATION apps/ OPTIONAL)