#include "kp.h"

int _kp_get_user_info(KP *kp,KP_USER_INFO *user,char *data);
int _kp_get_file_info(KP *kp,KP_FILE_INFO *file,char *data);
int _kp_get_file_share(KP *kp,KP_FILE_SHARE *file,char *data);
int object_get_err(json_object *obj,char **res);
void object_string_get(json_object *obj,char **res,char *key);
void object_int_get(json_object *obj,uint32_t *res,char *key);
void object_int64_get(json_object *obj,uint64_t *res,char *key);
KP_FILE_NODE *init_kp_file_node(void);

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
	{
		free(base);
		return KP_ERROR_NO_MEM;
	}
	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);
	free(base);

	res=oauth_http_get(url,NULL);
	free(url);
	if(res == NULL)
		return KP_ERROR_NET_GET;

	return _kp_get_file_info(kp,file,res);
}

int kp_get_file_share(KP *kp,KP_ARG *arg,char *root,char *path,
		KP_FILE_SHARE *file)
{
	char *key;
	char *res;
	char *arg_url;
	char *url;
	char *base;
	int len;

	len=sizeof(char)*(strlen("http://openapi.kuaipan.cn/1/shares/")+strlen(root)+strlen(path)+2);
	if((base=malloc(len)) == NULL)
		return KP_ERROR_NO_MEM;
	snprintf(base,len,"http://openapi.kuaipan.cn/1/shares/%s/%s",root,path);
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
	{
		free(base);
		return KP_ERROR_NO_MEM;
	}
	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);
	free(base);

	res=oauth_http_get(url,NULL);
	free(url);
	if(res == NULL)
		return KP_ERROR_NET_GET;

	return _kp_get_file_share(kp,file,res);
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
	json_object *item;
	KP_FILE_NODE *head,*node;
	int len;
	int i;

	file->files=NULL;
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

	object_string_get(obj,&file->path,"path");
	object_string_get(obj,&file->root,"root");
	object_string_get(obj,&file->file_id,"file_id");
	res=json_object_object_get(obj,"hash");
	if(res)
	{
		strncpy(file->hash,json_object_get_string(res),sizeof(char)*31);
		file->hash[32]='\0';

		json_object_put(res);
	}

	res=json_object_get_string(obj,"type");
	if(res)
	{
		char *str=json_object_get_string(res);
		if(strcmp(str,"folder") == 0)
			file->type=folder;
		else if(strcmp(str,"file") == 0)
			file->type=file;

		json_object_put(res);
	}
	object_int_get(obj,&file->size,"size");
	object_string_get(obj,&file->create_time,"create_tile");
	object_string_get(obj,&file->modity_time,"modity_time");
	object_string_get(obj,&file->name,"name");
	object_string_get(obj,&file->rev,"rev");
	res=json_object_object_get(obj,"is_deleted");
	if(res)
	{
		file->is_deleted=json_object_get_boolean(res);
		json_object_put(res);
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

	head=file->files;
	for(i=0;i < len;++i)
	{
		res=json_object_array_get_idx(files,i);
		
		node=init_kp_file_node();
		if(node == NULL)
			continue;

		object_string_get(res,&node->file_id,"file_id");
		item=json_object_object_get(res,"type");
		if(item)
		{
			char *str=json_object_get_string(item);
			if(strcmp(str,"folder") == 0)
				node->type=folder;
			else if(strcmp(str,"file") == 0)
				node->type=file;

			json_object_put(item);
		}
		object_int_get(res,&node->size,"size");
		object_string_get(res,&node->create_time,"create_time");
		object_string_get(res,&node->modity_time,"modity_time");
		object_string_get(res,&node->name,"name");
		object_string_get(res,&node->rev,"rev");
		item=json_object_object_get(res,"is_deleted");
		if(item)
		{
			node->is_deleted=json_object_get_boolean(item);
			json_object_put(item);
		}

		if(head == NULL)
		{
			file->files=node;
			head=node;
		}
		else
		{
			head->next=node;
			head=node;
		}

		json_object_put(res);
	}

	json_object_put(obj);
	free(data);

	return 0;
}

int _kp_get_file_share(KP *kp,KP_FILE_SHARE *file,char *data)
{
	json_object *obj;

	obj=json_tokener_paser(data);
	if(obj == NULL)
	{
		free(data);
		return KP_ERROR_FILE_SHARE;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(data);
		return KP_ERROR_FILE_SHARE;
	}

	object_string_get(obj,&file->url,"url");
	object_string_get(obj,&file->access_code,"access_code");

	json_object_put(obj);
	free(data);

	return 0;
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

KP_FILE_NODE *init_kp_file_node(void)
{
	KP_FILE_NODE *node;

	node=malloc(sizeof(KP_FILE_NODE));
	if(node == NULL)
		return NULL;

	node->file_id=NULL;
	node->type=file;
	node->size=0;
	node->create_time=NULL;
	node->modity_time=NULL;
	node->rev=NULL;
	node->is_deleted=false;

	return node;
}
