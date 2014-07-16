/*
 * 打印错误消息头头文件
 * 各错误字符串中文解释
 * account server error			帐号服务器查询失败，请过一段时间重试
 * cannot create app folder		不能创建用户目录，请重新获取access Token
 * login fail				登陆失败，可能用户密码不对或者无些帐号
 * wrong venfication code		注册验证码错误或者失效
 * commit fail				上传失败，请过一段时间重试
 * bad openid				不能用此openid注册
 * bad parameters			参数不正确
 * bad request				请求url、参数不对
 * no such api implemented		无此API
 * bad signature			签名不正确
 * request expired			请求过期(timestamp或者nonce不正确)
 * bad consumer key			没有这个consumer key(你确定申请了应用？）
 * not supported auth mode		你不能使用这种授权模式
 * authorization expired		用户授权过期，请重新获取access_token
 * api daily limit			您今天调用接口次数太多了吧
 * no right to call this api		无权即调用此api
 * reused nonce				nonce被重用了
 * bad verifier				verifier错误 
 * authorization failed			授权失败（不是生产状态的应用）
 * file exist				同名文件（夹）已经存在
 * forbidden				无权限访问，或者无权限进行此操作
 * file not exist			文件夹不存在，父目录不存在
 * too many files			一次操作太多文件
 * file too large			文件大小超过能上传的上限
 * server error				服务器错误，应该是我们（金山快盘公司）遇到bug了
 * over space				用户空间已满
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
