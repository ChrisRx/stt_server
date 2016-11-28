/*
 * Copyright 2016 Chris Marshall
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "util.h"

#include <stdio.h>
#include <stdlib.h>

#include <sphinxbase/err.h>


int check_wav_header(char *header, int expected_sr) {
    int sr;

    if (header[34] != 0x10) {
        E_ERROR("Input audio file has [%d] bits per sample instead of 16\n", header[34]);
        return 0;
    }
    if (header[20] != 0x1) {
        E_ERROR("Input audio file has compression [%d] and not required PCM\n", header[20]);
        return 0;
    }
    if (header[22] != 0x1) {
        E_ERROR("Input audio file has [%d] channels, expected single channel mono\n", header[22]);
        return 0;
    }
    sr = ((header[24] & 0xFF) | ((header[25] & 0xFF) << 8) | ((header[26] & 0xFF) << 16) | ((header[27] & 0xFF) << 24));
    if (sr != expected_sr) {
        E_ERROR("Input audio file has sample rate [%d], but decoder expects [%d]\n", sr, expected_sr);
        return 0;
    }
    return 1;
}

int file_read_all(const char *filename, char **buffer, size_t *buffer_length) {
    FILE *fp;
    if ((fp = fopen(filename, "rb")) == 0) {
        fprintf(stderr, "Failed to open file %s\n", filename);
    }
    fseek(fp, 0, SEEK_END);
    *buffer_length = ftell(fp);
    rewind(fp);
    *buffer = calloc(1, *buffer_length+1);
    if (*buffer == NULL) {
        fprintf(stderr, "No bufs\n");
        fclose(fp);
        return 1;
    }
    if (fread(*buffer, *buffer_length, 1, fp) != 1) {
        fprintf(stderr, "No read\n");
        fclose(fp);
        free(*buffer);
        return 1;
    }
    fclose(fp);
    return 0;
}
