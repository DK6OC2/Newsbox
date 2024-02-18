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
		$config['logged_in'] = check_login(true);

		// generate some values
		if(empty($settings['app_token'])) {
			$settings['app_token'] = bin2hex(random_bytes(16));
		}
		if(empty($settings['admin_pass'])) {
			$settings['admin_pass'] = suggest_password();
		}

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
	require(ROOT.DS.'snippets'.DS.'header.snippet.php');

?><body ontouchstart="">
	<div class="wrap">
		<?php require(ROOT.DS.'snippets'.DS.'nav.snippet.php'); ?>
		<?php if(isset($message['status']) && isset($message['message'])): ?>
		<p class="message <?= $message['status'] ?>"><?= $message['message'] ?></p>
		<?php endif; ?>
		<form action="" method="post" enctype="multipart/form-data" id="post-settings-form" data-redirect="<?= $config['url'] ?>/settings">
			<?php if($is_setup): ?><p class="message">First time setup! Please make sure to choose a password!</p><?php endif; ?>
			<fieldset>
				<legend>General</legend>

				<?php if($is_setup): ?><input name="s[do_setup]" type="hidden" value="0" /><?php endif; ?>

				<dl>
					<dt><label for="s-url">URL</label></dt>
					<dd><input id="s-url" name="s[url]" type="text" value="<?= $is_setup ? $config['url_detected'] : ($settings['url'] ?? '') ?>" placeholder="The URL of your microblog" /></dd>

					<!--<dt><label for="s-root">Path (auto-detect if empty)</label></dt>
					<dd><input id="s-root" name="s[root]" type="text" value="<?= $settings['root'] ?? '' ?>" placeholder="<?= realpath(ROOT) ?>" readonly /></dd>-->

					<dt><label for="s-admin-user">Login username</label></dt>
					<dd><input id="s-admin-user" name="s[admin_user]" type="text" value="<?= $settings['admin_user'] ?? '' ?>" placeholder="Set a login username" /></dd>

					<dt><label for="s-admin-pass">Login password</label></dt>
					<dd><input id="s-admin-pass" name="s[admin_pass]" type="text" value="<?= $is_setup ? $settings['admin_pass'] : '' ?>" placeholder="Set a login password" autocomplete="new-password" /></dd>

					<?php if(!$is_setup): ?><dt><label for="s-admin-passkey">Login Passkey</label></dt>
					<dd><?php if(empty($config['passkey'])): ?><button class="button hidden" id="passkey-create">Create Passkey</button><?php endif; ?><span id="passkey-status"><?= !empty($config['passkey']) ? 'Passkey is set (<a href="/pk/revoke" id="passkey-revoke">revoke</a>)' : 'No passkey set' ?></span></dd><?php endif; ?>

					<dt><label for="s-app-token">App Token</label></dt>
					<dd><input id="s-app-token" name="s[app_token]" type="text" value="<?= $settings['app_token'] ?? '' ?>" placeholder="A seperate password used for XMLRPC" /></dd>

					<dt><label for="s-admin-email">Recovery Email</label></dt>
					<dd><input id="s-admin-email" name="s[admin_email]" type="text" value="<?= $settings['admin_email'] ?? '' ?>" placeholder="Set an email address for password recovery" /></dd>
				</dl>
			</fieldset>

			<fieldset>
				<legend>Configuration</legend>

				<dl>
					
					

					
					<dt><label for="s-local_timezone">Local Time Zone</label></dt>
					<dd><select id="s-local_timezone" name="s[local_timezone]"><?php 
					$timezones= DateTimeZone::listIdentifiers();
					foreach ($timezones as $timezone) {
							$active = (($settings['local_timezone'] ?? '') == $timezone) ? ' selected' : '';
							echo('<option value="'.$timezone.'"'.$active.'>'.$timezone.'</option>'.NL);
					}
					?></select>
					</dd>


				</dl>
			</fieldset>

			

			

			<?php if(function_exists('bar_list')): ?><fieldset id="archive">
				<legend>Export</legend>
				<dl>
					<dt><label >Archives</label></dt>
					<dd>
						<?php
							$archives = bar_list();

							//var_dump($archives);
							//var_dump($settings);
							if(empty($archives)) {
								echo('<p>No archived versions</p>');
							}
						?>
						<ul><?php foreach($archives as $a): ?>
						<?php
							$abn = basename($a);
							list($_, $ts) = explode('-', pathinfo($abn, PATHINFO_FILENAME));
							$nice_name = date('Y-m-d H:i:s', $ts);
							$dl_name = 'mb-'.date('Y-m-d', $ts).'.bar';
						?><li><a href="<?= $config['url'].'/files/bar/'.$abn ?>" download="<?= $dl_name ?>"><?= $nice_name ?></a> <a href="<?= $config['url'].'/bar/?delete='.$abn ?>" onclick="return confirm('Really delete archive <?= $nice_name ?>?')">(Delete)</a></li>
						<?php endforeach; ?></ul>
					</dd>

					<dt><label for="s-bar-create"></label></dt>
					<dd><a href="<?= $settings['url'] ?>/bar" class="button secondary">Create new archive</a></dd>
			</fieldset><?php endif; ?>

			<div class="post-nav">
				<input type="submit" name="settings" value="Save" class="button" />
			</div>

			<details>
				<summary>Debug info</summary>
			<fieldset>
				<legend>Server</legend>
				<dl>
					<dt><label>Detected URL</label></dt>
					<dd><?= $config['url_detected'] ?></dd>

					<dt><label>Path</label></dt>
					<dd><?= realpath(ROOT) ?></dd>

					<dt><label>Timezone offset</label></dt>
					<dd><?= $config['local_time_offset'] ?></dd>



					<dt><label>Installed on domain root</label></dt>
					<dd><?= $config['subdir_install'] != 1 ? 'Yes' : 'No' ?></dd>

				</dl>
			</fieldset>
			</details>
		</form>
	</div>
	<?php require(ROOT.DS.'snippets'.DS.'footer.snippet.php'); ?>
</body>
</html>