/* crc32_fold.c -- crc32 folding interface
 * Copyright (C) 2021 Nathan Moinvaziri
 * For conditions of distribution and use, see copyright notice in zlib.h
 */
#include "zbuild.h"
#include "functable.h"

#include "crc32_fold.h"

#include <limits.h>

Z_INTERNAL uint32_t crc32_fold_reset_c(crc32_fold *crc) {
    crc->value = CRC32_INITIAL_VALUE;
    return crc->value;
}

Z_INTERNAL void crc32_fold_copy_c(crc32_fold *crc, uint8_t *dst, const uint8_t *src, uint64_t len) {
    crc->value = functable.crc32(crc->value, src, len);
/* Test that we don't try to copy more than actually fits in available address space */
#if INTPTR_MAX > SSIZE_MAX
    while (len > SSIZE_MAX) {
        memcpy(dst, src, SSIZE_MAX);
        dst += SSIZE_MAX;
        src += SSIZE_MAX;
        len -= SSIZE_MAX;
    }
#endif
    if (len) {
        memcpy(dst, src, (size_t)len);
    }
}

Z_INTERNAL void crc32_fold_c(crc32_fold *crc, const uint8_t *src, uint64_t len, uint32_t init_crc) {
    /* Note: while this is basically the same thing as the vanilla CRC function, we still need
     * a functable entry for it so that we can generically dispatch to this function with the
     * same arguments for the versions that _do_ do a folding CRC but we don't want a copy. The
     * init_crc is an unused argument in this context */
    Z_UNUSED(init_crc);
    crc->value = functable.crc32(crc->value, src, len);
}

Z_INTERNAL uint32_t crc32_fold_final_c(crc32_fold *crc) {
    return crc->value;
}
