#ifndef FB_DEBUG_HPP
#define FB_DEBUG_HPP

// debug utilities written as C++11

namespace fb {
namespace debug {
#ifdef FB_DEBUG

#endif
} // namespace fb
} // namespace debug

#ifdef FB_DEBUG

#else
#define FB_SCOPED_LOGGER
#endif
#endif
