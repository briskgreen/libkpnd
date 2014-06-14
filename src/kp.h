/* 金山快盘API C语言SDK
 * Version:1.0
 * 作者:炕头哥(Brisk)
 * 邮件地址:briskgreen@163.com
 */
#ifndef _KP_H
#define _KP_H

#include "kp_oauth.h"
#include <curl/curl.h>

typedef kp_progress curl_progress_callback;

/* 得到用户信息
 * 发生错误时返回错误代码
 * 结果保存在user中
 */
int kp_get_user_info(KP *kp,KP_ARG *arg,KP_USER_INFO *user);

//获取文件（夹）信息
int kp_get_file_info(KP *kp,KP_ARG *arg,char *root,char *path,KP_FILE_INFO *file);

//获取文件分享链接
int kp_get_file_share(KP *kp,KP_ARG *arg,char *root,char *path,KP_FILE_SHARE *file);

//获取文件的历史版本
int kp_get_file_histroy(KP *kp,KP_ARG *arg,char *root,char *path,KP_FILE_HIS *his);

//创建文件夹
int kp_create_file(KP *kp,KP_ARG *arg);

//删除文件（夹）
int kp_delete_file(KP *kp,KP_ARG *arg);

//移动文件（夹）
int kp_remove_file(KP *kp,KP_ARG *arg);

//复制文件（夹）
int kp_copy_file(KP *kp,KP_ARG *arg);

//复制引用
int kp_copy_ref(KP *kp,KP_ARG *arg,char *root,char *path);

/* 上传文件
 * func为进度函数
 * data为func函数的参数
 */
int kp_upload_file(KP *kp,KP_ARG *arg,char *filename,kp_progress func,void *data);

//下载文件
int kp_download_file(KP *kp,KP_ARG *arg,char *filename,kp_progress func,void *data);

//获取缩略图
int kp_get_thumbnail(KP *kp,KP_ARG *arg,char *filename);

//文档转换
int kp_doc_change(KP *kp,KP_ARG *arg,char *filename);

#endif
