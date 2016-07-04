<?php

$timeout_log_filename = '/var/log/apache2/timeout.log';

function timeout_log($text) {

	global $timeout_log_filename;

	$fh = fopen($timeout_log_filename, "ab");
	fwrite($fh, $text);
	fclose($fh);
}

function dumper($arg, $depth = 10, $level = 0) {

	if($level > $depth) return "reach end of depth";

	if( is_string($arg) || is_float($arg) || is_int($arg) ) {
		return $arg;
	}

	if( is_array($arg) ) {

		$lines = array();
		foreach($arg as $key => $val) {
			$lines[] = str_repeat("  ", $level + 1).dumper($key, $depth, $level + 1)." => ".dumper($val, $depth, $level + 1);
		}
		return "array\n".implode("\n", $lines);
	}

	return "unknown";
}

function timeout_handler() {

	global $timeout_log_filename;

	$stack = debug_backtrace();

	$text = strftime("%Y-%m-%d %H:%M:%S\n", time());
	timeout_log("\n---------------------------------------------------------------------------\n\n");

	timeout_log(print_r($_SERVER, true));

	timeout_log(dumper($stack));

	chmod($timeout_log_filename, 0666);

	header("HTTP/1.0 504 Gateway timeout");
	echo "<!doctype html><html><head><title>504 Gateway timeout</title></head><body><br><br><br><center><h1>504 Gateway timeout</h1></center></body></html>";
	
	ob_flush(); 
	flush();
}

alarm("timeout_handler", 28, 0);
