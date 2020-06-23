#include "labyrinth/include/system/collision.h"

#include "labyrinth/include/component/collider.h"
#include "labyrinth/include/component/grid_collider.h"
#include "labyrinth/include/component/position.h"
#include "labyrinth/include/component/velocity.h"

#include "plight/include/common/math.h"

#include "entt/src/entt/entt.hpp"

#include <iostream>


namespace Labyrinth::System::Collision
{
    /*
        Detect a collision between two collider rectangles
    */
    bool
    detectCollision(int lhsOffsetX,
                    int lhsOffsetY,
                    int rhsOffsetX,
                    int rhsOffsetY,
                    Component::ColliderRectangle const& rLhsRect,
                    Component::ColliderRectangle const& rRhsRect)
    {
        return rLhsRect.m_pos[0] + lhsOffsetX < rRhsRect.m_pos[0] + rRhsRect.m_dim[0] + rhsOffsetX
            && rRhsRect.m_pos[0] + rhsOffsetX < rLhsRect.m_pos[0] + rLhsRect.m_dim[0] + lhsOffsetX
            && rLhsRect.m_pos[1] + lhsOffsetY < rRhsRect.m_pos[1] + rRhsRect.m_dim[1] + rhsOffsetY
            && rRhsRect.m_pos[1] + rhsOffsetY < rLhsRect.m_pos[1] + rLhsRect.m_dim[1] + lhsOffsetY;
    }

    /*
        Detect a collision between a grid collider and a collider rectangle
    */
    bool
    detectCollision(Component::GridCollider const& rGridCollider,
                    Component::ColliderRectangle const& rColliderRect,
                    int gridOffsetX,
                    int gridOffsetY,
                    int offsetX,
                    int offsetY)
    {
        auto const findColumn = [&](int x)
        {
            return (x - gridOffsetX) / rGridCollider.m_cellDim[0];
        };

        auto const findRow = [&](int y)
        {
            return (y - gridOffsetY) / rGridCollider.m_cellDim[1];
        };

        auto const lowerRow = findRow(offsetY + rColliderRect.m_pos[1]);
        auto const upperRow = findRow(offsetY + rColliderRect.m_pos[1] + rColliderRect.m_dim[1] - 1);
        auto const leftColumn = findColumn(offsetX + rColliderRect.m_pos[0]);
        auto const rightColumn = findColumn(offsetX + rColliderRect.m_pos[0] + rColliderRect.m_dim[0] - 1);

        for (int row = std::max(lowerRow, 0); row < std::min(upperRow + 1, rGridCollider.m_dim[1]); ++row)
        {
            for (int column = std::max(leftColumn, 0); column < std::min(rightColumn + 1, rGridCollider.m_dim[0]); ++column)
            {
                if (rGridCollider.m_occupancies.at(static_cast<size_t>(row) * rGridCollider.m_dim[0] + column))
                    return true;
            }
        }
        return false;
    }

    /*
        Detect a collision between a grid collider and a collider
    */
    bool
    detectCollision(Component::GridCollider const& rGridCollider,
                    Component::Collider const& rCollider,
                    int gridOffsetX,
                    int gridOffsetY,
                    int offsetX,
                    int offsetY)
    {
        for (auto const& rColliderRectangle : rCollider.m_colliderRectangles)
            if (detectCollision(rGridCollider,
                                rColliderRectangle,
                                gridOffsetX,
                                gridOffsetY,
                                offsetX,
                                offsetY))
                return true;
        return false;
    }

