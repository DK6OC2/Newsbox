<?php

$db_version = 0;

//connect or create the database
try {
	$db = new PDO('sqlite:'.ROOT.DS.'newsbox.db');
	$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);

	$db_version = $db->query("PRAGMA user_version")->fetch(PDO::FETCH_ASSOC)['user_version'];
} catch(PDOException $e) {
	print 'Exception : '.$e->getMessage();
	die(' - cannot connect to or open the database - ');
}

// first time setup
if($db_version == 0) {
	try {
		$db->exec("
			PRAGMA `user_version` = 1;
			
			CREATE TABLE IF NOT EXISTS `topics` (
				`id` INTEGER PRIMARY KEY NOT NULL,
				`name` TEXT NOT NULL,
				`remark` TEXT,
				`created_at` TEXT NOT NULL
				);
			INSERT INTO `topics` (name, remark, created_at) VALUES ('Test', 'This is a Test topic.', datetime('now'));
			INSERT INTO `topics` (name, remark, created_at) VALUES ('Default', 'This is a default topic.',datetime('now'));

			CREATE TABLE IF NOT EXISTS `messages` (
			`id` INTEGER PRIMARY KEY NOT NULL,
			`topic` TEXT,
			`subject` TEXT,
            `line1` TEXT,
            `line2` TEXT,
            `line3` TEXT,
			`validfrom` TEXT,
			`created_at` TEXT,
			`created_by` TEXT,
			`modified_at` TEXT,
			`modified_by` TEXT
		    );
			INSERT INTO `messages` (line1, line2, line3, validfrom, created_at) VALUES ('Test Eintrag', '2.Zeile............', '3. Zeile',datetime('now'), datetime('now'));
			INSERT INTO `messages` (line1, line2, line3, validfrom, created_at) VALUES ('Default Eintrag', '2.Zeile...........', '3. Zeile',datetime('now'), datetime('now'));

			CREATE TABLE IF NOT EXISTS `map_messages_topics` ( 
				`id` INTEGER PRIMARY KEY NOT NULL,
				`id_message` INTEGER NOT NULL,
				`id_topic` INTEGER NOT NULL,
				`created_at` TEXT
		    );
			INSERT INTO `map_messages_topics` (id_message,id_topic,created_at) VALUES (1,1, datetime('now'));
			INSERT INTO `map_messages_topics` (id_message,id_topic,created_at) VALUES (1,2, datetime('now'));

			CREATE TABLE IF NOT EXISTS `map_clients_topics` (
				`id` INTEGER PRIMARY KEY NOT NULL,
				`id_client` INTEGER NOT NULL,
				`id_topic` INTEGER NOT NULL,
				`created_at` TEXT
		    );

			CREATE TABLE IF NOT EXISTS `clients` (
                `id` INTEGER PRIMARY KEY NOT NULL,
                `macaddr` TEXT UNIQUE,
                `callsign` TEXT,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
                `ov` TEXT,
				`locator` TEXT,
				`passkey` TEXT,
				`lastseen` TEXT,
				`created_at` TEXT
			);
            CREATE UNIQUE INDEX `macaddr` ON clients (`macaddr`);
			CREATE UNIQUE INDEX `passkey` ON clients (`passkey`);
			INSERT INTO `clients` (macaddr,callsign,ov,locator,passkey,lastseen,created_at) VALUES ('123456789012','DL0ABC','R00','JO31KD','ABCD',datetime('now'), datetime('now'));


			CREATE TABLE IF NOT EXISTS `admins` (
                `id` INTEGER PRIMARY KEY NOT NULL,
                `callsign` TEXT,
				`passkey` TEXT,
				`email` TEXT,
                `ov` TEXT,
				`created_at` TEXT
			);

			CREATE TABLE IF NOT EXISTS `settings` (
				`id` INTEGER PRIMARY KEY NOT NULL,
				`settings_key` TEXT NOT NULL UNIQUE,
				`settings_value` TEXT,
				`settings_value_previous` TEXT,
				`settings_updated` INTEGER
			);
			CREATE UNIQUE INDEX `settings_keys` ON settings (`settings_key`);               
			INSERT INTO `settings` (settings_key, settings_value, settings_updated) VALUES ('install_signature', '".$install_signature."', strftime('%s'));
			INSERT INTO `settings` (settings_key, settings_value, settings_updated) VALUES ('install_date', datetime('now'), strftime('%s'));
			INSERT INTO `settings` (settings_key, settings_value, settings_updated) VALUES ('do_setup', '1', strftime('%s'));
			INSERT INTO `settings` (settings_key, settings_value, settings_updated) VALUES ('passkey', '', strftime('%s'));
           ");
		$db_version = 1;
	} catch(PDOException $e) {
		print 'Exception : '.$e->getMessage();
		die('cannot set up initial database table!');
	}
}
