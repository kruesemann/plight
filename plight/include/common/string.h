#pragma once
#include <regex>
#include <sstream>


namespace Plight
{
    /*
        String class for easy formatting
    */
    class String : public std::string
    {
        using TInherited = std::string;

    public:

                                String() = default;
        explicit                String(char const* const);
        explicit                String(std::string const&);

        /*
            Convert an object to a String.
            Only works for types an std::stringstream will take as an argument.
        */
        template<typename T>
        static String
        toString(T&& rObject)
        {
            std::stringstream stringStream;
            stringStream << rObject;
            return String(stringStream.str());
        }

        /*
            Replace the first occurence of '%' with given argument.
            Only works for types an std::stringstream will take as an argument.
        */
        template<typename Argument>
        String
        arg(Argument&& rArgument) const
        {
            auto const replaceString = toString(std::forward<Argument>(rArgument));
            return String(std::regex_replace(*this, std::regex("%"), replaceString, std::regex_constants::format_first_only));
        }

        /*
            Replace the first occurences of '%' with given arguments.
            Only works for types an std::stringstream will take as an argument.
        */
        template<typename Argument, typename... Arguments>
        String
        arg(Argument&& rArgument, Arguments... arguments) const
        {
            return arg(rArgument).arg(arguments...);
        }

    };
}
