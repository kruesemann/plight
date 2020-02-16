#pragma once
#include "entt/src/entt/fwd.hpp"


namespace Labyrinth::System::UniformModelViewMatrix
{
    /*
        System for updating the uniform "u_modelViewMatrix"
    */
    void                    update(entt::registry const&);
}
