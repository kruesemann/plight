#pragma once
#include "labyrinth/include/component/tile_map.h"


namespace Labyrinth::Map
{
    /*
        Get the tile in given row and column on the map
    */
    inline
    Tile
    getTile(Component::TileMap const& rTileMap,
            int row,
            int column)
    {
        return rTileMap.m_tiles.at(static_cast<size_t>(row) * rTileMap.m_width + column);
    }
}
