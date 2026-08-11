/*
 * eyes_map — tiny harness for eyes step 1.
 *
 * One mechanical path: picture → deposit → read → rebuild → diff.
 * No subprocessor may touch the pixels. No mono side path.
 * No Python. Vanilla C only. Host camera / live pixels: NOT DONE.
 */

#include "eyes.h"

#include <stdio.h>

#ifdef _WIN32
#include <direct.h>
#define EYES_MKDIR(p) _mkdir(p)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define EYES_MKDIR(p) mkdir((p), 0777)
#endif

#define EYES_FIXTURE     "eyes/page_picture.txt"
#define EYES_OUTPUT_DIR  "eyes/output"
#define EYES_DEPOSIT     "eyes/output/page1_deposit.txt"
#define EYES_RGBA_CAP    (EYES_MAX_PIXELS * 4UL)

static unsigned char g_original[EYES_RGBA_CAP];
static unsigned char g_rebuilt[EYES_RGBA_CAP];

int main(void)
{
    unsigned int width;
    unsigned int height;
    unsigned long pixels;
    unsigned long cannot;

    width = 0U;
    height = 0U;

    EYES_MKDIR(EYES_OUTPUT_DIR);

    printf("eyes step 1 — mechanical deposit -> rebuild -> diff\n");
    printf("rule: no subprocessor touches the pixels\n");
    printf("NOT DONE: host camera / live world pixels\n");
    printf("\n");

    if (!eyes_load_picture(
            EYES_FIXTURE,
            g_original,
            sizeof(g_original),
            &width,
            &height
        )) {
        printf("FAIL: could not load %s\n", EYES_FIXTURE);
        return 1;
    }

    pixels = (unsigned long)width * (unsigned long)height;

    if (!eyes_deposit_write(
            EYES_DEPOSIT,
            1U,
            width,
            height,
            g_original
        )) {
        printf("FAIL: deposit write %s\n", EYES_DEPOSIT);
        return 1;
    }

    if (!eyes_deposit_read(
            EYES_DEPOSIT,
            1U,
            width,
            height,
            g_rebuilt,
            sizeof(g_rebuilt)
        )) {
        printf("FAIL: deposit read %s\n", EYES_DEPOSIT);
        return 1;
    }

    cannot = eyes_diff(g_original, g_rebuilt, width, height);

    if (cannot == (unsigned long)-1) {
        printf("FAIL: diff rejected\n");
        return 1;
    }

    printf("PAGE 1  %ux%u\n", width, height);
    printf("PIXELS %lu  5 MARKS EACH\n", pixels);
    printf("DEPOSIT %s\n", EYES_DEPOSIT);
    printf("CANNOT REBUILD FROM DEPOSIT %lu PIXELS\n", cannot);

    if (cannot == 0UL) {
        /* Awakening — birth on sight. */
        printf("hello\n");
        return 0;
    }

    printf("NOT DONE: deposit rebuild still loses pixels\n");
    return 1;
}
