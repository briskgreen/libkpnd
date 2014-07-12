#include "kp_data.h"

void _kp_arg_add(KP_VALUE *head,KP_VALUE *data);
bool _kp_arg_remove(KP_VALUE *head,char *key);
bool _kp_arg_update(KP_VALUE *head,char *key,char *value);
void _kp_arg_sort(KP_VALUE *head);
bool _kp_arg_in(KP_VALUE *head,char *key);

#define NULL_NOT_FREE(p) \
{\
	if(p)\
		free(p);\
}

KP *kp_init(char *key,char *secret,char *oauth_token,char *oauth_secret)
{
	KP *kp;

	if(key == NULL)
	{
		kp_errno=KP_ERROR_KEY;
		return NULL;
	}
	if(secret == NULL)
	{
		kp_errno=KP_ERROR_SECRET;
		return NULL;
	}

	kp=malloc(sizeof(KP));
	if(kp == NULL)
	{
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}

	kp->key=strdup(key);
	kp->secret=strdup(secret);
	//如果oauth_token参数不为空则设置
	if(oauth_token)
		kp->oauth_token=strdup(oauth_token);
	else
		kp->oauth_token=NULL;
	//与上同
	if(oauth_secret)
		kp->oauth_secret=strdup(oauth_secret);
	else
		kp->oauth_secret=NULL;
//	kp->oauth_callback=false;
//	kp->oauth_verifier=NULL;
	kp->errmsg=NULL;

	return kp;
}

void kp_free(KP *kp)
{
	NULL_NOT_FREE(kp->key);
	NULL_NOT_FREE(kp->secret);
	NULL_NOT_FREE(kp->oauth_token);
	NULL_NOT_FREE(kp->oauth_secret);
	//NULL_NOT_FREE(kp->oauth_verifier);
	NULL_NOT_FREE(kp->errmsg);
	free(kp);
}

void kp_user_info_free(KP_USER_INFO *user)
{
	NULL_NOT_FREE(user->user_name);
	free(user);
}

void kp_file_info_free(KP_FILE_INFO *file)
{
	KP_FILE_NODE *temp;

	NULL_NOT_FREE(file->path);
	NULL_NOT_FREE(file->root);
	NULL_NOT_FREE(file->file_id);
	NULL_NOT_FREE(file->create_time);
	NULL_NOT_FREE(file->modify_time);
	NULL_NOT_FREE(file->name);
	NULL_NOT_FREE(file->rev);

	while(file->files)
	{
		temp=file->files;

		NULL_NOT_FREE(file->files->sha1);
		NULL_NOT_FREE(file->files->share_id);
		NULL_NOT_FREE(file->files->file_id);
		NULL_NOT_FREE(file->files->create_time);
		NULL_NOT_FREE(file->files->modify_time);
		NULL_NOT_FREE(file->files->name);
		NULL_NOT_FREE(file->files->rev);

		file->files=file->files->next;
		free(temp);
	}
}

void kp_file_share_free(KP_FILE_SHARE *file)
{
	NULL_NOT_FREE(file->url);
	NULL_NOT_FREE(file->access_code);
	free(file);
}

void kp_file_his_free(KP_FILE_HIS *file)
{
	NULL_NOT_FREE(file->file_id);
	NULL_NOT_FREE(file->rev);
	NULL_NOT_FREE(file->create_time);
	free(file);
}

void kp_ref_free(KP_REF *ref)
{
	NULL_NOT_FREE(ref->copy_ref);
	NULL_NOT_FREE(ref->expires);
}

KP_ARG *kp_arg_init(void)
{
	KP_ARG *arg;

	arg=malloc(sizeof(KP_ARG));
	if(arg == NULL)
	{
		kp_errno=KP_ERROR_NO_MEM;
		return NULL;
	}

	arg->length=0;
	arg->arg=NULL;

	return arg;
}

