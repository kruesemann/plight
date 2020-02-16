#pragma once
#include "plight/include/common/string.h"


namespace Plight::Graphics
{
    /*
        GLSL uniform types
    */
    enum class EUniformType
    {
        Bool,
        BoolVec2,
        BoolVec3,
        BoolVec4,
        BoolMat2,
        BoolMat2x3,
        BoolMat2x4,
        BoolMat3x2,
        BoolMat3,
        BoolMat3x4,
        BoolMat4x2,
        BoolMat4x3,
        BoolMat4,
        Int,
        IntVec2,
        IntVec3,
        IntVec4,
        IntMat2,
        IntMat2x3,
        IntMat2x4,
        IntMat3x2,
        IntMat3,
        IntMat3x4,
        IntMat4x2,
        IntMat4x3,
        IntMat4,
        UInt,
        UIntVec2,
        UIntVec3,
        UIntVec4,
        UIntMat2,
        UIntMat2x3,
        UIntMat2x4,
        UIntMat3x2,
        UIntMat3,
        UIntMat3x4,
        UIntMat4x2,
        UIntMat4x3,
        UIntMat4,
        Float,
        FloatVec2,
        FloatVec3,
        FloatVec4,
        FloatMat2,
        FloatMat2x3,
        FloatMat2x4,
        FloatMat3x2,
        FloatMat3,
        FloatMat3x4,
        FloatMat4x2,
        FloatMat4x3,
        FloatMat4,
        Double,
        DoubleVec2,
        DoubleVec3,
        DoubleVec4,
        DoubleMat2,
        DoubleMat2x3,
        DoubleMat2x4,
        DoubleMat3x2,
        DoubleMat3,
        DoubleMat3x4,
        DoubleMat4x2,
        DoubleMat4x3,
        DoubleMat4
    };

    /*
        Basic data for shader uniform initialization
    */
    struct UniformInfo
    {
                            UniformInfo(String const& rName, EUniformType dataType)
                                : m_name(rName)
                                , m_dataType(dataType) {}

        String              m_name;

        EUniformType        m_dataType;
    };

    /*
        Basic data for shader uniform buffer initialization
    */
    struct UniformBufferInfo
    {
                            UniformBufferInfo(String const& rName, size_t rawSize)
                                : m_name(rName)
                                , m_rawSize(rawSize) {}

        String              m_name;

        size_t              m_rawSize;
    };
}
