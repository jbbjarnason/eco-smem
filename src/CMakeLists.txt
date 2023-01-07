
add_library(eco_smem)
add_library(eco::smem ALIAS eco_smem)

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


