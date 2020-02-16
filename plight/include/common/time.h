#pragma once


namespace Plight
{
    /*
        Basic time class for getting the current timestamp
    */
    struct Time
    {
        static Time             now();

        double                  m_seconds;
    };

    inline double operator-(Time const& rLhs, Time const& rRhs)
    {
        return rLhs.m_seconds - rRhs.m_seconds;
    }
}
