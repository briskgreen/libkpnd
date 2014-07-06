#include "kp_data.h"

void print_arg(KP_VALUE *arg)
{
	while(arg)
	{
		printf("%s = %s\n",arg->key,arg->value);
		arg=arg->next;
	}
}

int main(void)
{
	KP_ARG *arg;

	arg=kp_arg_init();
	kp_arg_add(arg,"key2","this is key2");
	kp_arg_add(arg,"key4","this is key4");
	kp_arg_add(arg,"key3","this is key3");
	kp_arg_add(arg,"key1","this is key1");

	print_arg(arg->arg);
	printf("\n\n");
	kp_arg_sort(arg);
	print_arg(arg->arg);

	kp_arg_destroy(arg);

	return 0;
}
