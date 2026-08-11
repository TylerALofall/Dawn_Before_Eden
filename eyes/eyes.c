#include "eyes.h"

#include <stdio.h>
#include <string.h>

int eyes_dimensions_ok(unsigned int width, unsigned int height)
{
    return width > 0U &&
           height > 0U &&
           (unsigned long)width * (unsigned long)height <= EYES_MAX_PIXELS;
}

/* Ink mark for one pixel — used only inside the deposit write walk. */
static char bit_of_pixel(
    const unsigned char *rgba,
    unsigned long pixel_index
)
{
    unsigned long base;
    unsigned int luma;

    base = pixel_index * 4UL;
    luma = ((unsigned int)rgba[base] * 30U +
            (unsigned int)rgba[base + 1UL] * 59U +
            (unsigned int)rgba[base + 2UL] * 11U) / 100U;

    return luma >= 128U ? '0' : '1';
}

static void strip_line_end(char *line)
{
    unsigned long length;

    length = (unsigned long)strlen(line);

    while (length > 0UL &&
           (line[length - 1UL] == '\n' || line[length - 1UL] == '\r')) {
        line[length - 1UL] = '\0';
        --length;
    }
}

static int palette_rgb(
    char letter,
    unsigned char *red,
    unsigned char *green,
    unsigned char *blue
)
{
    switch (letter) {
    case 'W': *red = 255U; *green = 255U; *blue = 255U; return 1;
    case 'K': *red = 0U;   *green = 0U;   *blue = 0U;   return 1;
    case 'R': *red = 255U; *green = 0U;   *blue = 0U;   return 1;
    case 'G': *red = 0U;   *green = 255U; *blue = 0U;   return 1;
    case 'B': *red = 0U;   *green = 0U;   *blue = 255U; return 1;
    case 'Y': *red = 255U; *green = 255U; *blue = 0U;   return 1;
    case 'C': *red = 0U;   *green = 255U; *blue = 255U; return 1;
    case 'M': *red = 255U; *green = 0U;   *blue = 255U; return 1;
    case 'D': *red = 128U; *green = 128U; *blue = 128U; return 1;
    default:  return 0;
    }
}

static int write_octet(FILE *file, unsigned char value)
{
    unsigned int bit;

    for (bit = 8U; bit > 0U; --bit) {
        int mark;

        mark = ((value >> (bit - 1U)) & 1U) ? '1' : '0';

        if (fputc(mark, file) == EOF) {
            return 0;
        }
    }

    return 1;
}

static int octet_value(const char *marks, unsigned char *value_out)
{
    unsigned int value;
    unsigned int bit;

    if ((unsigned long)strlen(marks) != 8UL) {
        return 0;
    }

    value = 0U;

    for (bit = 0U; bit < 8U; ++bit) {
        if (marks[bit] != '0' && marks[bit] != '1') {
            return 0;
        }

        value = (value << 1U) | (unsigned int)(marks[bit] - '0');
    }

    *value_out = (unsigned char)value;
    return 1;
}

int eyes_load_picture(
    const char *path,
    unsigned char *rgba,
    unsigned long rgba_capacity,
    unsigned int *width_out,
    unsigned int *height_out
)
{
    FILE *file;
    unsigned int width;
    unsigned int height;
    unsigned int row;
    char line[EYES_LINE_MAX];

    if (path == NULL || rgba == NULL ||
        width_out == NULL || height_out == NULL) {
        return 0;
    }

    file = fopen(path, "r");

    if (file == NULL) {
        return 0;
    }

    width = 0U;
    height = 0U;

    if (fgets(line, (int)sizeof(line), file) == NULL ||
        (strip_line_end(line), strcmp(line, "BINARY_PICTURE_V1")) != 0 ||
        fgets(line, (int)sizeof(line), file) == NULL ||
        sscanf(line, "WIDTH=%u", &width) != 1 ||
        fgets(line, (int)sizeof(line), file) == NULL ||
        sscanf(line, "HEIGHT=%u", &height) != 1 ||
        fgets(line, (int)sizeof(line), file) == NULL ||
        strncmp(line, "PALETTE", 7UL) != 0 ||
        fgets(line, (int)sizeof(line), file) == NULL ||
        (strip_line_end(line), strcmp(line, "ROWS")) != 0 ||
        !eyes_dimensions_ok(width, height) ||
        rgba_capacity < (unsigned long)width * (unsigned long)height * 4UL) {
        fclose(file);
        return 0;
    }

    for (row = 0U; row < height; ++row) {
        unsigned int column;

        if (fgets(line, (int)sizeof(line), file) == NULL) {
            fclose(file);
            return 0;
        }

        strip_line_end(line);

        if ((unsigned long)strlen(line) != (unsigned long)width) {
            fclose(file);
            return 0;
        }

        for (column = 0U; column < width; ++column) {
            unsigned long base;
            unsigned char red;
            unsigned char green;
            unsigned char blue;

            if (!palette_rgb(line[column], &red, &green, &blue)) {
                fclose(file);
                return 0;
            }

            base = ((unsigned long)row * (unsigned long)width +
                    (unsigned long)column) * 4UL;
            rgba[base] = red;
            rgba[base + 1UL] = green;
            rgba[base + 2UL] = blue;
            rgba[base + 3UL] = 255U;
        }
    }

    fclose(file);
    *width_out = width;
    *height_out = height;
    return 1;
}

