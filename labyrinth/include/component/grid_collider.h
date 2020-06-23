#pragma once
#include <vector>


namespace Labyrinth::Component
{
    /*
        Component holding an offset grid of occupancies
    */
    struct GridCollider
    {
        // Occupancy grid
        std::vector<bool>       m_occupancies;

        // Dimensions of the grid
        int                     m_dim[2] = {0, 0};

        // Dimensions of a cell
        int                     m_cellDim[2] = {1, 1};
    };
}
