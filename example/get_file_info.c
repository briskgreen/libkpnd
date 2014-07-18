/*
 * 获取文件信息
 * argv[1] 要获取的文件或者文件夹(以网盘为相对路径)
 */
#include "kp/kp.h"

#define KEY "your consumer_key"
#define SECRET "your consumer_secret"
#define OAUTH_TOKEN "access token"
#define OAUTH_SECRET "oauth secret"

int main(int argc,char *argv[])
{
	KP *kp;
	KP_ARG *arg;
	KP_FILE_INFO *info;

	kp=kp_init(KEY,SECRET,OAUTH_TOKEN,OAUTH_SECRET);
	arg=kp_oauth_arg_init(kp);
	/*
	 * 获取文件信息
	 * 注意第三个参数和第四个参数会加入到oauth参数列表中
	 * 特别要注意第四个参数需要进行url编码
	 * 事实上所有参数都应该进行url编码再传入
	 * 但在进行url编码时数字和a-Z等不会进行编码，所以第三个参数可以不进行url编码就传入
	 * 但第四个参数中会包括/等字符(相较网盘的路径)所以注意编码
	 */
	if(kp_get_file_info(kp,arg,"kuaipan",oauth_url_escape(argv[1]),&info))
	{
		KP_FILE_NODE *files;

		printf("path:%s\n",info->path);
		printf("root:%s\n",info->root);
		
		/*
		 * type是个枚举类型
		 */
		if(info->type == file)
			printf("这是一个文件!\n");
		else if(info->type == folder)
			printf("这是一个文件夹!\n");
		printf("大小:%ld\n",info->size);
		printf("创建时间:%s\n",info->create_time);
		printf("上次修改时间:%s\n",info->modify_time);
		if(info->rev)
			printf("版本:%s\n",info->rev);

		files=info->files;
		while(files)
		{
			printf("\nsha1:%s\n",files->sha1);
			printf("share id:%s\n",files->share_id);
			printf("file id:%s\n",files->file_id);
			if(files->type == file)
				printf("这是一个文件!\n");
			else if(files->type == folder)
				printf("这是一个文件夹!\n");
			printf("大小:%ld\n",files->size);
			printf("创建时间:%s\n",files->create_time);
			printf("上次修改时间:%s\n",files->modify_time);
			printf("name:%s\n",files->name);
			printf("版本:%s\n",files->rev);

			files=files->next;
		}

		kp_file_info_free(info);
	}
	else
		printf("%s",kp_error(kp->errmsg));

	kp_free(kp);
	kp_arg_destroy(arg);

	return 0;
}
