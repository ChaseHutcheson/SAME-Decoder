/*
 * -------------------------------------------------------------
 *  c-wav-player: Simple WAV File Parser and Win32 Sound Player
 *  Author: Myson Dio (gtRZync)  
 *  License: MIT License
 *  Repository: https://github.com/gtRZync/c-wav-player
 *  
 *  Description:
 *    Lightweight parser for standard RIFF/WAV files and a sound
 *    playback system using Windows waveOut API. Skips metadata
 *    chunks (e.g. LIST, smpl), reads PCM audio data, and plays it.
 *
 *  See README.md for usage and LICENSE for distribution terms.
 * -------------------------------------------------------------
 */

#pragma once
#include <stdint.h> 
#include <stdbool.h> 

/* Type used to represent a basic RIFF chunk. */
typedef struct chunk_header_t {
    char chunk_id[4];
    uint32_t chunk_size;
} chunk_header_t;

/* Type used to represent a WAVE header. */
typedef struct wav_header_t {
    char RIFF[5];
    uint32_t file_size;
    char WAVE[5];
    char fmt[5];
    uint32_t chunk_size;
    uint16_t format_type;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char data[5];
    uint32_t data_size;
} wav_header_t;

/* Type used to represent a structured WAVE file. */
typedef struct wav_file_t
{
    wav_header_t header;
    uint8_t* data;
    uint32_t data_length;
    uint32_t samples;
}wav_file_t;

/* Parse WAVE file and return pointer */
wav_file_t* parse_wav_file(const char* path);

/* Prints the WAVE file header to the stdout */
void print_wav_header(wav_header_t* header);