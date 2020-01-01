#include <iostream>

#define LOG(msg)                                                               \
    do {                                                                       \
        std::cerr << __FILE__ << ":" << __LINE__ << ": " << msg << std::endl;  \
    } while (0)
