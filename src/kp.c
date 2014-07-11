#include "kp.h"

typedef struct
{
	size_t len;
	char *data;
}KP_RET;

int _kp_get_user_info(KP *kp,KP_USER_INFO *user,char *data);
int _kp_get_file_info(KP *kp,KP_FILE_INFO *file,char *data);
int _kp_get_file_share(KP *kp,KP_FILE_SHARE *file,char *data);
int _kp_get_file_history(KP *kp,KP_FILE_HIS *his,char *data);

int object_get_err(json_object *obj,char **res);
void object_string_get(json_object *obj,char **res,char *key);
void object_int_get(json_object *obj,uint32_t *res,char *key);
void object_int64_get(json_object *obj,uint64_t *res,char *key);

KP_FILE_NODE *init_kp_file_node(void);
//libcurl获取返回数据的回调函数
size_t kp_get_data(char *ptr,size_t size,size_t nmemb,KP_RET *data);
size_t kp_save_to_file(char *ptr,size_t size,size_t nmemb,FILE *fp);

int kp_get_user_info(KP *kp,KP_ARG *arg,KP_USER_INFO *user)
{
	char *res;
	char *arg_url;
	char *url;
	char *key;
	char *base="http://openapi.kuaipan.cn/1/account_info";
	int len;

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

	len=sizeof(char)*(strlen(base)+strlen(arg_url)+2);
	url=malloc(len);
	if(url == NULL)
	{
		free(arg_url);
		return KP_ERROR_NO_MEM;
	}

	snprintf(url,len,"%s?%s",base,arg_url);
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

	arg_url=kp_arg_get_url(arg->arg);
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

	arg_url=kp_arg_get_url(arg->arg);
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

int kp_get_file_history(KP *kp,KP_ARG *arg,char *root,char *path,KP_FILE_HIS *his)
{
	char *key;
	char *res;
	char *arg_url;
	char *url;
	char *base;
	int len;

	len=sizeof(char)*(strlen("http://openapi.kuaipan.cn/1/history/")+strlen(root)+strlen(path)+2);
	if((base=malloc(len)) == NULL)
		return KP_ERROR_NO_MEM;
	snprintf(base,len,"http://openapi.kuaipan.cn/1/history/%s/%s",root,path);
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

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
	{
		free(base);
		return KP_ERROR_ARG;
	}

	len+=sizeof(char)*(strlen(arg_url+1));
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

	return _kp_get_file_history(kp,his,res);
}

int kp_create_file(KP *kp,KP_ARG *arg,char *root,char *path)
{
	char *res;
	char *arg_url;
	char *url;
	char *key;
	char *base="http://openapi.kuaipan.cn/1/fileops/create_folder";
	int len;
	json_object *obj;

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);
	kp_arg_add(arg,"root",root);
	kp_arg_add(arg,"path",path);

	key=kp_get_oauth_key(kp,"GET",base,arg);
	if(key == NULL)
		return KP_ERROR_ARG;
	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
		return KP_ERROR_ARG;

	len=sizeof(char)*(strlen(base)+strlen(arg_url)+2);
	url=malloc(len);
	if(url == NULL)
	{
		free(arg_url);
		return KP_ERROR_NO_MEM;
	}

	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);

	res=oauth_http_get(url,NULL);
	free(url);
	if(res == NULL)
		return KP_ERROR_NET_GET;

	obj=json_tokener_paser(res);
	if(obj == NULL)
	{
		free(res);
		return KP_ERROR_CREATE_FILE;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(res);

		return KP_ERROR_CREATE_FILE;
	}
	else
	{
		json_object_put(obj);
		free(res);

		return 0;
	}
}