bool kp_arg_add(KP_ARG *arg,char *key,char *value)
{
	KP_VALUE *data;

	if(kp_arg_in(arg,key))
		return false;

	data=malloc(sizeof(KP_VALUE));
	if(data == NULL)
	{
		kp_errno=KP_ERROR_NO_MEM;
		return false;
	}

	data->key=strdup(key);
	data->value=strdup(value);
	data->next=NULL;

	//如果当前链表为空则此节点就为表头
	if(arg->arg)
		_kp_arg_add(arg->arg,data);
	else
		arg->arg=data;
	++arg->length;

	return true;
}

void kp_arg_destroy(KP_ARG *arg)
{
	KP_VALUE *head=arg->arg;
	KP_VALUE *temp;

	while(head)
	{
		temp=head;

		free(head->key);
		free(head->value);

		head=head->next;
		free(temp);
	}

	free(arg);
}

void kp_arg_cleanup(KP_ARG *arg)
{
	KP_VALUE *head=arg->arg;
	KP_VALUE *temp;

	while(head)
	{
		temp=head;

		free(head->key);
		free(head->value);

		head=head->next;
		free(temp);
	}

	arg->length=0;
	arg->arg=NULL;
}

void kp_arg_sort(KP_ARG *arg)
{
	_kp_arg_sort(arg->arg);
}

int kp_arg_length(KP_ARG *arg)
{
	return arg->length;
}

bool kp_arg_in(KP_ARG *arg,char *key)
{
	return _kp_arg_in(arg->arg,key);
}

void kp_arg_remove(KP_ARG *arg,char *key)
{
	if(strcmp(arg->arg->key,key) == 0)
	{
		KP_VALUE *temp;

		temp=arg->arg;
		arg->arg=arg->arg->next;
		free(temp->key);
		free(temp->value);
		free(temp);
		--arg->length;

		return;
	}

	if(_kp_arg_remove(arg->arg,key))
		--arg->length;
}

bool kp_arg_update(KP_ARG *arg,char *key,char *value)
{
	return _kp_arg_update(arg->arg,key,value);
}

bool kp_arg_empty(KP_ARG *arg)
{
	if(arg->length)
		return true;
	else
		return false;
}

char *kp_arg_get_url(KP_VALUE *head)
{
	char *url=NULL;
	int len=0;
	int arg_len;

	while(head)
	{
		arg_len=len;
		len+=strlen(head->key)+strlen(head->value)+2;

		url=realloc(url,sizeof(char)*(len+1));
		if(url == NULL)
		{
			kp_errno=KP_ERROR_NO_MEM;
			return NULL;
		}

		snprintf(url+arg_len,sizeof(char)*(len+1-arg_len),"%s=%s&",head->key,head->value);
		head=head->next;
	}

	url[len-1]='\0';

	return url;
}

void _kp_arg_add(KP_VALUE *head,KP_VALUE *data)
{
	while(head->next)
		head=head->next;

	head->next=data;
}

bool _kp_arg_remove(KP_VALUE *head,char *key)
{
	KP_VALUE *temp;

	while(head->next)
	{
		if(strcmp(head->next->key,key) == 0)
			break;

		head=head->next;
	}

	if(head->next)
	{
		temp=head->next;
		head->next=head->next->next;
		free(temp->key);
		free(temp->value);
		free(temp);

		return true;
	}

	return false;
}

bool _kp_arg_update(KP_VALUE *head,char *key,char *value)
{
	while(head)
	{
		if(strcmp(head->key,key) == 0)
			break;

		head=head->next;
	}

	if(head)
	{
		free(head->value);
		head->value=strdup(value);
	}

	return false;
}

void _kp_arg_sort(KP_VALUE *head)
{
	KP_VALUE *p,temp;

	for(;head;head=head->next)
	{
		for(p=head->next;p;p=p->next)
		{
			if(strcmp(head->key,p->key) > 0)
			{
				temp.key=head->key;
				temp.value=head->value;

				head->key=p->key;
				head->value=p->value;

				p->key=temp.key;
				p->value=temp.value;
			}
		}
	}
}

bool _kp_arg_in(KP_VALUE *head,char *key)
{
	while(head)
	{
		if(strcmp(head->key,key) == 0)
			return true;

		head=head->next;
	}

	return false;
}
