#define CATCH_CONFIG_MAIN

#include "../include/n_container.hpp"
#include "catch.hpp"

NE::NContainer<int> container_1 ({1, 7, 3, 6, 10});
NE::NContainer<int> sorted_container_1 ({1, 3, 6, 7, 10});
NE::NContainer<int> sorted_container_2 ({1, 3, 6, 7, 10, 117});

TEST_CASE( "NContainer test", "[ncontainer]" ) {
    REQUIRE( container_1.InsertionSort() == sorted_container_1 );
    REQUIRE( container_1.Append(117,2).GetLength() == 6 );
    REQUIRE( container_1.BubbleSort() == sorted_container_2 );
    REQUIRE( container_1.Contains(117) == 5 );
    REQUIRE( container_1.Remove(sorted_container_1).Replace(117, 6).InsertionSort() != sorted_container_1 );
}