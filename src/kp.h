/* 金山快盘API C语言SDK
 * Version:1.0
 * 作者:炕头哥(Brisk)
 * 邮件地址:briskgreen@163.com
 */
#ifndef _KP_H
#define _KP_H

#include "kp_oauth.h"
#include <unistd.h>
#include <curl/curl.h>

/*
 * libcurl进度回调函数
 * 函数原型
 * int (*curl_progress_callback)(void *data,double dltotal,double dlnow,double ultotal,double ulnow)
 * data为传入的参数
 * dltotal为需要下载总数
 * dlnow为当前下载数量
 * ultotal为上传总数
 * ulnow为当前上传数量
 */
typedef curl_progress_callback kp_progress;

/* 
 * 得到用户信息
 * 发生错误时返回错误代码
 * 结果保存在user中
 * 该函数会为user参数开辟内存，所以调用完后注意使用kp_user_info_free进行释放
 */
bool kp_get_user_info(KP *kp,KP_ARG *arg,KP_USER_INFO **user);

/*
 * 获取文件（夹）信息
 * 会在arg列表中添加path和root参数（当使用其它函数时注意删除不必要参数）
 * 函数会为file开辟内存空间，使用完毕后注意使用kp_file_info_free进行释放
 */
bool kp_get_file_info(KP *kp,KP_ARG *arg,
		char *root,char *path,KP_FILE_INFO **file);

/*
 * 获取文件分享链接
 * 函数会向arg参数列表中添加root和path参数
 * 函数会为file开辟内存，注意使用kp_file_share_free进行释放
 */
bool kp_get_file_share(KP *kp,KP_ARG *arg,
		char *root,char *path,KP_FILE_SHARE **file);

/*
 * 获取文件的历史版本
 * 函数会向arg列表中添加root和path参数
 * 函数会为his开辟内存，注意使用kp_file_his_free进行释放
 */
bool kp_get_file_history(KP *kp,KP_ARG *arg,
		char *root,char *path,KP_FILE_HIS **his);

/* 创建文件夹
 * root app_folder或者kuaipan
 * path 相对于root的路径，包含文件名
 * 函数会添加root,path参数
 */
bool kp_create_file(KP *kp,KP_ARG *arg,char *root,char *path);

/*
 * 删除文件（夹）
 * 函数会添加root和path参数
 */
bool kp_delete_file(KP *kp,KP_ARG *arg,char *root,char *path);

/*
 * 移动文件（夹）
 * 函数会添加root、path、from_path、to_path参数
 */
bool kp_remove_file(KP *kp,KP_ARG *arg,
		char *root,char *from_path,char *to_path);

/*
 * 复制文件（夹）
 * 函数会添加root、path、from_path、to_path参数
 */
bool kp_copy_file(KP *kp,KP_ARG *arg,
		char *root,char *from_path,char *to_path);

/*
 * 复制引用
 * 函数会添加root、path参数 
 * 函数会为ref开辟内存，使用完毕后注意使用kp_ref_free进行释放
 */
bool kp_copy_ref(KP *kp,KP_ARG *arg,KP_REF **ref,char *root,char *path);

//获取上传文件的地址
char *kp_get_upload_url(KP *kp,KP_ARG *arg);

/* 上传文件
 * func为进度函数
 * data为func函数的参数
 * 参数会添加root、path、overwrite参数
 */
bool kp_upload_file(KP *kp,KP_ARG *arg,char *root,char *path,
		char *filename,bool overwrite,
		kp_progress func,void *data);

/*
 * 下载文件
 * 函数会添加root、path参数
 */
bool kp_download_file(KP *kp,KP_ARG *arg,char *root,char *path,
		char *filename,kp_progress func,void *data);

/* 
 * 获取缩略图
 * width 宽度
 * height 高度
 * filename 保存的文件名
 * 函数会添加root、path、width、height参数
 */
bool kp_get_thumbnail(KP *kp,KP_ARG *arg,int width,int height,
		char *root,char *path,char *filename);

/*
 * 文档转换
 * type 文档类型，参数如下:
 * pdf、doc、wps、csv、prn、xls、et、ppt、dps、txt、rtf
 * view 视图格式，参数可取normal、android、iPad、iphone
 * filename 文件的保存路径
 * 函数会添加type、view、root、path参数
 */
bool kp_doc_change(KP *kp,KP_ARG *arg,enum KP_CH_TYPE type,
		enum KP_VIEW view,char *root,char *path,
		char *filename);

#endif
