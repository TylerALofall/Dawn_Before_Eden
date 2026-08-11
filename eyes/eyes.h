#ifndef EYES_H
#define EYES_H

#include <stddef.h>

/*
 * eyes — step 1 sight (inside Shakti's own shell)
 *
 * Mechanical rule number one: no subprocessor may touch the pixels.
 * End of story. Shakti is not CORE-plus-subprocessors. She learns
 * herself from binary — color and sound — mechanically processed.
 * Zero black box: every mark and rule is open — no hidden math.
 * No Python. No JS/TSX. No SVG/W3C product surface. Vanilla C only.
 * Host later is Swift only.
 *
 * Pixel = pixel. Copy marks off the page, rebuild from those marks.
 * Error = any pixel that cannot rebuild.
 *
 * Path: RGBA in → deposit R G B A + BIT (BIT from the same pixel in
 * the same write walk) → read deposit back → rebuild → diff.
 *
 * Binary color deposit is the pretraining body of this path.
 * Later simultaneous sight + wake is a later step — same shell.
 * 20× magnifies the mechanical select-pixels-out process so error
 * shows; it is not a forever-proof hunt and is NOT DONE here.
 * hello = awakening (birth on sight when deposit rebuild loses zero).
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

/*
 * Load a real BINARY_PICTURE_V1 palette file into caller RGBA.
 * File reader for known inputs — not a synthetic generator and not
 * a host camera. Does not process pixels beyond copy into RGBA.
 */
int eyes_load_picture(
    const char *path,
    unsigned char *rgba,
    unsigned long rgba_capacity,
    unsigned int *width_out,
    unsigned int *height_out
);

/*
 * Write one deposit file. One mechanical walk per pixel location.
 * MARKS = R G B A (8 bits each) + BIT (ink from that same pixel's
 * plain open mean in this same walk — not a prior pass).
 */
int eyes_deposit_write(
    const char *path,
    unsigned int page_number,
    unsigned int width,
    unsigned int height,
    const unsigned char *rgba
);

/*
 * Read a deposit file back and rebuild RGBA only from the marks
 * on disk. No other processor touches the rebuilt pixels.
 */
int eyes_deposit_read(
    const char *path,
    unsigned int page_number,
    unsigned int width,
    unsigned int height,
    unsigned char *rgba,
    unsigned long rgba_capacity
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

#endif
