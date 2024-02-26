<?php

function path($fragment=null) {
	global $config;
	if($fragment === null) return $config['path'];
	return (!empty($config['path'][$fragment])) ? $config['path'][$fragment] : false;
}

function get_host($preserve_port=false) {
	/* this makes a SERVER_NAME out of HTTP_HOST */

	$parsed = parse_url('http://'.$_SERVER['HTTP_HOST']); // scheme is only required for the parser

	if(!$preserve_port || empty($parsed['port'])) {
		return $parsed['host'];
	} else {
		return $parsed['host'].':'.$parsed['port'];
	}
}

//todo schreib den content in ein array

// BOX REGISTRATION
function newsbox_register($macaddr) {
	global $db;
	if(empty($db)) return false;

	// check if already registered
	$statement = $db->prepare('SELCT FROM ');
	statement->bindValue(':macaddr', $macaddr, PDO::PARAM_STR);
	$statement->execute();

	// if not, register
	$statement = $db->prepare('INSERT INTO messages() VALUES (:macaddr) ');
	statement->bindValue(':macaddr', $macaddr, PDO::PARAM_STR);
	$statement->execute();
}

// MESSAGES
function msg_insert($arr_msg, $timestamp=NOW) {
	global $db;
	if(empty($db)) return false;

	$statement = $db->prepare('INSERT INTO messages (category, 1line, 2line, 3line, 4line, validfrom_timestamp, creation_timestamp ) VALUES (:category, :1line, :2line, :3line, :4line, :validfrom_timestamp, :creation_timestamp)');
	$statement->bindValue(':category', $arr_msg['category'], PDO::PARAM_STR);
	$statement->bindValue(':1line', $arr_msg['1line'], PDO::PARAM_INT);
	$statement->bindValue(':2line', $arr_msg['2line'], PDO::PARAM_STR);
    $statement->bindValue(':3line', $arr_msg['3line'], PDO::PARAM_STR);
    $statement->bindValue(':4line', $arr_msg['4line'], PDO::PARAM_STR);
    $statement->bindValue(':validfrom_timestamp', $arr_msg['validfrom_timestamp'], PDO::PARAM_STR);
    $statement->bindValue(':creation_timestamp', $timestamp, PDO::PARAM_STR);
	$statement->execute();

    return $db->lastInsertId();
}

function msg_update($post_id, $content, $timestamp=null) {
	global $db;
	if(empty($db)) return false;
	if(empty($content)) return false;
	if(!is_numeric($post_id) || $post_id <= 0) return false;

	if($timestamp !== null) {
		$statement = $db->prepare('UPDATE messages SET post_content = :post_content, post_edited = :post_edited, post_timestamp = :post_timestamp WHERE id = :id');
		$statement->bindValue(':post_timestamp', $timestamp, PDO::PARAM_INT);
	} else {
		$statement = $db->prepare('UPDATE messages SET post_content = :post_content, post_edited = :post_edited WHERE id = :id');
	}
	$statement->bindValue(':id', $post_id, PDO::PARAM_INT);
	$statement->bindValue(':post_content', $content, PDO::PARAM_STR);
	$statement->bindValue(':post_edited', time(), PDO::PARAM_INT);

	$statement->execute();

	return $statement->rowCount();
}

function msg_select($id=0) {
    global $db;
	if(empty($db)) return false;
	if($id === 0) return false;

    $statement = $db->prepare('SELECT * FROM messages WHERE id = :id LIMIT 1');
	$statement->bindValue(':id', $id, PDO::PARAM_INT);
	$statement->execute();
	$row = $statement->fetch(PDO::FETCH_ASSOC);

	return (!empty($row)) ? $row : false;

}

// SETTINGS
function get_setting($key, $raw=false) {
	global $db;
	if(empty($db) || empty($key)) return false;

	$statement = $db->prepare('SELECT * FROM settings WHERE settings_key = :skey LIMIT 1');
	$statement->bindValue(':skey', $key, PDO::PARAM_STR);
	$statement->execute();
	$row = $statement->fetch(PDO::FETCH_ASSOC);

	if(!empty($row)) {
		if($raw) {
			return $row;
		} else {
			return $row['settings_value'];
		}
	}
	return false;
}

function set_setting($key, $value) {
	global $db;
	if(empty($db) || empty($key)) return false;

	try {
		$statement = $db->prepare('INSERT OR REPLACE INTO settings (settings_key, settings_value, settings_updated) VALUES (:skey, :svalue, :supdated)');

		$statement->bindValue(':skey', $key, PDO::PARAM_STR);
		$statement->bindValue(':svalue', $value, PDO::PARAM_STR);
		$statement->bindValue(':supdated', time(), PDO::PARAM_INT);

		$statement->execute();
	} catch(PDOException $e) {
		// print 'Exception : '.$e->getMessage();
		return false;
	}

	return true;
}
