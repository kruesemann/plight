#pragma once
#include "labyrinth/include/map/tile.h"

#include <vector>


namespace Labyrinth::Component
{
    /*
        Map made of tiles
    */
    struct TileMap
    {
        // The tiles
        std::vector<Map::Tile>  m_tiles;

        // Number of columns
        int                     m_width = 0;

        // Number of rows
        int                     m_height = 0;

        // Width of one tile
        int                     m_tileWidth = 1;

        // Height of one tile
        int                     m_tileHeight = 1;
    };
}
