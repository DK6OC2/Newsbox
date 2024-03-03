<?php
	if(!defined('ROOT')) die('Don\'t call this directly.');

	if(!$is_setup && !$config['logged_in']) {
		// wrong data, kick user to login page
		header('HTTP/1.0 401 Unauthorized');
		header('Location: '.$config['url'].'/login');
		die();
	}


	if($is_setup) {
		// allow the user in
		//$config['logged_in'] = check_login(true);

		$config = array_merge($config, $settings);
	}

	$message = array();
	if(isset($_POST['settings'])) {
		// todo: clean up values such as paths, etc?
		$settings_clean = $_POST['s'];

		if(!empty($settings_clean['admin_pass'])) {
			// update password
			$settings_clean['admin_pass'] = password_hash($settings_clean['admin_pass'], PASSWORD_DEFAULT);
		} else unset($settings_clean['admin_pass']);

		$update = [
			'key' => null,
			'value' => null,
			'updated' => time()
		];

		try {
			// backup all old values (todo: handle `admin_pass`)
			// does not work as INSERT replaces `previous` column with NULL
			//$statement = $db->prepare('UPDATE settings SET settings_value_previous = settings_value;');
			//$statement->execute();

			$statement = $db->prepare('INSERT OR REPLACE INTO settings (id, settings_key, settings_value, settings_updated) VALUES ((SELECT id FROM settings WHERE settings_key = :skey), :skey, :sval, :supdate);');

			$statement->bindParam(':skey', $update['key'], PDO::PARAM_STR);
			$statement->bindParam(':sval', $update['value'], PDO::PARAM_STR);
			$statement->bindParam(':supdate', $update['updated'], PDO::PARAM_INT);

			foreach ($settings_clean as $key => $value) {
				$update['key'] = mb_strtolower($key);
				$update['value'] = ($value !== '') ? $value : null;

				$statement->execute();
			}
		} catch(PDOException $e) {
			print 'Exception: Could not save settings. '.$e->getMessage();
			return false;
		}

		header('Location: '.$config['url'].'/'.$template);
		die();
	}

	$title_suffix = 'settings';

	require(ROOT.DS.'templates'.DS.'header.inc.php');

?>

<body>
    <div class="container">
        <div class="row">
            <div class="col"></div>
            <div class="col-6" style="background-color: #FFEBCD;">
                <?php if(isset($message['status']) && isset($message['message'])): ?>
                <p class="message <?= $message['status'] ?>"><?= $message['message'] ?></p>
                <?php endif; ?>

                <form action="" method="post" enctype="multipart/form-data" id="post-settings-form"
                    data-redirect="<?= $config['url_detected'] ?>/settings">
                    <?php if($is_setup): ?><input name="s[do_setup]" type="hidden" value="0"><?php endif; ?>

                    <div class="mb-3">
                        <?php if($is_setup): ?><div class="alert alert-warning" role="alert">First time setup! Please
                            make sure to choose a password!</div><?php endif; ?>
                    </div>
                    <div class="mb-3">
                        <label for="s-url" class="form-label">URL</label>
                        <input id="s-url" name="s[url]" type="text" class="form-control"
                            value="<?= $is_setup ? $config['url_detected'] : ($settings['url'] ?? '') ?>"
                            placeholder="The URL of your newsbox">
                    </div>
                    <div class="mb-3">
                        <label for="s-newsboxname" class="form-label">Name of your newsbox</label>
                        <input id="s-newsboxname" name="s[newsbox_name]" type="text" class="form-control"
                            value="<?= $settings['site_title'] ?? '' ?>"
                            placeholder="Set a name for your newsbox group.">
                    </div>
                    <div class="mb-3">
                        <label for="s-admin-user" class="form-label">Login username</label>
                        <input id="s-admin-user" name="s[admin_user]" type="text" class="form-control"
                            value="<?= $settings['admin_user'] ?? '' ?>" placeholder="Set your login username">
                    </div>
                    <div class="mb-3">
                        <label for="s-admin-pass" class="form-label">Login password</label>
                        <input id="s-admin-pass" name="s[admin_pass]" type="text" class="form-control"
                            value="<?= $is_setup ? '' : $settings['admin_pass'] ?>" placeholder="Set a login password">
                    </div>
                    <div class="mb-3">
                        <label for="s-admin-email" class="form-label">Recovery Email</label>
                        <input id="s-admin-email" name="s[admin_email]" type="text" class="form-control"
                            value="<?= $settings['admin_email'] ?? '' ?>"
                            placeholder="Set an email address for password recovery">
                    </div>
                    <div class="mb-3">
                        <input class="btn btn-primary" type="submit" name="settings" value="Save">
                    </div>

                    <div class="card" style="width: 100%;">
                        <div class="card-body">
                            <h5 class="card-title">Debug Info</h5>
                            <p class="card-text">Some infos about your installation.</p>
                            <fieldset>
                                <legend>Server</legend>
                                <dl>
                                    <dt><label>Detected URL</label></dt>
                                    <dd><?= $config['url_detected'] ?></dd>
                                    <dt><label>Path</label></dt>
                                    <dd><?= realpath(ROOT) ?></dd>
                                    <dt><label>Timezone</label></dt>
                                    <dd><?= date_default_timezone_get(); ?></dd>
                                    <dt><label>Timezone offset</label></dt>
                                    <dd><?= $config['local_time_offset'] ?></dd>
                                    <dt><label>Installed on domain root</label></dt>
                                    <dd><?= $config['subdir_install'] != 1 ? 'Yes' : 'No' ?></dd>

                                    <dt><label>Is SQLite supported ?</label></dt>
                                    <?php if (class_exists('SQLite3')) { echo 'Yes, SQLite3 extension loaded.'; } else { echo '- NO,it is not - Now, please install SQLite3.';} ?>
                                    <dt><label>Settings</label></dt>
                                    <dd><pre><?php print_r($settings); ?></pre></dd>
                                </dl>
                            </fieldset>

                            <p>&nbsp;</p>
                            <br>
                        </div>
                    </div>
                </form>
            </div>
            <div class="col"></div>
        </div>
    </div>

    <?php require(ROOT.DS.'templates'.DS.'footer.inc.php'); ?>
</body>

</html>