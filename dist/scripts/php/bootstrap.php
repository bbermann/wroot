<?php

//echo '<pre>';

// Just a $_SERVER filter for which $env vars should be read
const SERVER_VARS = [
    'GATEWAY_INTERFACE', 'SERVER_ADDR', 'SERVER_NAME', 'SERVER_SOFTWARE', 
    'SERVER_PROTOCOL', 'REQUEST_METHOD', 'HTTP_METHOD', 'REQUEST_TIME', 
    'REQUEST_TIME_FLOAT', 'QUERY_STRING', 'DOCUMENT_ROOT', 'HTTP_ACCEPT', 
    'HTTP_ACCEPT_CHARSET', 'HTTP_ACCEPT_ENCODING', 'HTTP_ACCEPT_LANGUAGE', 
    'HTTP_CONNECTION', 'HTTP_HOST', 'HTTP_REFERER', 'HTTP_USER_AGENT', 
    'HTTPS', 'REMOTE_ADDR', 'REMOTE_HOST', 'REMOTE_PORT', 'REMOTE_USER', 
    'REDIRECT_REMOTE_USER', 'SCRIPT_FILENAME', 'SERVER_ADMIN', 'SERVER_PORT',
    'SERVER_SIGNATURE', 'PATH_TRANSLATED', 'SCRIPT_NAME', 'REQUEST_URI', 
    'PHP_AUTH_DIGEST', 'PHP_AUTH_USER', 'PHP_AUTH_PW', 'AUTH_TYPE', 
    'PATH_INFO', 'ORIG_PATH_INFO', 
];

function proper_parse_str($str) 
{
  # result array
  $arr = array();

  # split on outer delimiter
  $pairs = explode('&', $str);

  # loop through each pair
  foreach ($pairs as $i) {
    # split into name and value
    list($name,$value) = explode('=', $i, 2);
   
    # if name already exists
    if( isset($arr[$name]) ) {
      # stick multiple values into an array
      if( is_array($arr[$name]) ) {
        $arr[$name][] = $value;
      }
      else {
        $arr[$name] = array($arr[$name], $value);
      }
    }
    # otherwise, simply stick it in a scalar
    else {
      $arr[$name] = $value;
    }
  }

  # return result array
  return $arr;
}

function get_dolar_server(array $argv)
{
    // Needed in order to populate the $_SERVER array 
    // and subsequently the superglobals
    $env = getenv();
    
    $server = ['argv' => $argv];
    // Populate the server variables
    foreach (SERVER_VARS as $var) {    
        if (isset($env[$var])) {
            $server[$var] = $env[$var];
        }
    }
    return $server;
}

// This is needed in order to main parameters order 
// even when calling bootstrap.php (this file) 
// before the requested script.
$argv[0] = $requestedFile = $argv[1];
$argv[1] = $requestedUrl = $argv[2];
unset($argv[2]);

$_SERVER = get_dolar_server($argv);
//echo '$_SERVER => ';
//var_dump($_SERVER);

$method = $_SERVER['REQUEST_METHOD'];
$queryData = proper_parse_str($_SERVER['QUERY_STRING']);

switch ($method) {
    case 'GET':
        $_GET = $queryData;
        //echo '$_GET => ';
        //var_dump($_GET);
        break;

    case 'POST':
        $_POST = $queryData;
        //echo '$_POST => ';
        //var_dump($_POST);
        break;

    default: 
        break;
}

//echo '</pre>';

require $requestedFile;
?>