<?php

//PHP settings
error_reporting(E_ALL);
ini_set('display_errors', 1);
date_default_timezone_set('Europe/Berlin');

// Definitions
DEFINE('ROOT', __DIR__);
DEFINE('DS', DIRECTORY_SEPARATOR);
DEFINE('NOW', date("c")); // ISO8601 Format

/* characters for PassKey */
$characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';

// set up database
require_once(ROOT.DS.'lib'.DS.'database.php');

/* make the path easier to read */
$dir = dirname($_SERVER['SCRIPT_NAME']);
$uri = $_SERVER['REQUEST_URI'];
$uri = substr($uri, mb_strlen($dir)); // handle subdir installs
$path_fragments = parse_url($uri, PHP_URL_PATH);
$path = (empty($path_fragments)) ? [''] : explode('/', trim($path_fragments, '/'));
if(mb_strlen($path[0]) == 0) { $path = []; }

/* load settings */
$statement = $db->prepare('SELECT * FROM settings');
$statement->execute();
$settings_raw = $statement->fetchAll(PDO::FETCH_ASSOC);

if(!empty($settings_raw)) {
	// create config array
	$settings = array_column($settings_raw, 'settings_value', 'settings_key');
} else {
	// there were no settings in the DB. initialize!
	$settings = [];
}

$default_settings = array(
	'url' => '',
	'path' => __DIR__,
	'language' => 'en',
	'max_characters' => 280,
	'posts_per_page' => 10,
	'theme' => 'plain',
	'microblog_account' => '',
	'site_title' => 'Another Microblog',
	'site_claim' => 'This is an automated account. Don\'t mention or reply please.',
	'admin_user' => 'admin',
	'admin_pass' => 'admin',
	'app_token' => '',
	'cookie_life' => 60*60*24*7*4,
	'ping' => true,
	'activitypub' => true,
	'show_edits' => true
);

$old_config = $config ?? [];
$config = $settings;
$config['path'] = $path;
$config['url_detected'] = 'http'.(!empty($_SERVER['HTTPS']) ? 's' : '').'://'.$_SERVER['HTTP_HOST'].rtrim($dir, '/');
$config['subdir_install'] = ($dir === '/') ? false : true;
$config['local_time_offset'] = date('P');

unset($dir, $uri, $path_fragments, $path);

// not registered message 
$msg_not_registered = array(
	'Topic'	 => 'Anmeldung',
	'Zeile1' => 'Box ist unbekannt',
	'Zeile2' => 'Registrierung ',
	'Zeile3' => 'Dein PassKey: 12345'
);


// load functions
require_once(ROOT.DS.'lib'.DS.'functions.php');
