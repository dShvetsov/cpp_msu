
SET(SOURCES
    MarketPlace.h
    Shop.cpp
    Shop.h
    Product.cpp
    Product.h)

add_library(shop ${SOURCES})

add_executable(problem1 problem1.cpp)
add_executable(market problem2.cpp)

target_link_libraries(market shop pthread)
target_link_libraries(problem1 pthread)
