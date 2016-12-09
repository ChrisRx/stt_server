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

#ifndef __STT_SERVER_RECOGNIZE_H
#define __STT_SERVER_RECOGNIZE_H


#include <stdlib.h>

#include "pocketsphinx.h"


ps_decoder_t* recognizer_create();
void recognizer_destroy();
int recognize_from_buffer(char *buffer, size_t buffer_length, const char **message);

#endif