int kp_delete_file(KP *kp,KP_ARG *arg,char *root,char *path)
{
	char *res;
	char *arg_url;
	char *url;
	char *key;
	char *base="http://openapi.kuaipan.cn/1/fileops/delete ";
	int len;
	json_object *obj;

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);
	kp_arg_add(arg,"root",root);
	kp_arg_add(arg,"path",path);

	key=kp_get_oauth_key(kp,"GET",base,arg);
	if(key == NULL)
		return KP_ERROR_ARG;

	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
		return KP_ERROR_ARG;

	len=sizeof(char)*(strlen(base)+strlen(arg_url)+2);
	if((url=malloc(len)) == NULL)
	{
		free(arg_url);
		return KP_ERROR_NO_MEM;
	}
	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);

	res=oauth_http_get(url,NULL);
	free(url);
	if(res == NULL)
		return KP_ERROR_NET_GET;

	obj=json_tokener_paser(res);
	if(obj == NULL)
	{
		free(res);
		return KP_ERROR_DELETE_FILE;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(res);

		return KP_ERROR_DELETE_FILE;
	}
	else
	{
		json_object_put(obj);
		free(res);

		return 0;
	}
}

int kp_remove_file(KP *kp,KP_ARG *arg,char *root,char *from_path,char *to_path)
{
	char *res;
	char *arg_url;
	char *url;
	char *key;
	char *base="http://openapi.kuaipan.cn/1/fileops/move";
	int len;
	json_object *obj;

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);
	kp_arg_add(arg,"root",root);
	kp_arg_add(arg,"from_path",from_path);
	kp_arg_add(arg,"to_path",to_path);

	key=kp_get_oauth_key(kp,"GET",base,arg);
	if(key == NULL)
		return KP_ERROR_NO_MEM;

	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
		return KP_ERROR_ARG;

	len=sizeof(char)*(strlen(base)+strlen(arg_url)+2);
	if((url=malloc(len)) == NULL)
	{
		free(arg_url);
		return KP_ERROR_NO_MEM;
	}
	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);

	res=oauth_http_get(url,NULL);
	free(url);
	if(res == NULL)
		return KP_ERROR_NET_GET;

	obj=json_tokener_paser(res);
	if(obj == NULL)
	{
		free(res);
		return KP_ERROR_REMOVE_FILE;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(res);

		return KP_ERROR_REMOVE_FILE;
	}
	else
	{
		json_object_put(obj);
		free(res);

		return 0;
	}
}

int kp_copy_file(KP *kp,KP_ARG *arg,char *root,char *from_path,char *to_path)
{
	char *res;
	char *arg_url;
	char *url;
	char *key;
	char *base="http://openapi.kuaipan.cn/1/fileops/copy";
	int len;
	json_object *obj;

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);
	kp_arg_add(arg,"root",root);
	kp_arg_add(arg,"from_path",from_path);
	kp_arg_add(arg,"to_path",to_path);

	key=kp_get_oauth_key(kp,"GET",base,arg);
	if(key == NULL)
		return KP_ERROR_NO_MEM;

	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
		return KP_ERROR_ARG;

	len=sizeof(char)*(strlen(base)+strlen(arg_url)+2);
	if((url=malloc(len)) == NULL)
	{
		free(arg_url);
		return KP_ERROR_NO_MEM;
	}
	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);

	res=oauth_http_get(url,NULL);
	free(url);
	if(res == NULL)
		return KP_ERROR_NET_GET;

	obj=json_tokener_paser(res);
	if(obj == NULL)
	{
		free(res);
		return KP_ERROR_COPY_FILE;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(res);

		return KP_ERROR_COPY_FILE;
	}
	else
	{
		json_object_put(obj);
		free(res);

		return 0;
	}
}

int kp_copy_ref(KP *kp,KP_ARG *arg,KP_REF *ref,char *root,char *path)
{
	char *key;
	char *res;
	char *arg_url;
	char *url;
	char *base;
	int len;
	json_object *obj;

	len=sizeof(char)*(strlen("http://openapi.kuaipan.cn/1/copy_ref/")+strlen(root)+strlen(path)+2);
	if((base=malloc(len)) == NULL)
		return KP_ERROR_NO_MEM;
	snprintf(base,len,"http://openapi.kuaipan.cn/1/copy_ref/%s/%s",root,path);
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

	arg_url=kp_arg_get_url(arg->arg);
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

	obj=json_tokener_paser(res);
	if(obj == NULL)
	{
		free(res);
		return KP_ERROR_COPY_REF;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(res);
		
		return KP_ERROR_COPY_REF;
	}
	else
	{
		object_string_get(obj,&ref->copy_ref,"copy_ref");
		object_string_get(obj,&ref->expires,"expires");
		json_object_put(obj);
		free(res);

		return 0;
	}
}

