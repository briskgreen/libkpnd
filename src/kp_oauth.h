/* 金山快盘API授权认证
 * OAuth1.0
 */
#ifndef _KP_OAUTH_H
#define _KP_OAUTH_H

#include "kp_data.h"
#include <oauth.h>
#include <json/json.h>

/* 得到签名字符串
 * kp为KP数据结构
 * http_method为HTTP方式如GET、POST
 * url为要访问的URL
 * arg为要使用的参数
 */
char *kp_get_oauth_key(KP *kp,char *http_method,char *url,KP_ARG *arg);

//得到授权网页地址
char *kp_get_authorize_url(KP *kp,KP_ARG *arg);

//进行授权
bool kp_app_authorize(KP *kp);

#endif
