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


#include "server.h"

#include <stdlib.h>

#include "lwan.h"
#include "recognize.h"
#include "util.h"


static lwan_http_status_t default_route(lwan_request_t *request, lwan_response_t *response, void *data) {
    static const char message[] = "Hello, World!";
    response->mime_type = "text/plain";
    strbuf_set_static(response->buffer, message, sizeof(message) - 1);

    return HTTP_OK;
}

static lwan_http_status_t recognize(lwan_request_t *request, lwan_response_t *response, void *data) {
    static const char NONE[] = "None";
    if (!request->header.body) {
        return HTTP_OK;
    }
    const char *message = NULL;
    recognize_from_buffer(request->header.body->value, request->header.body->len, &message);
    if (message == NULL) {
        response->mime_type = "text/plain";
        strbuf_set_static(response->buffer, NONE, sizeof(NONE) - 1);
        return HTTP_OK;
    }
    response->mime_type = "text/plain";
    strbuf_set_static(response->buffer, message, strlen(message));
    return HTTP_OK;
}

static lwan_config_t get_default_config() {
    lwan_config_t c;
    const lwan_config_t* default_config = lwan_get_default_config();
    memcpy(&c, default_config, sizeof(*default_config));
    return c;
}

void server_start(const char *host, int16_t port) {
    // Disable sphinxbase logging (logs to stderr by default)
    err_set_logfp(NULL);

    recognizer_create();

    const lwan_url_map_t default_map[] = {
        { .prefix = "/", .handler = default_route },
        { .prefix = "/recognize", .handler = recognize },
        { .prefix = NULL }
    };
    lwan_t l;
    lwan_config_t c = get_default_config();

    c.max_post_data_size = 8 * 1024 * 1024;
    char *address = malloc(sizeof(char)*1024);
    snprintf(address, sizeof(address), "%s:%d", host, port);
    c.listener = address;

    lwan_init_with_config(&l, &c);

    lwan_set_url_map(&l, default_map);
    lwan_main_loop(&l);

    lwan_shutdown(&l);

    recognizer_destroy();
}