char *kp_get_upload_url(KP *kp,KP_ARG *arg)
{
	char *url;
	char *res;
	char *arg_url;
	char *key;
	char *base="http://api-content.dfs.kuaipan.cn/1/fileops/upload_locate";
	int len;
	json_object *obj;

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);
	key=kp_get_oauth_key(kp,"GET",base,arg);
	if(key == NULL)
		return NULL;

	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
		return NULL;

	len=sizeof(char)*(strlen(base)+strlen(arg_url)+2);
	if((url=malloc(len)) == NULL)
	{
		free(arg_url);
		return NULL;
	}
	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);

	res=oauth_http_get(url,NULL);
	free(url);
	if(res == NULL)
		return NULL;

	obj=json_tokener_paser(res);
	if(obj == NULL)
	{
		free(res);
		return NULL;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(res);

		return NULL;
	}
	else
	{
		object_string_get(obj,&url,"url");
		json_object_put(obj);
		free(res);

		return url;
	}
}

int kp_upload_file(KP *kp,KP_ARG *arg,char *filename,
		kp_progress func,void *data)
{
	char *url;
	CURL *curl;
	int len;
	char *key;
	char *base;
	char *arg_url;
	KP_RET res;
	CURLcode code;
	struct curl_httppost *post=NULL,*last=NULL;
	json_object *obj;

	url=kp_get_upload_url(kp,arg);
	if(url == NULL)
		return KP_ERROR_UPLOAD_URL;
	len=sizeof(char)*(strlen(url)+strlen("/1/fileops/upload_file")+1);
	if((base=malloc(len)) == NULL)
	{
		free(url);
		return KP_ERROR_NO_MEM;
	}
	snprintf(base,len,"%s%s",url,base);
	free(url);
	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);
	key=kp_get_oauth_key(kp,"POST",base,arg);
	if(key == NULL)
	{
		free(base);
		return KP_ERROR_ARG;
	}
	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg->arg);
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

	curl_formadd(&post,&last,CURLFORM_COPYNAME,"file",CURLFORM_FILE,filename,CURLFORM_END);
	res.len=0;
	res.data=NULL;

	curl=curl_easy_init();
	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl,CURLOPT_HTTPPOST,post);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,kp_get_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,*res);
	if(func)
	{
		curl_easy_setopt(curl,CURLOPT_NOPROGRESS,false);
		curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,func);
		curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,data);
	}

	code=curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	curl_formfree(post);
	free(url);

	if(code != 0)
		return code;

	obj=json_tokener_paser(res.data);
	if(obj == NULL)
	{
		if(res.data)
			free(res.data);
		return KP_ERROR_UPLOAD_FILE;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(res.data);

		return KP_ERROR_UPLOAD_FILE;
	}
	else
	{
		json_object_put(obj);
		free(res.data);

		return 0;
	}
}

int kp_download_file(KP *kp,KP_ARG *arg,char *root,char *path,
		char *filename,kp_progress func,void *data)
{
	char *arg_url;
	char *url;
	char *key;
	char *base="http://api-content.dfs.kuaipan.cn/1/fileops/download_file";
	FILE *fp;
	int len;
	CURL *curl;
	CURLcode code;
	long httpcode;
	json_object *obj;

	if(access(filename,F_OK) == -1)
	{
		if((fp=fopen(filename,"wb")) == NULL)
			return KP_ERROR_CREATE_FILE;
	}
	else
		return KP_ERROR_FILE_ALREADY_EXISTS;

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);
	kp_arg_add(arg,"root",root);
	kp_arg_add(arg,"path",path);

	key=kp_get_oauth_key(kp,"GET",base,arg);
	if(key == NULL)
	{
		fclose(fp);
		return KP_ERROR_NO_MEM;
	}
	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
	{
		fclose(fp);
		return KP_ERROR_ARG;
	}
	len=sizeof(char)*(strlen(base)+strlen(arg_url)+1);
	if((url=malloc(len)) == NULL)
	{
		free(arg_url);
		fclose(fp);
		return KP_ERROR_ARG;
	}
	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);

	curl=curl_easy_init();
	curl_easy_setopt(curl,CURLOPT_URL,url);
