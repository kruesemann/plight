#include "labyrinth/include/map/path_finding.h"

#include "labyrinth/include/map/tools.h"

#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/tile_map.h"

#include <algorithm>
#include <cmath>


namespace Labyrinth::Map
{
    /*
        Helper tile for path finding
    */
    struct SearchTile
    {
                                SearchTile(Map::ETileType tileType,
                                           int row,
                                           int column,
                                           int heuristic = 0)
                                    : m_tileType(tileType)
                                    , m_row(row)
                                    , m_column(column)
                                    , m_heuristic(heuristic) {}

        // Tile type
        Map::ETileType const    m_tileType;

        // Row on the map
        int const               m_row;

        // Column on the map
        int const               m_column;

        // Heuristic for A*
        int                     m_heuristic;

        // Weight for A*
        int                     m_weight = 0;

        // True if the tile has been visited during path finding
        bool                    m_visited = false;

        // True if the shortest path to the tile has been found
        bool                    m_closed = false;

        // Predecessor in the shortest path to the tile
        SearchTile*             m_pPredecessor = nullptr;
    };

    /*
        Determines if the tiles are the same (position-wise)
    */
    bool
    haveSamePosition(SearchTile const& rLhs, SearchTile const& rRhs)
    {
        return rLhs.m_row == rRhs.m_row
            && rLhs.m_column == rRhs.m_column;
    }

    /*
        Computes the manhattan distance between the positions
    */
    int
    manhattanDistance(int lhsRow,
                      int lhsColumn,
                      int rhsRow,
                      int rhsColumn)
    {
        return std::abs(lhsRow - rhsRow) + std::abs(lhsColumn - rhsColumn);
    }

    /*
        Finds the shortest path using only allowed tiles between two positions on given map
    */
    std::vector<Component::Position>
    findShortestPath(Component::TileMap const& rTileMap,
                     Component::Position const& rMapOffset,
                     Component::Position const& rPosition,
                     Component::Position const& rTargetPosition,
                     std::set<Map::ETileType> const& rAllowedTileTypes)
    {
        std::vector<Component::Position> result;

        SearchTile const targetTile(ETileType::Empty,
                                    (rTargetPosition.m_value[1] - rMapOffset.m_value[1]) / rTileMap.m_tileHeight,
                                    (rTargetPosition.m_value[0] - rMapOffset.m_value[0]) / rTileMap.m_tileWidth);

        std::vector<SearchTile> searchMap;

        for (int i = 0; i < rTileMap.m_height; ++i)
            for (int j = 0; j < rTileMap.m_width; ++j)
                searchMap.emplace_back(getTile(rTileMap, i, j).m_type,
                                       i,
                                       j,
                                       manhattanDistance(i, j, targetTile.m_row, targetTile.m_column));

        static int const directions[8][2] = {
            {-1, -1},
            {-1,  0},
            {-1,  1},
            { 0, -1},
            { 0,  1},
            { 1, -1},
            { 1,  0},
            { 1,  1}
        };

        auto const getNeighbor = [&searchMap, &rTileMap](SearchTile const* pSearchTile, int di, int dj)
        {
            return &searchMap.at((static_cast<size_t>(pSearchTile->m_row) + di) * rTileMap.m_width + pSearchTile->m_column + dj);
        };

        auto const isAllowed = [&rAllowedTileTypes](ETileType tileType)
        {
            return rAllowedTileTypes.find(tileType) != rAllowedTileTypes.end();
        };

        auto const startColumn = (rPosition.m_value[0] - rMapOffset.m_value[0]) / rTileMap.m_tileHeight;
        auto const startRow = (rPosition.m_value[1] - rMapOffset.m_value[1]) / rTileMap.m_tileHeight;

        std::vector<SearchTile*> queue;
        queue.emplace_back(&searchMap.at(static_cast<size_t>(startRow) * rTileMap.m_width + startColumn));

        SearchTile* pCurrent = nullptr;

        while (!queue.empty())
        {
            pCurrent = queue.back();
            queue.pop_back();

            if (haveSamePosition(*pCurrent, targetTile))
                break;

            pCurrent->m_closed = true;

            for (size_t i = 0; i < 8; ++i)
            {
                auto* pNeighbor = getNeighbor(pCurrent, directions[i][0], directions[i][1]);

                if (pNeighbor->m_closed ||
                    !isAllowed(pNeighbor->m_tileType))
                    continue;

                auto const isDiagonalNeighbor = directions[i][0] != 0 && directions[i][1] != 0;

                if (isDiagonalNeighbor &&
                    !isAllowed(getNeighbor(pCurrent, directions[i][0], 0)->m_tileType) &&
                    !isAllowed(getNeighbor(pCurrent, 0, directions[i][1])->m_tileType))
                    continue;

                auto const weight = pCurrent->m_weight + (isDiagonalNeighbor ? 141 : 100);

                if (pNeighbor->m_visited &&
                    weight >= pNeighbor->m_weight)
                    continue;

                pNeighbor->m_pPredecessor = pCurrent;
                pNeighbor->m_weight = weight;

                if (!pNeighbor->m_visited)
                {
                    pNeighbor->m_visited = true;
                    queue.push_back(pNeighbor);
                }
                std::sort(queue.begin(),
                          queue.end(),
                          [](auto const* pLhs, auto const* pRhs)
                          {
                              return pLhs->m_heuristic + pLhs->m_weight > pRhs->m_heuristic + pRhs->m_weight;
                          });
            }
        }

        if (pCurrent &&
            haveSamePosition(*pCurrent, targetTile))
        {
            // instead of adding the target tile position to the path, add the actual target position
            result.emplace_back(rTargetPosition);
            if (pCurrent->m_pPredecessor)
                pCurrent = pCurrent->m_pPredecessor;

            while (pCurrent->m_pPredecessor)
            {
                result.push_back({pCurrent->m_column * rTileMap.m_tileWidth + rMapOffset.m_value[0] + rTileMap.m_tileWidth / 2,
                                  pCurrent->m_row * rTileMap.m_tileHeight + rMapOffset.m_value[1] + rTileMap.m_tileHeight / 2});
                pCurrent = pCurrent->m_pPredecessor;
            }
        }

        return result;
    }
}
