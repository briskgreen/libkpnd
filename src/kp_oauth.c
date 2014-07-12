#include "kp_oauth.h"

#define KP_ADD(key,value) \
{\
	if(!kp_arg_add(arg,key,value))\
		return NULL;\
}

char *oauth_get_key(KP *kp,KP_ARG *arg);
char *oauth_get_base_key(char *http_method,char *url,KP_VALUE *head);
char *kp_get_token(KP *kp,char *data);
bool oauth_get_access_token(KP *kp,char *data);

KP_ARG *kp_oauth_arg_init(KP *kp)
{
	KP_ARG *arg;
	char *nonce;
	char timestamp[64]={0};
	time_t t;

	arg=kp_arg_init();
	if(arg == NULL)
		return NULL;

	t=time(NULL);
	snprintf(timestamp,sizeof(timestamp),"%ld",t);
	nonce=oauth_gen_nonce();
	if(!nonce)
	{
		kp_errno=KP_ERROR_NONCE;
		return NULL;
	}

	KP_ADD("oauth_consumer_key",kp->key);
	if(kp->oauth_token)
		KP_ADD("oauth_token",kp->oauth_token);
	KP_ADD("oauth_signature_method","HMAC-SHA1");
	KP_ADD("oauth_timestamp",timestamp);
	KP_ADD("oauth_nonce",nonce);
	KP_ADD("oauth_version","1.0");

	free(nonce);

	return arg;
}

bool kp_oauth_update_timestamp(KP_ARG *arg)
{
	time_t t;
	char timestamp[64]={0};

	t=time(NULL);
	snprintf(timestamp,sizeof(timestamp),"%ld",t);
	
	return kp_arg_update(arg,"oauth_timestamp",timestamp);
}

bool kp_oauth_update_nonce(KP_ARG *arg)
{
	char *nonce;

	nonce=oauth_gen_nonce();
	if(!nonce)
	{
		kp_errno=KP_ERROR_NONCE;
		return false;
	}

	if(kp_arg_update(arg,"oauth_nonce",nonce))
	{
		free(nonce);
		return true;
	}

	free(nonce);
	return false;
}

void kp_oauth_update_signature(KP_ARG *arg,char *key)
{
	if(kp_arg_update(arg,"oauth_signature",key))
		return;
	else
		kp_arg_add(arg,"oauth_signature",key);
}

void kp_oauth_update_token(KP_ARG *arg,char *token)
{
	if(kp_arg_update(arg,"oauth_token",token))
		return;
	else
		kp_arg_add(arg,"oauth_token",token);
}

char *kp_get_oauth_key(KP *kp,char *http_method,char *url,KP_ARG *arg)
{
	char *key;
	char *signature;
	char *buf;
	char *base_string;

	kp_arg_sort(arg);

	base_string=oauth_get_base_key(http_method,url,arg->arg);
	if(base_string == NULL)
		return NULL;

	key=oauth_get_key(kp,arg);
	if(key == NULL)
	{
		free(base_string);
		return NULL;
	}

	buf=oauth_sign_hmac_sha1(base_string,key);
	if(buf == NULL)
	{
		free(base_string);
		free(key);
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}

	signature=oauth_url_escape(buf);
	if(signature == NULL)
	{
		free(buf);
		free(base_string);
		free(key);
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}

	free(buf);
	free(key);
	free(base_string);

	return signature;
}

char *kp_get_authorize_url(KP *kp,KP_ARG *arg)
{
	char *url;
	char *res;
	char *arg_url;
	char *signature;
	char *base_url="https://openapi.kuaipan.cn/open/requestToken";
	char *token_url="https://www.kuaipan.cn/api.php?ac=open&op=authorise&oauth_token=";
	int len;

	kp_oauth_update_timestamp(arg);
	kp_oauth_update_nonce(arg);

	signature=kp_get_oauth_key(kp,"GET",base_url,arg);
	if(signature == NULL)
		return NULL;
	kp_oauth_update_signature(arg,signature);
	free(signature);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
		return NULL;

	len=strlen(base_url)+strlen(arg_url);
	url=malloc(sizeof(char)*len+2);
	if(url == NULL)
	{
		free(arg_url);
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}
	snprintf(url,sizeof(char)*len+2,"%s?%s",base_url,arg_url);
	free(arg_url);

	res=oauth_http_get(url,NULL);
	free(url);
	if(res == NULL)
	{
		kp_errno=KP_ERROR_NET_GET;
		return NULL;
	}

	arg_url=kp_get_token(kp,res);
	free(res);
	if(arg_url == NULL)
		return NULL;

	len=strlen(token_url)+strlen(arg_url);
	url=malloc(sizeof(char)*len+1);
	if(url == NULL)
	{
		free(arg_url);
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}
	snprintf(url,sizeof(char)*len+1,"%s%s",token_url,arg_url);
	free(arg_url);

	return url;
}

