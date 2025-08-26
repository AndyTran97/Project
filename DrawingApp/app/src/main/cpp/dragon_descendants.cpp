#include <jni.h>
#include <time.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <stdlib.h>

#define  LOG_TAG    "PIXEL MANIPULATING"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static int rgb_clamp(int value){
    if (value > 255){
        return 255;
    }
    if (value < 0){
        return 0;
    }
    return value;
}

static void invertColor(AndroidBitmapInfo* info, void* pixels){
    int xx, yy, alpha, red, green, blue;
    uint32_t* line;

    for(yy = 0; yy < info->height;yy++){
        line = (uint32_t*)pixels;
        for(xx = 0; xx < info->width; xx++){
            // extract the RGB values from the pixel
            alpha = (int)((line[xx] & 0xFF000000) >> 24);
            red = (int) ((line[xx] & 0x00FF0000) >> 16);
            green = (int)((line[xx] & 0x0000FF00) >> 8);
            blue = (int) (line[xx] & 0x00000FF );

            // Manipulate each value
            red = rgb_clamp((int)(red ^ 0xFF));
            green = rgb_clamp((int)(green ^ 0xFF));
            blue = rgb_clamp((int)(blue ^ 0xFF));

            // set the pixel value back
            line[xx] = ((alpha << 24) & 0xFF000000) |
                    ((red << 16) & 0x00FF0000) |
                    ((green << 8) & 0x0000FF00) |
                    (blue & 0x000000FF);

        }
        pixels = (char*)pixels + info->stride;
    }
}
static void addNoise(AndroidBitmapInfo* info, void* pixels){
    int xx, yy, alpha, red, green, blue;
    uint32_t* line;

    int mean = 0, stddev = 25;


    // Seed the random number generator
    srand(time(NULL));

    for(yy = 0; yy < info->height;yy++){
        line = (uint32_t*)pixels;
        for(xx = 0; xx < info->width; xx++){
            // extract the RGB values from the pixel
            alpha = (int)((line[xx] & 0xFF000000) >> 24);
            red = (int) ((line[xx] & 0x00FF0000) >> 16);
            green = (int)((line[xx] & 0x0000FF00) >> 8);
            blue = (int) (line[xx] & 0x00000FF );


            // Add noise to each color channel
            float noiseRed = ((float)rand() / RAND_MAX) * stddev + mean;
            float noiseGreen = ((float)rand() / RAND_MAX) * stddev + mean;
            float noiseBlue = ((float)rand() / RAND_MAX) * stddev + mean;

            red += (int)round(noiseRed);
            green += (int)round(noiseGreen);
            blue += (int)round(noiseBlue);

            red = rgb_clamp(red);
            green = rgb_clamp(green);
            blue = rgb_clamp(blue);

            // set the pixel value back
            line[xx] = ((alpha << 24) & 0xFF000000) |
                       ((red << 16) & 0x00FF0000) |
                       ((green << 8) & 0x0000FF00) |
                       (blue & 0x000000FF);

        }
        pixels = (char*)pixels + info->stride;
    }


}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_dragon_1descendants_NativeDrawingUtils_invertColor(JNIEnv *env, jobject obj,
                                                                    jobject bitmap) {
    AndroidBitmapInfo  info;
    int ret;
    void* pixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }


    invertColor(&info, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_dragon_1descendants_NativeDrawingUtils_addNoise(JNIEnv *env, jobject thiz,
                                                                 jobject bitmap) {
    AndroidBitmapInfo  info;
    int ret;
    void* pixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }


    addNoise(&info, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
}