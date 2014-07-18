/*
 * 上传文件
 * argv[1] 上传的文件名
 * argv[2] 网盘中保存的路径
 */
#include "kp/kp.h"

#define KEY "your consumer_key"
#define SECRET "your consumer_secret"
#define OAUTH_TOKEN "access token"
#define OAUTH_SECRET "oauth secret"

int progress(void *filename,double dltotal,double dlnow,
		double ultotal,double ulnow)
{
	if(ulnow/ultotal > 0)
		printf("上传 %s %.1lf%%\n",(char *)filename,ulnow/ultotal*100.0);

	//必需返回0，不然会出错
	return 0;
}

int main(int argc,char *argv[])
{
	KP *kp;
	KP_ARG *arg;

	kp=kp_init(KEY,SECRET,OAUTH_TOKEN,OAUTH_SECRET);
	arg=kp_oauth_arg_init(kp);

	/*
	 * 上传文件
	 * 第五个参数为要上传的文件
	 * 第六个参数为true表示会得覆盖为false则不覆盖
	 * 第七个参数为一个libcurl进度回调函数，第八个参数为该进度回调函数的传入的数据
	 */
	if(kp_upload_file(kp,arg,"kuaipan",oauth_url_escape(argv[2]),argv[1],true,progress,argv[1]))
		printf("上传%s成功\n",argv[1]);
	else
		printf("%s",kp_error(kp->errmsg));

	kp_free(kp);
	kp_arg_destroy(arg);

	return 0;
}