#ifdef _WIN32
	curl_easy_setopt(curl,CURLOPT_COOKIEJAR,NULL);
	curl_easy_setopt(curl,CURLOPT_COOKIEFILE,NULL);
#else
	curl_easy_setopt(curl,CURLOPT_COOKIEJAR,"/dev/null");
	curl_easy_setopt(curl,CURLOPT_COOKIEFILE,"/dev/null");
#endif
	if(func)
	{
		curl_easy_setopt(curl,CURLOPT_NOPROGRESS,false);
		curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,func);
		curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,data);
	}
	curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,true);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,kp_save_to_file);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

	code=curl_easy_perform(curl);
	curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&httpcode);
	curl_easy_cleanup(curl);
	free(url);
	fclose(fp);

	if(code != 0)
	{
		remove(filename);
		return code;
	}

	if(httpcode == 200)
		return 0;

	obj=json_object_from_file(filename);
	if(obj == NULL)
	{
		remove(filename);
		return KP_ERROR_DOWNLOAD_FILE;
	}

	object_get_err(obj,&kp->errmsg);
	json_object_put(obj);
	remove(filename);

	return KP_ERROR_DOWNLOAD_FILE;
}

int kp_get_thumbnail(KP *kp,KP_ARG *arg,int width,int height,
		char *root,char *path,char *filename)
{
	CURL *curl;
	char *arg_url;
	char *url;
	char *key;
	char *base="http://conv.kuaipan.cn/1/fileops/thumbnail";
	int len;
	json_object *obj;
	char v_width[20];
	char v_height[20];
	CURLcode code;
	long httpcode;
	FILE *fp;

	if(access(filename,F_OK) == -1)
	{
		if((fp=fopen(filename,"wb")) == NULL)
			return KP_ERROR_CREATE_FILE;
	}
	else
		return KP_ERROR_FILE_ALREADY_EXISTS;

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);
	kp_arg_add(arg,"root",root);
	kp_arg_add(arg,"path",path);
	snprintf(v_width,sizeof(v_width),"%d",width);
	snprintf(v_height,sizeof(v_height),"%d",height);
	kp_arg_add(arg,"width",v_width);
	kp_arg_add(arg,"height",v_height);

	key=kp_get_oauth_key(kp,"GET",base,arg);
	if(key == NULL)
		return KP_ERROR_NO_MEM;
	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
		return KP_ERROR_ARG;

	len=sizeof(char)*(strlen(base)+strlen(arg_url)+2);
	if((url=malloc(len)) == NULL)
	{
		free(arg_url);
		return KP_ERROR_NO_MEM;
	}
	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);

	curl=curl_easy_init();
	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,kp_download_file);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

	code=curl_easy_perform(curl);
	curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&httpcode);
	curl_easy_cleanup(curl);
	fclose(fp);
	free(url);

	if(code != 0)
		return code;

	if(httpcode == 200)
		return 0;

	obj=json_object_from_file(filename);
	if(obj == NULL)
	{
		remove(filename);
		return KP_ERROR_THUMBNAIL;
	}

	object_get_err(obj,&kp->errmsg);
	json_object_put(obj);
	remove(filename);

	return KP_ERROR_THUMBNAIL;
}

