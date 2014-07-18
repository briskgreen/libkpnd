/*
 * 获取授权链接地址
 * argv[1] consumer_key
 * argv[2] consumer_secret
 * 程序返回一个进行授权认证的网页地址
 */
#include "kp/kp_oauth.h"

int main(int argc,char **argv)
{
	KP *kp;
	KP_ARG *arg;
	char *url;

	/*
	 * 初始化
	 * 当应用没有被授权时后两个参数可以设置为NULL
	 * 函数会为kp开辟内存空间，使用完后注意使用kp_free进行释放
	 */
	kp=kp_init(argv[1],argv[2],NULL,NULL);
	/*
	 * 初始化Oauth参数列表
	 * 这两步是每个程序中都必有的两步
	 * 使用完成后可以使用kp_arg_destroy销势列表
	 */
	arg=kp_oauth_arg_init(kp);

	/*
	 * 获取应用授权网页地址
	 * 就是这么简单
	 * 函数成功返回应用授权网页地址
	 * 函数失败时返回NULL
	 * 返回错误时kp->errmsg可能存放错误信息
	 */
	url=kp_get_authorize_url(kp,arg);
	if(url == NULL)
		printf("%s",kp_error(kp->errmsg)); //打印错误信息
	else
	{
		printf("请将%s复制到浏览器中!\n",url);
		//返回的地址需要进行释放
		free(url);
		/*
		 * kp->oauth_token 获取得到的access_token
		 * kp->secret 授权后的oauth_secret
		 */
		printf("access_token:%s\naccess_secret:%s\n",kp->oauth_token,kp->oauth_secret);
	}

	kp_free(kp);
	kp_arg_destroy(arg);

	return 0;
}
