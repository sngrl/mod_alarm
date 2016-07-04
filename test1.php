<?php

error_reporting(E_ALL);
ini_set('error_reporting', E_ALL);
ini_set('display_errors', 'yes');

function alarm_func() {

	$alarms = debug_backtrace();
	echo "<pre>";
	print_r($alarms);
	echo "</pre>";

	ob_flush(); 
	flush();
}

/*
 * params:
 *  alarm_func_name
 *  second
 *  micro second
 */

alarm("alarm_func", 1, 0);

mysql_connect('example.ru:3306', 'user', 'pass', true);

function trololo($b) {

	mysql_query("select sleep(2)");
	mysql_query("select sleep(2)");
	mysql_query("select sleep(2)");

 	sleep(5);
 	sleep(5);
 	sleep(5);
 	sleep(5);
}

trololo(16);
echo time()."<br>";
echo "1<br>";
