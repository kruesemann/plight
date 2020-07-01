#pragma once
#include <set>
#include <vector>


namespace Labyrinth
{
    namespace Component
    {
        struct Position;
        struct TileMap;
    }

    namespace Map
    {
        enum class ETileType;
    }
}

namespace Labyrinth::Map
{
    /*
        Path finding tools
    */

    std::vector<Component::Position> findShortestPath(Component::TileMap const&,
                                                      Component::Position const&,
                                                      Component::Position const&,
                                                      Component::Position const&,
                                                      std::set<Map::ETileType> const&);
}
