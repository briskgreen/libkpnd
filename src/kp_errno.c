#include "kp_errno.h"

char *kp_error(char *msg)
{
	char *res=NULL;
	char *error_msg;
	int len=0;

	switch(kp_error)
	{
		case KP_ERROR_ACCESS_TOKEN:
			error_msg="获取Access Token出错!\n";
			break;
		case KP_ERROR_ARG:
			error_msg="创建参数出错!\n";
			break;
		case KP_ERROR_DOC_CHANGE:
			error_msg="文档转换出错!\n";
			break;
		case KP_ERROR_CHANGE_TYPE:
			error_msg="错误的文档类型!\n";
			break;
		case KP_ERROR_CHANGE_VIEW:
			error_msg="错误的视图格式!\n";
			break;
		case KP_ERROR_COPY_FILE:
			error_msg="复制文件出错!\n";
			break;
		case KP_ERROR_COPY_REF:
			error_msg="复制文件引用出错!\n";
			break;
		case KP_ERROR_CREATE_FILE:
			error_msg="创建文件出错!\n";
			break;
		case KP_ERROR_DELETE_FILE:
			error_msg="删除文件出错!\n";
			break;
		case KP_ERROR_DOWNLOAD_FILE:
			error_msg="下载文件出错!\n";
			break;
		case KP_ERROR_FILE_ALREADY_EXISTS:
			error_msg="文件已存在!\n";
			break;
		case KP_ERROR_FILE_HISTORY:
			error_msg="获取文件历史版本出错!\n";
			break;
		case KP_ERROR_FILE_INFO:
			error_msg="获取文件信息出错!\n";
			break;
		case KP_ERROR_FILE_SHARE:
			error_msg="获取文件共享链接出错!\n";
			break;
		case KP_ERROR_KEY:
			error_msg="获取key出错!\n";
			break;
		case KP_ERROR_NET_GET:
			error_msg="HTTP GET请求出错!\n";
			break;
		case KP_ERROR_NO_MEM:
			error_msg="没有足够的内存!\n";
			break;
		case KP_ERROR_NONCE:
			error_msg="获取nonce出错!\n";
			break;
		case KP_ERROR_REMOVE_FILE:
			error_msg="移动文件出错!\n";
			break;
		case KP_ERROR_SECRET:
			error_msg="获取secret出错!\n";
			break;
		case KP_ERROR_THUMBNAIL:
			error_msg="获取缩略图出错!\n";
			break;
		case KP_ERROR_UPLOAD_FILE:
			error_msg="上传文件出错!\n";
			break;
		case KP_ERROR_UPLOAD_URL:
			error_msg="获取上传文件的url出错!\n";
			break;
		case KP_ERROR_USER_INFO:
			error_msg="获取用户信息出错!\n";
			break;

		default:
			error_msg="未知道错误!\n";

	}

	if(msg)
	{
		len=strlen(msg)+strlen(error_msg)+2;
		res=malloc(sizeof(char)*len);
		snprintf(res,sizeof(char)*len,"%s:%s",msg,error_msg);
	}
	else
		res=strdup(error_msg);

	return res;
}

char *kp_errnum(int errnum,char *msg)
{
	int temp=kp_errno;
	char *res;

	kp_errno=errnum;
	res=kp_error(msg);
	kp_errno=temp;

	return res;
}
