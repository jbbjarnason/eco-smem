
add_library(eco_smem)
add_library(eco::smem ALIAS eco_smem)

find_package(Boost COMPONENTS system REQUIRED)
find_package(fmt CONFIG REQUIRED)
target_link_libraries(eco_smem PUBLIC fmt::fmt Boost::boost Boost::system)


target_include_directories(eco_smem
  PRIVATE
    include
  PUBLIC
    include/public
)

target_sources(eco_smem
  PUBLIC
    smem.cpp
)


