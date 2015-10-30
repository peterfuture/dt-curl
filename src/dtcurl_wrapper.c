/*
 * =====================================================================================
 *
 *    Filename   :  dtcurl_wrapper.c
 *    Description:
 *    Version    :  1.0
 *    Created    :  2015��10��30�� 10ʱ43��06��
 *    Revision   :  none
 *    Compiler   :  gcc
 *    Author     :  peter-s
 *    Email      :  peter_future@outlook.com
 *    Company    :  dt
 *
 * =====================================================================================
 */

#include "dtcurl_wrapper.h"
#include "dtcurl_log.h"
#include "dtcurl_macro.h"

int dtcurl_wrapper_init(dtcurl_wrapper_t *wrapper, const char *uri)
{
    memset(wrapper, 0, sizeof(dtcurl_wrapper_t));
    if (!wrapper->curl_handle) {
        wrapper->curl_handle = curl_easy_init();
        if (wrapper->curl_handle) {
            CURL_LOG("curl easy init ok \n");
        }
    }
    return CURL_ERROR_NONE;
}
