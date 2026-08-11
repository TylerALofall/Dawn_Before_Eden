#ifndef EYES_H
#define EYES_H

#include <stddef.h>

/*
 * eyes — sight core (step 1)
 *
 * Rule (truce): a pixel is a pixel. Copy pixels off the document as
 * binary marks, rebuild the page from those marks, and count every
 * pixel that cannot rebuild. That count is the only error.
 *
 * One honest path: accept RGBA (or load a real picture file)
 *   -> deposit R G B A + mono BIT
 *   -> read deposit back
 *   -> rebuild
 *   -> diff
 *
 * No 20× cycle hunt. No generated fake pages as product truth.
 * Host camera / live pixels: NOT DONE (see eyes/README.md).
 */

#define EYES_MAX_PIXELS 65536UL
#define EYES_PATH_MAX   256UL
#define EYES_LINE_MAX   256UL

/*
 * Return codes: 1 success, 0 rejection.
 * eyes_diff returns drifted pixel count, or (unsigned long)-1 on reject.
 */

int eyes_dimensions_ok(unsigned int width, unsigned int height);

/* Mono ink bit: luma < 128 -> '1' ink, else '0' paper. */
int eyes_pull_mono(
    const unsigned char *rgba,
    unsigned int width,
    unsigned int height,
    char *bits,
    unsigned long bits_capacity
);

/*
 * Load a real BINARY_PICTURE_V1 palette file into caller RGBA.
 * This is a file reader for known inputs — not a synthetic document
 * generator and not a host camera.
 */
int eyes_load_picture(
    const char *path,
    unsigned char *rgba,
    unsigned long rgba_capacity,
    unsigned int *width_out,
    unsigned int *height_out
);

/*
 * Write one deposit file: 5 marks per pixel location.
 * MARKS = R G B A (8 bits each) + BIT (mono ink, 1 bit).
 */
int eyes_deposit_write(
    const char *path,
    unsigned int page_number,
    unsigned int width,
    unsigned int height,
    const unsigned char *rgba,
    const char *bits
);

/*
 * Read a deposit file back and rebuild RGBA + mono bits only from
 * the marks on disk.
 */
int eyes_deposit_read(
    const char *path,
    unsigned int page_number,
    unsigned int width,
    unsigned int height,
    unsigned char *rgba,
    unsigned long rgba_capacity,
    char *bits,
    unsigned long bits_capacity
);

/*
 * Exact RGBA per-pixel compare. Drift = pixels that did not come back.
 * Zero means every pixel rebuilt.
 */
unsigned long eyes_diff(
    const unsigned char *original,
    const unsigned char *reconstructed,
    unsigned int width,
    unsigned int height
);

/* Rebuild black/white page from mono bits alone (lossy for color). */
int eyes_rebuild_mono(
    const char *bits,
    unsigned int width,
    unsigned int height,
    unsigned char *rgba,
    unsigned long rgba_capacity
);

#endif
