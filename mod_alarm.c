
#include "php.h"
#include <malloc.h>

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

static int __timer_started = 0;
int mod_alarm_shutdown(int type, int module_number TSRMLS_DC) {
	// dprintf(2, "mod_alarm: mod_alarm_shutdown\n"); fflush(stderr);
	if(__timer_started) {
		// dprintf(2, "mod_alarm: mod_alarm_shutdown clear alarm\n"); fflush(stderr);
		__timer_started = 0;

		struct itimerval new_value, old_value;

		new_value.it_value.tv_sec	= 0;
		new_value.it_value.tv_usec	= 0;
		new_value.it_interval.tv_sec	= 0;
		new_value.it_interval.tv_usec	= 0;

		if(setitimer(ITIMER_REAL, &new_value, &old_value) < 0) {
			// dprintf(2, "mod_alarm: mod_alarm_shutdown setitimer error\n"); fflush(stderr);
		}
	}
	return 0;
}

ZEND_FUNCTION( alarm );

zend_function_entry mod_alarm_functions[] =
{
	ZEND_FE(alarm, NULL)

	{NULL, NULL, NULL}
};

zend_module_entry mod_alarm_module_entry =
{
	STANDARD_MODULE_HEADER,
	"mod_alarm",
	mod_alarm_functions,
	NULL,
	mod_alarm_shutdown,
	NULL,
	mod_alarm_shutdown,
	NULL,
	NO_VERSION_YET,
	STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE( mod_alarm )

char __alarm_param[1024];

void __alarm_func(int signo) {

	zval funcname;
	zval retval;

	// dprintf(2, "mod_alarm: __alarm_func\n"); fflush(stderr);

	ZVAL_STRING(&funcname, __alarm_param, 0);
	zend_first_try {
		if(call_user_function(EG(function_table), NULL, &funcname, &retval, 0, NULL TSRMLS_CC) == FAILURE) {
			// dprintf(2, "mod_alarm: __alarm_func call_user_function error\n"); fflush(stderr);
		}
		else {
			exit(0);
		}
	} zend_catch {
		// dprintf(2, "mod_alarm: __alarm_func call_user_function catche\n"); fflush(stderr);
	} zend_end_try();	
}

ZEND_FUNCTION( alarm )
{
	int name_len;
	char *name;
	long seconds, useconds;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"sll", &name, &name_len, &seconds, &useconds) == FAILURE) {
		RETURN_LONG(1);
        	return;
	}

	struct itimerval new_value, old_value;

	new_value.it_value.tv_sec	= seconds;
	new_value.it_value.tv_usec	= useconds;
	new_value.it_interval.tv_sec	= 0;
	new_value.it_interval.tv_usec	= 0;

	if(setitimer(ITIMER_REAL, &new_value, &old_value) < 0) {
		RETURN_LONG(1);
		return;
	}

	strcpy(__alarm_param, name);
	signal( SIGALRM, __alarm_func );

	__timer_started = 1;

	RETURN_LONG(0);
}
