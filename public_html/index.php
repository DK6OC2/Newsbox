<?php

    // create config file
	$config_file = __DIR__.DIRECTORY_SEPARATOR.'config.php';
	$config_file_default = __DIR__.DIRECTORY_SEPARATOR.'config-dist.php';
	if(!include_once($config_file)) {
		if(file_exists($config_file_default)) {
			copy($config_file_default, $config_file);
			chmod($config_file, 0644);
			header('Refresh:1');
			exit();
		}
	}

    // check if we are running for the first time
	//print_r($settings);
	$is_setup = (isset($settings) && !empty($settings['do_setup']) && $settings['do_setup'] == 1) ? true : false;
	if($is_setup && path(0) !== 'settings') {
		// first time setup
		header('Location: '.$config['url_detected'].'/settings');
		echo $config['url_detected'];
		echo path(1);
		die();
	}

	$page = mb_strtolower(path(0));
	switch($page) {
		case 'message':
			require_once(ROOT.DS.'displaymessage.php');
			break;
		case 'login':
			$template = 'login';
			require_once(ROOT.DS.'templates'.DS.'loginform.inc.php');
			break;
		case 'logout':
			$host = get_host(false); // cookies are port-agnostic
			$domain = ($host != 'localhost') ? $host : false;
			setcookie('microblog_login', '', time()-3600, '/', $domain, false);
			unset($_COOKIE['microblog_login']);

			header('Location: '.$config['url']);
			break;
		case 'new':
			$template = 'postform';
			require_once(ROOT.DS.'templates'.DS.'msgform.inc.php');
			break;
		case 'settings':
			$template = 'settings';
			require_once(ROOT.DS.'templates'.DS.'settings.inc.php');
			break;
		case 'pk':
			require_once(ROOT.DS.'lib'.DS.'passkeys.php');
			break;
		case 'recovery':
			// password recovery via email
			if(!empty($config['admin_email'])) {

				if(!empty(path(1))) {
					// get magic link
					list('settings_value' => $bytes, 'settings_updated' => $age) = get_setting('magic_url', true);
					if(empty($bytes)) exit('Invalid URL');

					// validate
					if(path(1) === $bytes) {

						// check link age (valid for 1h)
						if($age > NOW - 3600) {
							$config['logged_in'] = check_login(true); // force entry!

							header('Location: '.$config['url'].'/settings');
							exit('Success');
						}

						exit('Link has expired');
					} else {
						exit('Invalid URL');
					}
				} else {
					// send a recovery email with link
					$bytes = bin2hex(random_bytes(16));
					$magic_link = $config['url'].'/recovery/'.$bytes;

					db_set_setting('magic_url', $bytes);

					$mailtext  = 'Your recovery link for Microblog:'.NL;
					$mailtext .= $magic_link.NL;

					$host = parse_url($config['url'], PHP_URL_HOST);
					$headers = array(
						'From' => 'admin@'.$host,
						'Reply-To' => 'admin@'.$host,
						'X-Mailer' => 'PHP/' . phpversion()
					);

					if(mail(trim($config['admin_email']), 'Your Microblog recovery link', $mailtext, $headers)) {
						// var_dump($mailtext);
						header('Location: '.$config['url'].'/login/recovery');
					} else {
						exit('Could not send email with recovery link!');
					}
				}
			}

			break;
		default:
			// redirect everything else to the homepage
			if(!empty(path(0)) && path(0) != 'settings') {
				// die(path(0) . path(1) . 'WTF');
				header('Location: '.$config['url']);
				die();
			}
			echo "This is the NewsBox System";		
			break;
	}