#include <iostream>

#ifdef GOL_NO_LOG
#define LOG(msg) do{}while(0)
#else
#define LOG(msg)                                                               \
    do {                                                                       \
        std::cerr << __FILE__ << ":" << __LINE__ << ": " << msg << std::endl;  \
    } while (0)
#endif
