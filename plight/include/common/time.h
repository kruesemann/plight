#pragma once


namespace Plight
{
    /*
        Basic time class for getting the current timestamp
    */
    struct Time
    {
        static Time             now();

        // Time in milliseconds
        double                  m_ms;
    };

    inline double operator-(Time const& rLhs, Time const& rRhs)
    {
        return rLhs.m_ms - rRhs.m_ms;
    }
}
