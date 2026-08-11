#ifndef EYES_H
#define EYES_H

#include <stddef.h>

/*
 * eyes — sight core (step 1)
 *
 * Mechanical rule number one: no subprocessor may touch the pixels.
 * End of story. One core. One pass over the marks. No second path.
 *
 * Pixel = pixel. Copy marks off the page, rebuild from those marks.
 * Error = any pixel that cannot rebuild.
 *
 * Path: RGBA in → deposit R G B A + BIT (BIT from the same pixel in
 * the same write walk) → read deposit back → rebuild → diff.
 *
 * Binary color deposit is the pretraining body of this path.
 * Later simultaneous sight + wake is a later step — not a second core.
 * 20× is only for finding mistakes later; not a forever-proof hunt.
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
 * luma in this same walk — not a prior subprocessor pass).
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
