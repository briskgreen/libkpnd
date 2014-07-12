/*
 * 打印错误消息头头文件
 */
#ifndef _KP_ERRNO_H
#define _KP_ERRNO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//错误代码全局变量
int kp_errno;

/*
 * 错误代码
 */
#define KP_ERROR_ACCESS_TOKEN -1           //获取Access Token出错
#define KP_ERROR_ARG -2                    //创建参数出错
#define KP_ERROR_DOC_CHANGE -3             //文档转换出错
#define KP_ERROR_CHANGE_TYPE -4            //错误的文档类型
#define KP_ERROR_CHANGE_VIEW -5            //错误的视图格式
#define KP_ERROR_COPY_FILE -6              //复制文件出错
#define KP_ERROR_COPY_REF -7               //复制文件引用出错
#define KP_ERROR_CREATE_FILE -8            //创建文件出错
#define KP_ERROR_DELETE_FILE -9            //删除文件出错
#define KP_ERROR_DOWNLOAD_FILE -10         //下载文件出错
#define KP_ERROR_FILE_ALREADY_EXISTS -11   //文件已存在
#define KP_ERROR_FILE_HISTORY -12          //获取文件历史版本出错
#define KP_ERROR_FILE_INFO -13             //获取文件信息出错
#define KP_ERROR_FILE_SHARE -14            //获取文件共享链接出错
#define KP_ERROR_KEY -15                   //获取key出错
#define KP_ERROR_NET_GET -16               //HTTP GET请求出错
#define KP_ERROR_NO_MEM -17                //没有足够的内存
#define KP_ERROR_NONCE -18                 //获取nonce出错
#define KP_ERROR_REMOVE_FILE -19           //移到文件出错
#define KP_ERROR_SECRET -20                //获取secret出错
#define KP_ERROR_THUMBNAIL -21             //获取缩略图出错
#define KP_ERROR_UPLOAD_FILE -22           //上传文件出错
#define KP_ERROR_UPLOAD_URL -23            //获取上传文件url出错
#define KP_ERROR_USER_INFO -24             //获取用户信息出错
#define KP_ERROR_TOKEN -25                 //获取Token出错

/*
 * 打印错误消息
 */
char *kp_error(char *msg);

//根据错误代码打印错误消息
char *kp_errnum(int errnum,char *msg);

#endif
