#pragma once


namespace Labyrinth::Component
{
    /*
        Component holding a move that is tried
    */
    struct Velocity
    {
        int                     m_delta[2] = {0, 0};

        // Index of the direction that was blocked last (will be preferred in next move)
        size_t                  m_lastBlockedIndex = 0;
    };
}
