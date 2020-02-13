#pragma once
#include <vector>


namespace Plight::Graphics::Uniform
{
    void                        updateFloatBuffer(unsigned int,
                                                  std::vector<float> const&);

    void                        updateIntBuffer(unsigned int,
                                                std::vector<int> const&);
}
