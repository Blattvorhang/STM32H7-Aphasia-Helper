#ifndef _MUSIC_H
#define _MUSIC_H

#include "main.h"


#define BUZZER_SET    BUZZER_GPIO_Port->BSRR = BUZZER_Pin
#define BUZZER_RESET  BUZZER_GPIO_Port->BSRR = (uint32_t)BUZZER_Pin << 16

#define SONG_MAXLEN 200  // 一首歌的最大长度
#define DURATION_UNIT 100  // 音符持续时间单位，100ms

// 音符，值为频率，计算时类型转换为uint32_t
typedef enum {
    TONE_STOP          = -1,
    TONE_SILENT        = 0,
    TONE_LOW_DO        = 131,
    TONE_LOW_RE        = 147,
    TONE_LOW_MI        = 165,
    TONE_LOW_FA        = 175,
    TONE_LOW_SOL       = 196,
    TONE_LOW_LA        = 220,
    TONE_LOW_TI        = 247,
    TONE_DO            = 262,
    TONE_RE            = 294,
    TONE_MI            = 330,
    TONE_FA            = 349,
    TONE_SOL           = 392,
    TONE_LA            = 440,
    TONE_TI_FLAT       = 466,
    TONE_TI            = 494,
    TONE_HIGH_DO       = 523,
    TONE_HIGH_RE       = 587,
    TONE_HIGH_MI_FLAT  = 622,
    TONE_HIGH_MI       = 659,
    TONE_HIGH_FA       = 698,
    TONE_HIGH_SOL      = 784,
    TONE_HIGH_LA       = 880,
    TONE_HIGH_TI       = 988
} Tone;

typedef struct {
    Tone tone;
    int16_t duration;  // 音符持续时间，单位为100ms
} Note;

typedef enum {
    TWINKLE_TWINKLE = 0,
    LORD_OF_CINDER,
    CITY_OF_TEARS
} Song;

extern Note tone[][SONG_MAXLEN];

void SoundNote(Note note);
void PlaySong(uint8_t song_index);

#endif
