#pragma once


namespace Labyrinth::Map
{
    /*
        Tile type
    */
    enum class ETileType
    {
        Empty,      // no tile at all
        Wall,       // a inaccessable wall tile
        Ground      // a accessable ground tile
    };

    /*
        A tile on a map
    */
    struct Tile
    {
        // Type
        ETileType               m_type = ETileType::Empty;
    };
}