bool kp_get_access_token(KP *kp,KP_ARG *arg)
{
	char *signature;
	char *base_url="https://openapi.kuaipan.cn/open/accessToken";
	char *url;
	char *res;
	char *arg_url;
	int len;

	kp_oauth_update_token(arg,kp->oauth_token);
	kp_oauth_update_timestamp(arg);
	kp_oauth_update_nonce(arg);

	signature=kp_get_oauth_key(kp,"GET",url,arg);
	if(signature == NULL)
		return false;
	kp_oauth_update_signature(arg,signature);
	free(signature);

	arg_url=kp_arg_get_url(arg->arg);
	if(arg_url == NULL)
		return false;

	len=strlen(base_url)+strlen(arg_url);
	url=malloc(sizeof(char)*(len+2));
	if(url == NULL)
	{
		free(arg_url);
		kp_errno=KP_ERROR_NO_MEM;
		return false;
	}
	snprintf(url,sizeof(char)*(len+2),"%s?%s",base_url,arg_url);
	free(arg_url);

	res=oauth_http_get(url,NULL);
	free(url);
	if(res == NULL)
	{
		kp_errno=KP_ERROR_NET_GET;
		return false;
	}

	return oauth_get_access_token(kp,res);
}

char *oauth_get_key(KP *kp,KP_ARG *arg)
{
	char *key;
	int len;

	if(kp_arg_in(arg,"oauth_token"))
	{
		len=strlen(kp->secret)+strlen(kp->oauth_secret)+1;
		key=malloc(sizeof(char)*len+1);
		if(key == NULL)
		{
			kp_errno=KP_ERROR_NO_MEM;
			return NULL;
		}
		snprintf(key,sizeof(char)*len+1,"%s&%s",kp->secret,kp->oauth_secret);
	}
	else
	{
		len=strlen(kp->secret)+1;
		key=malloc(sizeof(char)*len+1);
		if(key == NULL)
		{
			kp_errno=KP_ERROR_NO_MEM;
			return NULL;
		}
		snprintf(key,sizeof(char)*len+1,"%s&",kp->secret);
	}

	return key;
}

char *oauth_get_base_key(char *http_method,char *url,KP_VALUE *head)
{
	char *res=NULL;
	char *buf;
	char *arg;

	res=realloc(res,strlen(http_method)+2);
	if(res == NULL)
	{
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}
	snprintf(res,strlen(http_method)+2,"%s&",http_method);
	buf=oauth_url_escape(url);
	if(buf == NULL)
	{
		free(res);
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}

	res=realloc(res,strlen(res)+strlen(buf)+2);
	if(res == NULL)
	{
		free(buf);
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}
	strcat(res,buf);
	strcat(res,"&");
	free(buf);

	arg=kp_arg_get_url(head);
	if(arg == NULL)
	{
		free(res);
		return NULL;
	}

	buf=oauth_url_escape(arg);
	if(buf == NULL)
	{
		free(res);
		free(arg);
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}

	res=realloc(res,strlen(res)+strlen(buf)+1);
	if(res == NULL)
	{
		free(arg);
		free(buf);
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}
	strcat(res,buf);
	free(buf);
	free(arg);

	return res;
}

char *kp_get_token(KP *kp,char *data)
{
	json_object *obj;
	json_object *str;
	char *res;

	obj=json_tokener_parse(data);
	if(obj == NULL)
	{
		kp_errno=KP_ERROR_TOKEN;
		return NULL;
	}

	str=json_object_object_get(obj,"msg");
	if(str)
	{
		if(strcmp(json_object_get_string(str),"ok") == 0)
			goto ok;

		if(kp->errmsg)
			free(kp->errmsg);
		kp->errmsg=strdup(json_object_get_string(str));
		json_object_put(str);
		json_object_put(obj);

		return NULL;
	}
ok:
	str=json_object_object_get(obj,"oauth_token");
	if(str == NULL)
	{
		json_object_put(obj);
		kp_errno=KP_ERROR_TOKEN;
		return NULL;
	}
	res=strdup(json_object_get_string(str));
	kp->oauth_token=strdup(json_object_get_string(str));
	json_object_put(str);

	str=json_object_object_get(obj,"oauth_token_secret");
	if(str == NULL)
	{
		json_object_put(obj);
		free(kp->oauth_token);
		free(res);
		kp_errno=KP_ERROR_TOKEN;
		return NULL;
	}
	
	kp->oauth_secret=strdup(json_object_get_string(str));
	json_object_put(str);
	json_object_put(obj);

	return res;
}

bool oauth_get_access_token(KP *kp,char *data)
{
	json_object *obj;
	json_object *str;

	obj=json_tokener_parse(data);
	if(obj == NULL)
	{
		kp_errno=KP_ERROR_ACCESS_TOKEN;
		free(data);
		return false;
	}

	if(str=json_object_object_get(obj,"msg"))
	{
		json_object_put(str);
		json_object_put(obj);
		free(data);
		kp_errno=KP_ERROR_ACCESS_TOKEN;
		return false;
	}

	str=json_object_object_get(obj,"oauth_token");
	if(str)
	{
		kp->oauth_token=strdup(json_object_get_string(str));
		json_object_put(str);
	}

	str=json_object_object_get(obj,"oauth_token_secret");
	if(str)
	{
		kp->oauth_secret=strdup(json_object_get_string(str));
		json_object_put(str);
	}

	json_object_put(obj);
	free(data);

	return true;
}
