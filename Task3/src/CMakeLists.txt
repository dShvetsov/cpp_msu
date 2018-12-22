
SET(SOURCES
    MarketPlace.h
    Shop.cpp
    Shop.h
    Product.cpp
    Product.h)

add_library(shop ${SOURCES})

add_executable(market problem2.cpp)

target_link_libraries(market shop pthread)
