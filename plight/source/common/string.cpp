#include "plight/include/common/string.h"


namespace Plight
{
    String::String(char const* const chars)
        : TInherited(chars)
    {
    }

    String::String(std::string const& rString)
        : TInherited(rString)
    {
    }
}
