/* libkpnd的数据结构头文件
 * 包括一些数据结构的操作
 */

#ifndef _KP_DATA_H
#define _KP_DATA_H

#include "kp_errno.h"
#include <stdbool.h>
#include <json/json.h>

/*
 * 枚举变量
 */
enum KP_TYPE {folder,file};
enum KP_CH_TYPE {pdf,doc,wps,csv,prn,xls,et,ppt,dps,txt,rtf};
enum KP_VIEW {normal,android,iPad,iphone};

/* 快盘API接口参数链表
 * key为参数名称
 * value为参数值
 * next为下一个参数的地址
 */
typedef struct kp_node
{
	char *key;
	char *value;

	struct kp_node *next;
}KP_VALUE;

/* 快盘API接口参数链表头
 * length为当前参数个数
 * arg为参数链表
 */
typedef struct
{
	int length;
	KP_VALUE *arg;
}KP_ARG;

/* 快盘API数据结构
 * key为用户创建应用时的consumer_key
 * secret为用户创建应用时的consumer_secret
 * oauth_token为临时token或者授权后的token
 * oauth_secret为授权后的oauth_secret
 * errmsg为返回的错误字符串
 */
typedef struct
{
	char *key;
	char *secret;
	char *oauth_token;
	char *oauth_secret;
	char *errmsg;
}KP;

/* 用户信息数据结构
 * user_id:标识用户的唯一ID
 * user_name:用户名字
 * max_file_size:允许上传最大文件
 * quota_total:用户空间总量
 * quota_used:用户空间已使用量
 * quota_recycled:用户空间的回收站空间使用量
 */
typedef struct
{
	uint32_t user_id;
	char *user_name;
	uint32_t max_file_size;
	uint64_t quota_total;
	uint64_t quota_used;
	uint64_t quota_recycled;
}KP_USER_INFO;

/* 文件信息
 * sha1:文件哈希值
 * share_id:共享ID
 * file_id:文件唯一标识id
 * type:folder为文件夹，file为文件。
 * size:文件大小
 * create_time:YYYY-MM-DD hh:mm:ss
 * modify_time:YYYY-MM-DD hh:mm:ss
 * name:文件名
 * is_deleted:是否被删除的文件
 * next:下一个文件（夹）信息
 */
typedef struct kp_file_node 
{
	char *sha1;
	char *share_id;
	char *file_id;
	enum KP_TYPE type;
//	enum TYPE {folder,file}type;
	uint32_t size;
	char *create_time;
	char *modify_time;
	char *name;
	char *rev;
	bool is_deleted;

	struct kp_file_node *next;
}KP_FILE_NODE;

/* 文件（夹）信息列表
 * path:文件或文件夹相对<root>的路径
 * root:kuaipan或app_folder
 * hash:list=true才返回,当前这级文件夹的哈希值。
 * file_id:path=/,root=kuaipan时不返回。文件唯一标识id。
 * type:path=/,root=kuaipan时不返回。folder为文件夹，file为文件。
 * size:path=/,root=kuaipan时不返回。文件大小。
 * create_time:path=/,root=kuaipan时不返回。YYYY-MM-DD hh:mm:ss。
 * modify_time:path=/,root=kuaipan时不返回。YYYY-MM-DD hh:mm:ss。
 * name:path=/，root=kuaipan时不返回。文件名。
 * rev:path=/,root=kuaipan时不返回。
 * is_deleted:path=/，root=kuaipan时不返回。是否被删除的文件。
 * files:文件信息
 */

typedef struct kp_file_info
{
	char *path;
	char *root;
	char hash[33];
//	char *hash;
	char *file_id;
	enum KP_TYPE type;
//	enum TYPE {folder,file}type;
	uint32_t size;
	char *create_time;
	char *modify_time;
	char *name;
	char *rev;
	bool is_deleted;

	KP_FILE_NODE *files;
}KP_FILE_INFO;

/* 文件分享信息
 * url:外链下载页面
 * access_code:提取码
 */
typedef struct
{
	char *url;
	char *access_code;
}KP_FILE_SHARE;

/* 文件历史版本
 * file_id:文件id 
 * rev:文件版本
 * create_time:YYYY-MM-DD hh:mm:ss该版本被覆盖的时间。
 * next:下一个版本
 */
typedef struct kp_file_his
{
	char *file_id;
	char *rev;
	char *create_time;

	struct kp_file_his *next;
}KP_FILE_HIS;

/* 复制引用数据结构
 * copy_ref 复制引用，可以作为copy接口的参数
 * expires YYYY-MM-DD hh:mm:ss引用过期时间
 */
typedef struct
{
	char *copy_ref;
	char *expires;
}KP_REF;

/* 初始化KP
 * key为consumer_key
 * secret为consumer_secret
 * oauth_token为oauth_token
 * oauth_secret为oauth_secret
 * 后两个参数在没有验证时可以设置为NULL
 */
KP *kp_init(char *key,char *secret,char *oauth_token,char *oauth_secret);

//释放内存
void kp_free(KP *kp);

//释放用户信息
void kp_user_info_free(KP_USER_INFO *user);

//释放文件（夹）内存占用
void kp_file_info_free(KP_FILE_INFO *file);

//释放外链信息
void kp_file_share_free(KP_FILE_SHARE *file);

//释放文件历史版本占用内存
void kp_file_his_free(KP_FILE_HIS *file);

//释放复制引用战胜内存
void kp_ref_free(KP_REF *ref);

//初始化参数链表
KP_ARG *kp_arg_init(void);

/* 添加一个参数
 * arg为使用kp_arg_init返回的指针
 * key为参数名称
 * value为参数的值
 * 成功时返回true
 * 失败时返回false
 */
bool kp_arg_add(KP_ARG *arg,char *key,char *value);

//销毁一个参数列表
void kp_arg_destroy(KP_ARG *arg);

//清空一个参数列表
void kp_arg_cleanup(KP_ARG *arg);

//以字典顺序将参数列表进行排序
void kp_arg_sort(KP_ARG *arg);

//得到参数个数
int kp_arg_length(KP_ARG *arg);

//判断列表中是否有一个参数
bool kp_arg_in(KP_ARG *arg,char *key);

//删除一个参数
void kp_arg_remove(KP_ARG *arg,char *key);

/* 更新一个参数
 * key为要更新的参数
 * value为更新的值
 * 如果成功则返回true
 * 失败返回false
 * 如果参数链表中没有要更新的参数则更新失败
 */
bool kp_arg_update(KP_ARG *arg,char *key,char *value);

//判断参数链表是否为空
bool kp_arg_empty(KP_ARG *arg);

//得到参数 
char *kp_arg_get_url(KP_VALUE *head);

#endif