int eyes_deposit_write(
    const char *path,
    unsigned int page_number,
    unsigned int width,
    unsigned int height,
    const unsigned char *rgba
)
{
    FILE *file;
    unsigned long pixel_count;
    unsigned long index;
    int success;

    if (path == NULL || rgba == NULL ||
        !eyes_dimensions_ok(width, height)) {
        return 0;
    }

    pixel_count = (unsigned long)width * (unsigned long)height;

    file = fopen(path, "w");

    if (file == NULL) {
        return 0;
    }

    success =
        fprintf(file, "BINARY_DEPOSIT_V1\n") > 0 &&
        fprintf(file, "PAGE=%u\n", page_number) > 0 &&
        fprintf(file, "WIDTH=%u\n", width) > 0 &&
        fprintf(file, "HEIGHT=%u\n", height) > 0 &&
        fprintf(file, "MARKS=R G B A BIT\n") > 0;

    for (index = 0UL; success && index < pixel_count; ++index) {
        unsigned long base;
        unsigned int mark;
        char ink;

        base = index * 4UL;
        ink = bit_of_pixel(rgba, index);

        success = fprintf(
            file,
            "X=%02lu Y=%02lu ",
            index % (unsigned long)width,
            index / (unsigned long)width
        ) > 0;

        for (mark = 0U; success && mark < 4U; ++mark) {
            success = write_octet(file, rgba[base + (unsigned long)mark]) &&
                      fputc(' ', file) != EOF;
        }

        success = success &&
                  fputc(ink, file) != EOF &&
                  fputc('\n', file) != EOF;
    }

    success = success && fprintf(file, "END\n") > 0 && fflush(file) == 0;

    if (fclose(file) != 0) {
        success = 0;
    }

    return success;
}

int eyes_deposit_read(
    const char *path,
    unsigned int page_number,
    unsigned int width,
    unsigned int height,
    unsigned char *rgba,
    unsigned long rgba_capacity
)
{
    FILE *file;
    unsigned long pixel_count;
    unsigned long index;
    unsigned int header_page;
    unsigned int header_width;
    unsigned int header_height;
    char line[EYES_LINE_MAX];

    if (path == NULL || rgba == NULL ||
        !eyes_dimensions_ok(width, height)) {
        return 0;
    }

    pixel_count = (unsigned long)width * (unsigned long)height;

    if (rgba_capacity < pixel_count * 4UL) {
        return 0;
    }

    file = fopen(path, "r");

    if (file == NULL) {
        return 0;
    }

    header_page = 0U;
    header_width = 0U;
    header_height = 0U;

    if (fgets(line, (int)sizeof(line), file) == NULL ||
        (strip_line_end(line), strcmp(line, "BINARY_DEPOSIT_V1")) != 0 ||
        fgets(line, (int)sizeof(line), file) == NULL ||
        sscanf(line, "PAGE=%u", &header_page) != 1 ||
        fgets(line, (int)sizeof(line), file) == NULL ||
        sscanf(line, "WIDTH=%u", &header_width) != 1 ||
        fgets(line, (int)sizeof(line), file) == NULL ||
        sscanf(line, "HEIGHT=%u", &header_height) != 1 ||
        fgets(line, (int)sizeof(line), file) == NULL ||
        (strip_line_end(line), strcmp(line, "MARKS=R G B A BIT")) != 0 ||
        header_page != page_number ||
        header_width != width ||
        header_height != height) {
        fclose(file);
        return 0;
    }

    for (index = 0UL; index < pixel_count; ++index) {
        unsigned long x;
        unsigned long y;
        char red_marks[9];
        char green_marks[9];
        char blue_marks[9];
        char alpha_marks[9];
        char bit_mark[2];
        unsigned long base;

        if (fgets(line, (int)sizeof(line), file) == NULL ||
            sscanf(
                line,
                "X=%lu Y=%lu %8[01] %8[01] %8[01] %8[01] %1[01]",
                &x, &y, red_marks, green_marks, blue_marks,
                alpha_marks, bit_mark
            ) != 7 ||
            x != index % (unsigned long)width ||
            y != index / (unsigned long)width) {
            fclose(file);
            return 0;
        }

        base = index * 4UL;

        /* BIT is on the deposit line; rebuild is RGBA marks only. */
        (void)bit_mark;

        if (!octet_value(red_marks, &rgba[base]) ||
            !octet_value(green_marks, &rgba[base + 1UL]) ||
            !octet_value(blue_marks, &rgba[base + 2UL]) ||
            !octet_value(alpha_marks, &rgba[base + 3UL])) {
            fclose(file);
            return 0;
        }
    }

    if (fgets(line, (int)sizeof(line), file) == NULL ||
        (strip_line_end(line), strcmp(line, "END")) != 0) {
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

unsigned long eyes_diff(
    const unsigned char *original,
    const unsigned char *reconstructed,
    unsigned int width,
    unsigned int height
)
{
    unsigned long pixel_count;
    unsigned long index;
    unsigned long drift;

    if (original == NULL || reconstructed == NULL ||
        !eyes_dimensions_ok(width, height)) {
        return (unsigned long)-1;
    }

    pixel_count = (unsigned long)width * (unsigned long)height;
    drift = 0UL;

    for (index = 0UL; index < pixel_count; ++index) {
        if (memcmp(
                original + index * 4UL,
                reconstructed + index * 4UL,
                4UL
            ) != 0) {
            ++drift;
        }
    }

    return drift;
}
