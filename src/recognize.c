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


#include "recognize.h"

#include <stdio.h>

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include "pocketsphinx.h"



static ps_decoder_t *ps = NULL;
static cmd_ln_t *config = NULL;

ps_decoder_t* recognizer_create() {
    config = cmd_ln_init(NULL, ps_args(), TRUE, NULL);
    ps_default_search_args(config);
    ps = ps_init(config);
    return ps;
}

void recognizer_destroy() {
    if (config != NULL) {
        cmd_ln_free_r(config);
    }
    if (ps != NULL) {
        ps_free(ps);
    }
}

int recognize_from_buffer(char *buffer, size_t buffer_length, char **message) {
    ps_start_utt(ps);
    ps_process_raw(ps, (int16*)buffer, buffer_length/sizeof(int16), FALSE, FALSE);
    uint8 in_speech = ps_get_in_speech(ps);
    const char* hyp = ps_get_hyp(ps, NULL);
    if (hyp != NULL) {
        *message = (char*)hyp;
    }
    ps_end_utt(ps);
    return 0;
}
