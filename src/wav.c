#include "wav.h"
#include "colors.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Parse WAVE file and return pointer */
wav_file_t* parse_wav_file(const char* path) {
    printf("Parsing WAVE file...\n");

    wav_file_t *wptr = calloc(1, sizeof(wav_file_t));
    chunk_header_t index;

    FILE *fptr = fopen(path, "rb");

    // Identify 'RIFF' chunk
    fread(&index, 1, sizeof(chunk_header_t), fptr);

    if (memcmp(index.chunk_id, "RIFF", 4) != 0)
    {
        fprintf(stderr, "Error parsing WAVE file: Invalid File -> \'RIFF\' chunk missing.\n");
        return NULL;
        free(wptr);
    }
    memcpy(wptr->header.RIFF, &index.chunk_id, 4);
    memcpy(&(wptr->header.file_size), &index.chunk_size, 4);

    // Verify form type
    char form_type[4];
    fread(form_type, 1, sizeof(form_type), fptr);

    if (memcmp(form_type, "WAVE", 4) != 0)
    {
        fprintf(stderr, "Error parsing WAVE file: Invalid File -> form_type is not WAVE.\n");
        return NULL;
        free(wptr);
    }

    memcpy(wptr->header.WAVE, form_type, 4);

    // Locate 'fmt ' subchunk;
    bool fmt_found = false;
    do {
        fread(&index, 1, sizeof(chunk_header_t), fptr);
        if (memcmp(index.chunk_id, "fmt ", 4) == 0) { 
            printf("FMT Found\n");
            fmt_found = true;
            break; 
        }
        printf("Found subchunk before \'fmt \'. chunk_id = %.4s, chunk_size = %u", index.chunk_id, index.chunk_size);
        fseek(fptr, index.chunk_size + (index.chunk_size % 4), SEEK_CUR);
    } while (!fmt_found);

    // Read common fields
    unsigned short int format_tag, channels, block_align;
    unsigned int samples_per_second, avg_bytes_per_second;

    fread(&format_tag, 1, sizeof(format_tag), fptr);
    fread(&channels, 1, sizeof(channels), fptr);
    fread(&samples_per_second, 1, sizeof(samples_per_second), fptr);
    fread(&avg_bytes_per_second, 1, sizeof(avg_bytes_per_second), fptr);
    fread(&block_align, 1, sizeof(block_align), fptr);

    memcpy(wptr->header.fmt, index.chunk_id, 4);
    wptr->header.chunk_size = index.chunk_size;
    wptr->header.format_type = format_tag;
    wptr->header.num_channels = channels;
    wptr->header.sample_rate = samples_per_second;
    wptr->header.block_align = block_align;

    

    // Read format specific fields and verify 'fmt '
    switch(format_tag) {
        // PCM
        case 1: {
                unsigned short int bits_per_sample;
                fread(&bits_per_sample, 1, sizeof(bits_per_sample), fptr);

                unsigned int calculated_avg_bytes_per_second = samples_per_second * channels * (bits_per_sample / 8);
                
                if (avg_bytes_per_second != calculated_avg_bytes_per_second)
                {
                    fprintf(stderr, "Error parsing WAVE file: Invalid File -> \'avg_bytes_per_second\' (%u) doesn't equal calculated value (%u).\n", avg_bytes_per_second, calculated_avg_bytes_per_second);
                    return NULL;
                    free(wptr);
                }

                    wptr->header.byte_rate = avg_bytes_per_second;
                    wptr->header.bits_per_sample = bits_per_sample;

                break;
            }
        default:
            fprintf(stderr, "Unregistered WAVE format encountered. Format Tag: %hu", format_tag);
            return NULL;
            free(wptr);
    }

    // Locate 'data' subchunk;
    bool data_found = false;
    do {
        fread(&index, 1, sizeof(chunk_header_t), fptr);
        if (memcmp(index.chunk_id, "data", 4) == 0) { 
            printf("Data Found\n");
            data_found = true;
            break; 
        }
        printf("Found subchunk before \'data \'. chunk_id = %.4s, chunk_size = %u", index.chunk_id, index.chunk_size);
        fseek(fptr, index.chunk_size + (index.chunk_size % 4), SEEK_CUR);
    } while (!data_found);

    memcpy(wptr->header.data, index.chunk_id, 4);
    wptr->header.data_size = index.chunk_size;

    printf("Parsing WAVE file complete!\n");

    return wptr;
}

/* Prints the WAVE file header to the stdout */
void print_wav_header(wav_header_t* header) {
    printf(CYN "----- WAV Header Info -----\n" COLOR_RESET);
    
    printf(GRN "ChunkID: " COLOR_RESET "%.4s\n", header->RIFF);
    printf(GRN "File Size (minus 8 bytes): " COLOR_RESET "%d bytes\n", header->file_size);
    printf(GRN "Format: " COLOR_RESET "%.4s\n", header->WAVE);
    
    printf("\n" YEL "Format Subchunk:" COLOR_RESET "\n");
    printf("  " BLU "Subchunk1ID: " COLOR_RESET "%.4s\n", header->fmt);
    printf("  " BLU "Subchunk1 Size: " COLOR_RESET "%d bytes\n", header->chunk_size);
    
    printf("  " BLU "Audio Format: " COLOR_RESET "%d ", header->format_type);
    if (header->format_type == 1)
        printf(GRN "(PCM)\n" COLOR_RESET);
    else
        printf(RED "(Compressed/Other)\n" COLOR_RESET);
    
    printf("  " BLU "Number of Channels: " COLOR_RESET "%d ", header->num_channels);
    if (header->num_channels == 1)
        printf(GRN "(Mono)\n" COLOR_RESET);
    else if (header->num_channels == 2)
        printf(GRN "(Stereo)\n" COLOR_RESET);
    else
        printf(YEL "(Multi-channel)\n" COLOR_RESET);
    
    printf("  " BLU "Sample Rate: " COLOR_RESET "%d Hz\n", header->sample_rate);
    printf("  " BLU "Byte Rate: " COLOR_RESET "%d bytes/sec\n", header->byte_rate);
    printf("  " BLU "Block Align: " COLOR_RESET "%d bytes/frame\n", header->block_align);
    printf("  " BLU "Bits per Sample: " COLOR_RESET "%d bits\n", header->bits_per_sample);

    printf("\n" YEL "Data Subchunk:" COLOR_RESET "\n");
    printf("  " BLU "Data Header: " COLOR_RESET "%.4s\n", header->data);
    printf("  " BLU "Data Size: " COLOR_RESET "%d bytes\n", header->data_size);

    if (header->byte_rate > 0) {
        double duration_sec = (double)header->data_size / header->byte_rate;
        printf("\n" GRN "Approximate Duration: " COLOR_RESET "%.2f seconds\n", duration_sec);
    }
    
    printf(CYN "---------------------------\n" COLOR_RESET);
}
