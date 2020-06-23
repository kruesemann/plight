#pragma once
#include <vector>


namespace Labyrinth::Component
{
    /*
        Rectangle for colliders
    */
    struct ColliderRectangle
    {
        // Position on the grid
        int                     m_pos[2] = {0, 0};

        // Dimensions
        int                     m_dim[2] = {0, 0};
    };

    /*
        Component for detecting collisions with other entities
    */
    struct Collider
    {
        // Collection of rectangles the collider is composed of
        std::vector<ColliderRectangle> m_colliderRectangles;

        // Dimension approximation for resolving blocks
        int                     m_dim[2] = {0, 0};

        // Offset on x-axis
        int                     m_offsetX = 0;

        // Offset on y-axis
        int                     m_offsetY = 0;
    };
}
