/*
 * 获取用户信息
 * argv[1] consumer_key
 * argv[2] consumer_secret
 * argv[3] access_token
 * argv[4] access_secret
 */

#include "kp/kp.h"

int main(int argc,char *argv[])
{
	KP *kp;
	KP_ARG *arg;
	KP_USER_INFO *user;

	kp=kp_init(argv[1],argv[2],argv[3],argv[4]);
	arg=kp_oauth_arg_init(kp);
	/*
	 * 获取用户信息
	 * 如果成功则返回true
	 * 出错则返回false并设置kp_errno
	 * 如果可能的话kp->errmsg会有错误信息
	 */
	if(kp_get_user_info(kp,arg,&user))
	{
		printf("用户ID:%ld\n",user->user_id);
		printf("用户名:%s\n",user->user_name);
		printf("允许上传最大文件:%ld M\n",user->max_file_size/1024/1024);
		printf("用户空间总量:%ld M\n",user->quota_total/1024/1024);
		printf("用户已使用空间:%ld M\n",user->quota_used/1024/1024);
		printf("回收站空间使用量:%ld M\n",user->quota_recycled/1024/1024);

		//释放内存
		kp_user_info_free(user);
	}
	else
		printf("%s",kp_error(kp->errmsg));

	kp_free(kp);
	kp_arg_destroy(arg);

	return 0;
}
