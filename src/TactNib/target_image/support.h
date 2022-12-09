/** ===========================================================================
 *
 * File Name: support.h
 * Purpose:	  Place holder for miscellaneous support functions.
 * Author:	  Michael Eaton
 *
 * ============================================================================*/

#ifndef SYSTEM_API_SUPPORT_H
#define SYSTEM_API_SUPPORT_H


// Template to allow "enum class" to be used in cout
template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

#endif //SYSTEM_API_SUPPORT_H
