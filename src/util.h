#ifndef __UTIL_H__

#define __UTIL_H__

#include <stddef.h>
#include <stdint.h>

/**
 * @brief utility function for putting random bytes into a buffer of len length
 * @param p the buffer to put the random bytes into
 * @param len the available length of p
 */
void get_random_bytes(uint8_t* p, size_t len);

#endif /* __UTIL_H__ */