    /*
        Resolve a collision between an moving object with collider and a tile map with grid collider
    */
    void
    resolve(Component::Position const& rPosition,
            Component::Velocity& rVelocity,
            Component::Collider const& rCollider,
            Component::Position const& rGridPosition,
            Component::GridCollider const& rGridCollider)
    {
        auto const signZero = [](int x)
        {
            return x == 0 ? 0 : (x < 0 ? -1 : 1);
        };

        int dTile[2];
        dTile[0] = signZero(rVelocity.m_delta[0]);
        dTile[1] = signZero(rVelocity.m_delta[1]);

        // Find a coordinate of the tile the object reaches next (based on a coordinate)
        auto const coordinateToTileCoordinate = [&](int coordinate, size_t index)
        {
            return (coordinate - rGridPosition.m_value[index] + dTile[index] * (rGridCollider.m_cellDim[index] / 2 - 1)) / rGridCollider.m_cellDim[index];
        };

        // Find the coodinate the object is trying to reach (based on a tile coordinate)
        auto const tileCoordinateToTargetCoordinate = [&](int tileCoordinate, size_t index)
        {
            return tileCoordinate * rGridCollider.m_cellDim[index] + rGridCollider.m_cellDim[index] / 2 - dTile[index] * (rCollider.m_dim[index] / 2 - rGridCollider.m_cellDim[index] / 2) + rGridPosition.m_value[index];
        };

        // Find the tile the object reaches next
        int tile[2];
        tile[0] = coordinateToTileCoordinate(rPosition.m_value[0], 0);
        tile[1] = coordinateToTileCoordinate(rPosition.m_value[1], 1);

        // If only moving on x-axis
        if (dTile[1] == 0)
        {
            rVelocity.m_delta[0] = signZero(rVelocity.m_delta[0]) * std::min(std::abs(rVelocity.m_delta[0]),
                                                                             std::abs(tileCoordinateToTargetCoordinate(tile[0], 0) - rPosition.m_value[0]));
            rVelocity.m_lastBlockedIndex = 0;
            return;
        }

        // If only moving on y-axis
        if (dTile[0] == 0)
        {
            rVelocity.m_delta[1] = signZero(rVelocity.m_delta[1]) * std::min(std::abs(rVelocity.m_delta[1]),
                                                                             std::abs(tileCoordinateToTargetCoordinate(tile[1], 1) - rPosition.m_value[1]));
            rVelocity.m_lastBlockedIndex = 1;
            return;
        }

        // First try to move into the direction that was blocked last
        auto changeTileIndex = rVelocity.m_lastBlockedIndex;
        auto remainTileIndex = (rVelocity.m_lastBlockedIndex + 1) & 1;

        int target[2];
        target[changeTileIndex] = tileCoordinateToTargetCoordinate(tile[changeTileIndex] + dTile[changeTileIndex], changeTileIndex);
        target[remainTileIndex] = tileCoordinateToTargetCoordinate(tile[remainTileIndex], remainTileIndex);

        auto const lastBlockedDirectionStillBlocked = detectCollision(rGridCollider,
                                                                      rCollider,
                                                                      rGridPosition.m_value[0],
                                                                      rGridPosition.m_value[1],
                                                                      target[0],
                                                                      target[1]);

        // If still blocked, try to move into the other direction
        if (lastBlockedDirectionStillBlocked)
        {
            changeTileIndex = remainTileIndex;
            remainTileIndex = rVelocity.m_lastBlockedIndex;

            target[changeTileIndex] = tileCoordinateToTargetCoordinate(tile[changeTileIndex] + dTile[changeTileIndex], changeTileIndex);
            target[remainTileIndex] = tileCoordinateToTargetCoordinate(tile[remainTileIndex], remainTileIndex);

            // If that direction is blocked as well, just move up to the wall
            if (detectCollision(rGridCollider,
                                rCollider,
                                rGridPosition.m_value[0],
                                rGridPosition.m_value[1],
                                target[0],
                                target[1]))
            {
                target[0] = tileCoordinateToTargetCoordinate(tile[0], 0);
                target[1] = tileCoordinateToTargetCoordinate(tile[1], 1);
            }
        }

        // The actual movement: first move on the axis where the tile does not change, then move the rest of the way on the other axis
        auto const norm = Plight::sqrt(rVelocity.m_delta[0] * rVelocity.m_delta[0] + rVelocity.m_delta[1] * rVelocity.m_delta[1]);
        auto const remainTileDiff = target[remainTileIndex] - rPosition.m_value[remainTileIndex];
        auto const changeTileDiff = target[changeTileIndex] - rPosition.m_value[changeTileIndex];

        rVelocity.m_delta[remainTileIndex] = signZero(remainTileDiff) * std::min(std::abs(remainTileDiff), norm);
        rVelocity.m_delta[changeTileIndex] = signZero(changeTileDiff) * std::min(std::abs(changeTileDiff), Plight::sqrt(norm * norm - rVelocity.m_delta[remainTileIndex] * rVelocity.m_delta[remainTileIndex]));

        // If the object actually moved in the chosen direction, mark the other direction as last blocked
        if (rVelocity.m_delta[changeTileIndex] != 0)
            rVelocity.m_lastBlockedIndex = remainTileIndex;
    }

    /*
        Resolve all collisions
    */
    void
    update(entt::registry& rRegistry)
    {
        rRegistry.view<Component::Position,
                       Component::Velocity,
                       Component::Collider>().each([&](auto const entityLhs, auto const& rPositionLhs, auto& rVelocityLhs, auto const& rColliderLhs)
                                                   {
                                                       rRegistry.view<Component::Position,
                                                                      Component::GridCollider>().each([&](auto const& rGridPosition, auto const& rGridCollider)
                                                                                                      {
                                                                                                          if (detectCollision(rGridCollider,
                                                                                                                              rColliderLhs,
                                                                                                                              rGridPosition.m_value[0],
                                                                                                                              rGridPosition.m_value[1],
                                                                                                                              rPositionLhs.m_value[0] + rVelocityLhs.m_delta[0],
                                                                                                                              rPositionLhs.m_value[1] + rVelocityLhs.m_delta[1]))
                                                                                                              resolve(rPositionLhs,
                                                                                                                      rVelocityLhs,
                                                                                                                      rColliderLhs,
                                                                                                                      rGridPosition,
                                                                                                                      rGridCollider);
                                                                                                      });
                                                   
                                                       rRegistry.view<Component::Position,
                                                                      Component::Velocity,
                                                                      Component::Collider>().each([&](auto const entityRhs, auto const& rPositionRhs, auto const& rVelocityRhs, auto const& rColliderRhs)
                                                                                                  {
                                                                                                      if (entityLhs >= entityRhs)
                                                                                                          return;
                                           
                                                                                                      for (auto const& rLhsColliderRectangle : rColliderLhs.m_colliderRectangles)
                                                                                                      {
                                                                                                          for (auto const& rRhsColliderRectangle : rColliderRhs.m_colliderRectangles)
                                                                                                          {
                                                                                                              if (detectCollision(rPositionLhs.m_value[0] + rVelocityLhs.m_delta[0],
                                                                                                                                  rPositionLhs.m_value[1] + rVelocityLhs.m_delta[1],
                                                                                                                                  rPositionRhs.m_value[0] + rVelocityRhs.m_delta[0],
                                                                                                                                  rPositionRhs.m_value[1] + rVelocityRhs.m_delta[1],
                                                                                                                                  rLhsColliderRectangle, rRhsColliderRectangle))
                                                                                                              {
                                                                                                                  // Collision!
                                                                                                              }
                                                                                                          }
                                                                                                      }
                                                                                                  });
                                                   });
    }
}
