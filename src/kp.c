#include "kp.h"

int _kp_get_user_info(KP *kp,KP_USER_INFO *user,char *data);
int _kp_get_file_info(KP *kp,KP_FILE_INFO *file,char *data);
int object_get_err(json_object *obj,char **res);
void object_string_get(json_object *obj,char **res,char *key);
void object_int_get(json_object *obj,uint32_t *res,char *key);
void object_int64_get(json_object *obj,uint64_t *res,char *key);

int kp_get_user_info(KP *kp,KP_ARG *arg,KP_USER_INFO *user)
{
	char *res;
	char *arg_url;
	char *url;
	char *key;
	char *base="http://openapi.kuaipan.cn/1/account_info";

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);

	key=kp_get_oauth_key(kp,"GET",base,arg);
	if(key == NULL)
		return KP_ERROR_ARG;
	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
		return KP_ERROR_ARG;

	url=malloc(sizeof(char)*base+sizeof(char)*(strlen(arg_url)+2));
	if(url == NULL)
	{
		free(arg_url);
		return KP_ERROR_NO_MEM;
	}

	snprintf(url,sizeof(char)*base+sizeof(char)*(strlen(arg_url)+2),"%s?%s",base,arg_url);
	free(arg_url);

	res=oauth_http_get(url,NULL);
	free(url);

	if(res == NULL)
		return KP_ERROR_NET_GET;

	return _kp_get_user_info(kp,user,res);
}

int kp_get_file_info(KP *kp,KP_ARG *arg,char *root,
		char *path,KP_FILE_INFO *file)
{
	char *key;
	char *res;
	char *arg_url;
	char *url;
	char *base;
	int len;

	len=sizeof(char)*(strlen("http://openapi.kuaipan.cn/1/metadata/")+strlen(root)+strlen(path)+2);
	if((base=malloc(len)) == NULL)
		return KP_ERROR_NO_MEM;
	snprintf(base,len,"http://openapi.kuaipan.cn/1/metadata/%s/%s",root,path);

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);
	key=kp_get_oauth_key(kp,"GET",base,arg);
	if(key == NULL)
	{
		free(base);
		return KP_ERROR_ARG;
	}
	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg);
	if(arg_url == NULL)
	{
		free(base);
		return KP_ERROR_ARG;
	}

	len+=sizeof(char)*(strlen(arg_url)+1);
	if((url=malloc(len)) == NULL)
		return KP_ERROR_NO_MEM;
	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);
	free(base);

	res=oauth_http_get(url,NULL);
	free(url);
	if(res == NULL)
		return KP_ERROR_NET_GET;

	return _kp_get_file_info(kp,file,res);
}

int _kp_get_user_info(KP *kp,KP_USER_INFO *user,char *data)
{
	json_object *obj;

	obj=json_tokener_paser(data);
	if(obj == NULL)
	{
		free(data);

		return KP_ERROR_USER_INFO;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(data);
		return KP_ERROR_USER_INFO;
	}

	object_int_get(obj,&user->user_id,"user_id");
	object_string_get(obj,&user->user_name,"user_name");
	object_int64_get(obj,&user->max_file_size,"max_file_size");
	object_int64_get(obj,&user->quota_total,"quota_total");
	object_int64_get(obj,&user->quota_used,"quota_used");
	object_int64_get(obj,&user->quota_recycled,"quota_recycled");

	json_object_put(obj);
	free(data);

	return 0;
}

int _kp_get_file_info(KP *kp,KP_FILE_INFO *file,char *data)
{
	json_object *obj;
	json_object *files;
	json_object *res;
	KP_FILE_INFO *node;
	int len;
	int i;

	obj=json_tokener_paser(data);
	if(obj == NULL)
	{
		free(data);
		return KP_ERROR_FILE_INFO;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(data);
		return KP_FILE_INFO;
	}

	files=json_object_object_get(obj,"files");
	len=json_object_array_length(files);
	if(len < 0)
	{
		json_object_put(files);
		json_object_put(obj);
		free(data);
		return KP_ERROR_FILE_INFO;
	}

	for(i=0;i < len;++i)
	{
		res=json_object_array_get_idx(files,i);

		node=malloc(sizeof(KP_FILE_INFO));
		if(node == NULL)
			continue;

		object_string_get(res,&node->
	}
}

int object_get_err(json_object *obj,char **res)
{
	json_object *data;

	data=json_object_object_get(obj,"msg");
	if(data)
	{
		if(*res)
			free(*res);
		*res=strdup(json_object_get_string(data));
		json_object_put(data);
		return 1;
	}

	return 0;
}

void object_string_get(json_object *obj,char **res,char *key)
{
	json_object *data;

	data=json_object_object_get(obj,key);
	if(data)
	{
		*res=json_object_get_string(data);
		json_object_put(data);
	}

	*res=NULL;
}

void object_int_get(json_object *obj,uint32_t *res,char *key)
{
	json_object *data;

	data=json_object_object_get(obj,key);
	if(data)
	{
		*res=json_object_get_int(data);
		json_object_put(data);
	}
}

void object_int64_get(json_object *obj,uint64_t *res,char *key)
{
	json_object *data;

	data=json_object_object_get(obj,key);
	if(data)
	{
		*res=json_object_get_int64(data);
		json_object_put(data);
	}
}
