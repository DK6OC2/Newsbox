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

function current_datetime() {

	$value = date('Y-m-d\TH:i:sp');
	return $value;
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


function generate_random_string($length = 5) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[random_int(0, $charactersLength - 1)];
    }
    return $randomString;
}

// CLIENT 
/* Database columns:
	macaddr
    callsign                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
    ov
	lastseen 
	created_at
*/
function client_check($mac_addr) {
	global $db;
	if(empty($db)) return false;

	// check if already registered
	$statement = $db->prepare('SELECT COUNT(macaddr) FROM clients WHERE macaddr = :macaddr');
	$statement->bindValue(':macaddr', strtolower($mac_addr), PDO::PARAM_STR);
	$statement->execute() or die(print_r($db->errorInfo(), true));

	//return $statement->rowCount();
	return $statement->fetchColumn();

}
function client_register($mac_addr) {
	global $db;
	if(empty($db)) return false;

	// if not, register
	if (TRUE) {
		$statement = $db->prepare('INSERT INTO clients(macaddr,passkey, lastseen,created_at) VALUES (:mac_addr, :passkey, :lastseen,:created_at)');
		$statement->bindValue(':mac_addr', strtolower($mac_addr), PDO::PARAM_STR);
		$statement->bindValue(':passkey', generate_random_string(), PDO::PARAM_STR);
		$statement->bindValue(':lastseen', NOW, PDO::PARAM_STR);
		$statement->bindValue(':created_at', NOW, PDO::PARAM_STR);
		$statement->execute();
	}
	//return $statement->rowCount();
	return $statement->fetchAll();

}
function client_update_lastseen($mac_addr) {
	global $db;
	if(empty($db)) return false;
	if (TRUE) {
		$statement = $db->prepare('UPDATE clients SET lastseen = '.NOW.' WHERE macaddr = ');
		$statement->bindValue(':macaddr', strtolower($mac_addr), PDO::PARAM_STR);
		
		$statement->bindValue(':created_at', NOW, PDO::PARAM_STR);
		$statement->execute();
	}
	return $statement->rowCount();
}

function client_update_values($arr_content, $mac_addr ) {
	global $db;
	if(empty($db)) return false;
	if (TRUE) {
		$statement = $db->prepare('UPDATE clients SET callsign = :callsign, ov = :ov WHERE macaddr = :macaddr');
		$statement->bindValue(':macaddr', strtolower($mac_addr), PDO::PARAM_STR);
		$statement->bindValue(':callsign', strtoupper($arr_content), PDO::PARAM_STR);
		$statement->bindValue(':ov', strtoupper($arr_content), PDO::PARAM_STR);
		$statement->execute();
	}
	return $statement->rowCount();
}

function client_get_passkey($mac_addr){
	global $db;
	if(empty($db)) return false;
	if (TRUE) {
		$statement = $db->prepare('SELECT passkey FROM clients WHERE macaddr = :macaddr');
		$statement->bindValue(':macaddr', strtolower($mac_addr), PDO::PARAM_STR);
		$statement->execute();
	}
	return $statement->fetch();
}

// MESSAGES
function msg_insert($arr_post, $timestamp=NOW) {
	global $db;
	if(empty($db)) return false;

	$statement = $db->prepare('INSERT INTO messages (line1, line2, line3, validfrom, created_at ) VALUES (:category, :1line, :2line, :3line, :4line, :validfrom_timestamp, :creation_timestamp)');
	$statement->bindValue(':line1', $arr_post['line1'], PDO::PARAM_INT);
	$statement->bindValue(':line2', $arr_post['line2'], PDO::PARAM_STR);
    $statement->bindValue(':line3', $arr_post['line3'], PDO::PARAM_STR);
    $statement->bindValue(':validfrom', $arr_post['validfrom'], PDO::PARAM_STR);
    $statement->bindValue(':created_at', NOW, PDO::PARAM_STR);
	$statement->execute();

    return $db->lastInsertId();
}
function msg_update($post_id, $arr_post, $timestamp=null) {
	global $db;
	if(empty($db)) return false;
	if(empty($content)) return false;
	if(!is_numeric($post_id) || $post_id <= 0) return false;

	if($timestamp !== null) {
		$statement = $db->prepare('UPDATE messages SET line1 = :line1, line2 = :line2, line3 = :line3,	modified_at = :modified_at WHERE id = :id');
		$statement->bindValue(':post_timestamp', $timestamp, PDO::PARAM_INT);
	} else {
		$statement = $db->prepare('UPDATE messages SET post_content = :post_content, post_edited = :post_edited WHERE id = :id');
	}
	$statement->bindValue(':id', $post_id, PDO::PARAM_INT);
	$statement->bindValue(':topic', $arr_post['topic'], PDO::PARAM_STR);
	$statement->bindValue(':line1', $arr_post['line1'], PDO::PARAM_STR);
	$statement->bindValue(':line2', $arr_post['line2'], PDO::PARAM_STR);
	$statement->bindValue(':line3', $arr_post['line3'], PDO::PARAM_STR);
	$statement->bindValue(':modified_at', time(), PDO::PARAM_STR);

	$statement->execute();

	return $statement->rowCount();
}
function msg_select_latest() {
    global $db;
	if(empty($db)) return false;

    $statement = $db->prepare('SELECT line1,line2,line3 FROM messages ORDER BY created_at DESC LIMIT 1');
	$statement->execute();
	$row = $statement->fetch(PDO::FETCH_ASSOC);

	return (!empty($row)) ? $row : false;

}