int kp_doc_change(KP *kp,KP_ARG *arg,enum KP_CH_TYPE type,
		enum KP_VIEW view,char *root,char *path,
		char *filename)
{
	CURL *curl;
	char *arg_url;
	char *url;
	char *key;
	char *base="http://conv.kuaipan.cn/1/fileops/documentView";
	int len;
	json_object *obj;
	char *v_type;
	char *v_view;
	CURLcode code;
	long httpcode;
	FILE *fp;

	if(access(filename,F_OK) == -1)
	{
		if((fp=fopen(filename,"wb")) == NULL)
			return KP_ERROR_CREATE_FILE;
	}
	else
		return KP_ERROR_FILE_ALREADY_EXISTS;

	switch(type)
	{
		case pdf:
			v_type="pdf";
			break;
		case doc:
			v_type="doc";
			break;
		case wps:
			v_type="wps";
			break;
		case csv:
			v_type="csv";
			break;
		case prn:
			v_type="prn";
			break;
		case xls:
			v_type="xls";
			break;
		case et:
			v_type="et";
			break;
		case ppt:
			v_type="ppt";
			break;
		case dps:
			v_type="dps";
			break;
		case txt:
			v_type="txt";
			break;
		case rtf:
			v_type="rtf";
			break;

		default:
			return KP_ERROR_CHANGE_TYPE;
	}

	switch(view)
	{
		case normal:
			v_view="normal";
			break;
		case android:
			v_view="android";
			break;
		case iPad:
			v_view="iPad";
			break;
		case iphone:
			v_view="iphone";
			break;

		default:
			return KP_ERROR_CHANGE_VIEW;
	}

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_once(arg);
	kp_arg_add(arg,"root",root);
	kp_arg_add(arg,"path",path);
	kp_arg_add(arg,"type",v_type);
	kp_arg_add(arg,"view",v_view);

	key=kp_get_oauth_key(kp,"GET",base,arg);
	if(key == NULL)
		return KP_ERROR_NO_MEM;
	kp_oauth_update_signature(arg,key);
	free(key);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
		return KP_ERROR_ARG;

	len=sizeof(char)*(strlen(base)+strlen(arg_url)+2);
	if((url=malloc(len)) == NULL)
	{
		free(arg_url);
		return KP_ERROR_NO_MEM;
	}
	snprintf(url,len,"%s?%s",base,arg_url);
	free(arg_url);

	curl=curl_easy_init();
	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,kp_download_file);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);

	code=curl_easy_perform(curl);
	curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&httpcode);
	curl_easy_cleanup(curl);
	fclose(fp);
	free(url);

	if(code != 0)
		return code;

	if(httpcode == 200)
		return 0;

	obj=json_object_from_file(filename);
	if(obj == NULL)
	{
		remove(filename);
		return KP_ERROR_DOC_CHANGE;
	}

	object_get_err(obj,&kp->errmsg);
	json_object_put(obj);
	remove(filename);

	return KP_ERROR_DOC_CHANGE;
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

int _kp_get_file_history(KP *kp,KP_FILE_HIS *his,char *data)
{
	json_object *obj;
	json_object *files;

	obj=json_tokener_paser(data);
	if(obj == NULL)
	{
		free(data);

		return KP_ERROR_FILE_HISTORY;
	}

	if(object_get_err(obj,&kp->errmsg))
	{
		json_object_put(obj);
		free(data);
		
		return KP_ERROR_FILE_HISTORY;
	}

	files=json_object_object_get(obj,"files");
	object_string_get(files,&his->file_id,"file_id");
	object_string_get(files,&his->rev,"rev");
	object_string_get(files,&his->create_time,"create_time");

	json_object_put(files);
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
		if(strcmp(json_object_get_string(data,"ok") == 0))
		{
			json_object_put(data);
			return 0;
		}

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

size_t kp_get_data(char *ptr,size_t size,size_t nmemb,KP_RET *data)
{
	data->data=realloc(data->data,sizeof(char)*(data->len+nmemb+1));
	snprintf(data->data+data->len,sizeof(char)*(nmemb+1),"%s",ptr);

	data->len+=nmemb;

	return nmemb;
}

size_t kp_save_to_file(char *ptr,size_t size,size_t nmemb,FILE *fp)
{
	return fwrite(ptr,size,nmemb,fp);
}
