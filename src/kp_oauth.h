/* 金山快盘API授权认证
 * OAuth1.0
 */
#ifndef _KP_OAUTH_H
#define _KP_OAUTH_H

#include "kp_data.h"
#include <oauth.h>

/* 初始化oauth参数并得到一个KP_ARG参数列表
 */
KP_ARG *kp_oauth_arg_init(KP *kp);

/* 更新时间戳*/
bool kp_oauth_update_timestamp(KP_ARG *arg);

/* 更新nonce*/
bool kp_oauth_update_nonce(KP_ARG *arg);

/* 更新签名参数*/
void kp_oauth_update_signature(KP_ARG *arg,char *key);

/* 更新oauth_token*/
void kp_oauth_update_token(KP_ARG *arg,char *token);

/* 得到签名字符串
 * kp为KP数据结构
 * http_method为HTTP方式如GET、POST
 * url为要访问的URL
 * arg为要使用的参数
 */
char *kp_get_oauth_key(KP *kp,char *http_method,char *url,KP_ARG *arg);

//得到授权网页地址
char *kp_get_authorize_url(KP *kp,KP_ARG *arg);

//得到accessToken
bool kp_get_access_token(KP *kp,KP_ARG *arg);

#endif
