#include "delay.h"
#include "music.h"


#define SILENT        TONE_SILENT
#define LOW_DO        TONE_LOW_DO
#define LOW_RE        TONE_LOW_RE
#define LOW_MI        TONE_LOW_MI
#define LOW_FA        TONE_LOW_FA
#define LOW_SOL       TONE_LOW_SOL
#define LOW_LA        TONE_LOW_LA
#define LOW_TI        TONE_LOW_TI
#define DO            TONE_DO
#define RE            TONE_RE
#define MI            TONE_MI
#define FA            TONE_FA
#define SOL           TONE_SOL
#define LA            TONE_LA
#define TI_FLAT       TONE_TI_FLAT
#define TI            TONE_TI
#define HIGH_DO       TONE_HIGH_DO
#define HIGH_RE       TONE_HIGH_RE
#define HIGH_MI_FLAT  TONE_HIGH_MI_FLAT
#define HIGH_MI       TONE_HIGH_MI
#define HIGH_FA       TONE_HIGH_FA
#define HIGH_SOL      TONE_HIGH_SOL
#define HIGH_LA       TONE_HIGH_LA
#define HIGH_TI       TONE_HIGH_TI


Note song[][SONG_MAXLEN] = {{
        // 小星星
        {HIGH_DO,  4}, {HIGH_DO,  4}, {HIGH_SOL, 4}, {HIGH_SOL, 4}, {HIGH_LA, 4}, {HIGH_LA, 4}, {HIGH_SOL, 6}, {SILENT, 4},
        {HIGH_FA,  4}, {HIGH_FA,  4}, {HIGH_MI,  4}, {HIGH_MI,  4}, {HIGH_RE, 4}, {HIGH_RE, 4}, {HIGH_DO,  8}, {SILENT, 4},
        {HIGH_SOL, 4}, {HIGH_SOL, 4}, {HIGH_FA,  4}, {HIGH_FA,  4}, {HIGH_MI, 4}, {HIGH_MI, 4}, {HIGH_RE,  6}, {SILENT, 4},
        {HIGH_SOL, 4}, {HIGH_SOL, 4}, {HIGH_FA,  4}, {HIGH_FA,  4}, {HIGH_MI, 4}, {HIGH_MI, 4}, {HIGH_RE,  6}, {SILENT, 4},
        {HIGH_DO,  4}, {HIGH_DO,  4}, {HIGH_SOL, 4}, {HIGH_SOL, 4}, {HIGH_LA, 4}, {HIGH_LA, 4}, {HIGH_SOL, 6}, {SILENT, 4},
        {HIGH_FA,  4}, {HIGH_FA,  4}, {HIGH_MI,  4}, {HIGH_MI,  4}, {HIGH_RE, 4}, {HIGH_RE, 4}, {HIGH_DO,  6}, {SILENT, 4},
        {TONE_STOP, -1} // 停止标记
    }, {
        // Lord of Cinder
        {HIGH_MI, 2}, {HIGH_RE, 2}, {LA,       4}, {SILENT, 2},
        {HIGH_MI, 2}, {HIGH_RE, 2}, {HIGH_SOL, 4}, {HIGH_FA, 2}, {HIGH_MI, 4}, {SILENT, 2},
        {TONE_STOP, -1} // 停止标记
    }, {
        // City of Tears
        {HIGH_RE, 8}, {HIGH_DO, 4}, {TI_FLAT, 4}, {HIGH_MI_FLAT, 8}, {HIGH_RE, 8},
        {HIGH_RE, 8}, {HIGH_DO, 4}, {TI_FLAT, 4}, {HIGH_FA,      8}, {HIGH_RE, 8},
        {HIGH_RE, 8}, {HIGH_DO, 4}, {TI_FLAT, 4}, {HIGH_MI_FLAT, 8}, {HIGH_RE, 8},
        {HIGH_RE, 8}, {HIGH_DO, 4}, {TI_FLAT, 4}, {HIGH_FA,      8}, {SILENT, 8},
        {TONE_STOP, -1} // 停止标记
    }
};

/* 响一个音符 */
void SoundNote(Note note)
{
    uint16_t i;
    uint32_t half_period_us = 500000 / (uint32_t)(note.tone);
    // freq = note.tone, freq * period = 1
    uint32_t impulse_num = (uint32_t)(note.tone) * (uint32_t)(note.duration) * DURATION_UNIT / 1000;
    for (i = 0; i < impulse_num; i++) {
        // tone为频率，period_us为周期
        if (tone != TONE_SILENT) {  // 频率为0时不发音
            BUZZER_RESET;  // BUZZER引脚拉低，响
            delay_us(half_period_us);
            BUZZER_SET;    // BUZZER引脚拉高，不响
            delay_us(half_period_us);
        }
        else
            delay_us(half_period_us * 2);
    }
}

/* 播放一首歌 */
void PlaySong(uint8_t song_index)
{
    uint16_t note_index = 0;
    while(song[song_index][note_index].tone != TONE_STOP) {
        SoundNote(song[song_index][note_index]);  // 播放一个音符
        delay_ms(300);
        note_index++;
    }
}
