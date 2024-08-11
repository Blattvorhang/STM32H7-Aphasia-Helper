#include <stdio.h>
#include <string.h>
#include "ai_platform.h"
#include "class_mapping.h"
#include "network.h"
#include "pic.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "SYN6288.h"

#define WIDTH 240
#define HEIGHT 240
#define NEW_WIDTH 160
#define NEW_HEIGHT 160

extern uint16_t screen_buffer[WIDTH][HEIGHT];

void speak_class_name(int32_t class_idx);

void RGB565_to_RGB888(uint16_t rgb565, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = (rgb565 >> 11) & 0x1F;
    *g = (rgb565 >> 5) & 0x3F;
    *b = rgb565 & 0x1F;

    // Expand the 5-bit color components to 8-bit
    *r = (*r << 3) | (*r >> 2);
    *g = (*g << 2) | (*g >> 4);
    *b = (*b << 3) | (*b >> 2);
}

void RGB888_to_RGB565(uint8_t r, uint8_t g, uint8_t b, uint16_t *rgb565)
{
    *rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

void crop(uint8_t *src, uint8_t *dst, int src_width, int src_height, int dst_width, int dst_height)
{
    if (src_width < dst_width || src_height < dst_height)
    {
        return;
    }

    int x_offset = (src_width - dst_width) / 2;
    int y_offset = (src_height - dst_height) / 2;

    for (int i = 0; i < dst_height; i++)
    {
        for (int j = 0; j < dst_width; j++)
        {
            dst[(i * dst_width + j) * 3 + 0] = src[((i + y_offset) * src_width + (j + x_offset)) * 3 + 0];
            dst[(i * dst_width + j) * 3 + 1] = src[((i + y_offset) * src_width + (j + x_offset)) * 3 + 1];
            dst[(i * dst_width + j) * 3 + 2] = src[((i + y_offset) * src_width + (j + x_offset)) * 3 + 2];
        }
    }
}

void resize(uint8_t *src, uint8_t *dst, int src_width, int src_height, int dst_width, int dst_height)
{
    int x_ratio = (int)((src_width << 16) / dst_width) + 1;
    int y_ratio = (int)((src_height << 16) / dst_height) + 1;

    for (int i = 0; i < dst_height; i++)
    {
        for (int j = 0; j < dst_width; j++)
        {
            int x = ((j * x_ratio) >> 16);
            int y = ((i * y_ratio) >> 16);
            dst[(i * dst_width + j) * 3 + 0] = src[(y * src_width + x) * 3 + 0];
            dst[(i * dst_width + j) * 3 + 1] = src[(y * src_width + x) * 3 + 1];
            dst[(i * dst_width + j) * 3 + 2] = src[(y * src_width + x) * 3 + 2];
        }
    }
}

void normalize_and_convert_to_int8(uint8_t *rgb888, int8_t *int8_data, int width, int height)
{
    for (int i = 0; i < width * height * 3; i++)
    {
        int8_data[i] = (int8_t)((int16_t)rgb888[i] - 128);
    }
}

void copy_data(uint8_t *src, uint8_t *dst, int size)
{
    for (int i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
}

/* Read an image with size WIDTH x HEIGHT,
 * crop and resize it to NEW_WIDTH x NEW_HEIGHT (160 x 160)
 */
void acquire_and_process_data(int8_t *data)
{
    // Use static variables to avoid allocating memory on the stack
    // Read the RGB565 format image data
    static uint16_t *rgb565_data = (uint16_t *)screen_buffer; // WIDTH * HEIGHT;
    static uint8_t src_img[WIDTH * HEIGHT * 3];
    static uint8_t dst_img[NEW_WIDTH * NEW_HEIGHT * 3];

    uint32_t i;

    // Convert RGB565 to RGB888
    for (i = 0; i < WIDTH * HEIGHT; i++)
    {
        RGB565_to_RGB888(rgb565_data[i], &src_img[i * 3 + 0], &src_img[i * 3 + 1], &src_img[i * 3 + 2]);
    }

    // const int min_size = WIDTH < HEIGHT ? WIDTH : HEIGHT;
    // crop(src_img, dst_img, WIDTH, HEIGHT, min_size, min_size);

    // copy_data(dst_img, src_img, min_size * min_size * 3);

    resize(src_img, dst_img, WIDTH, HEIGHT, NEW_WIDTH, NEW_HEIGHT);

    // Convert RGB888 to int8 format
    normalize_and_convert_to_int8(dst_img, data, NEW_WIDTH, NEW_HEIGHT);
}

void post_process(const int8_t *data)
{
    // Find the index of the maximum value, which corresponds to the predicted class
    int32_t max = INT32_MIN;
    int32_t max_index = 0;
    static char class_str[50];

    for (int i = 0; i < AI_NETWORK_OUT_1_SIZE_BYTES; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
            max_index = i;
        }
    }
    printf("Prediction: %d %s\n", max_index, class_name[max_index]);

    sprintf(class_str, "%d %s", max_index, class_name[max_index]);
    Show_Str(45, 17, WHITE, DARKBLUE, (u8 *)class_str, 16, 1);
		
    // LCD_ShowNum(15, 20, max_index, 5, 12);
    // LCD_ShowString(50, 15, 16, (u8 *)class_name[max_index], 1);
		
    speak_class_name(max_index);
}

void speak_class_name(int32_t class_idx)
{
    char text[100];
    char class_name[100];
    switch (class_idx)
    {
        case 508:
            strcpy(class_name, "¼üÅÌ");
            break;
        case 607:
        case 948:
        case 950:
        case 722:
            strcpy(class_name, "éÙ×Ó");
            break;
        case 954:
            strcpy(class_name, "Ïã½¶");
            break;
        case 999:
            strcpy(class_name, "Ö½½í");
            break;
        case 440:
        case 441:
        case 455:
        case 737:
        case 898:
        case 907:
            strcpy(class_name, "Æ¿×Ó");
            break;
        default:  // Other classes are not supported
            return;
    }
    sprintf(text, "ÎÒÒª%s", class_name);

    uint8_t repeat_time = 3;
    for (uint8_t i = 0; i < repeat_time; i++)
        SYN_Speak(text);
}
