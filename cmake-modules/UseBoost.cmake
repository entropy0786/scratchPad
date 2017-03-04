set(boost_INCLUDE_DIR C:/_DEV/3rdParties/boost_1_61_0)
set(Boost_LIBRARY_DIR C:/_DEV/3rdParties/boost_1_61_0/stage/lib/win64)

find_package(Boost 1.61.0 
    COMPONENTS regex date_time chrono thread serialization graph system filesystem atomic log program_options unit_test timer QUIET)

set(Boost_USE_STATIC_LIBS ON)    
    
if (Boost_FOUND) 
  set(Base_FOUND 1)
else(Boost_FOUND)
    message("please set BOOST_ROOT environment variable to a proper boost install")
endif(Boost_FOUND)

