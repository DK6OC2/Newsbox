<?php 

//debug
//echo client_check(path(1));
//echo current_datetime(); 
//echo NOW; 

function send_msg($arr_msg, $id=9999){
    header('Content-Type: application/json; charset=utf-8');
    
    $string_to_send = array (
        'ID' => $id,
        'date' =>  date("d.m.Y"), 
        'message' => $arr_msg
    );
    print json_encode($string_to_send);
}

// check if mac is missing
if (empty($mac_addr)) {
    $msg = array(
        'message' => array (
        'topic'	 => 'E409',
        'line1' => ' - E R R O R - ',
        'line2' => ' Missing Addr ',
        'line3' => ''
        )
    );
    send_msg($msg, 9999 /* muss noch die ID rein */);
    exit;
}

// check if max is 12 char
if (strlen($mac_addr) !== 12 ){
    $msg = array(
        'message' => array (
            'topic'	 => 'E409',
            'line1' => ' - E R R O R - ',
            'line2' => substr($mac_addr, 0, 20),
            'line3' => 'is not a mac addr!'
        )
    );
    send_msg($msg, 9999);
    exit;
}

// check if mac is registered
if ( client_check($mac_addr) == 1 ) {
    $clientStatus = "known";
    [$message, $id] = msg_select_latest();
    send_msg($message, $id /* muss noch die ID rein */);
    
} else {
    $clientStatus = "unknown";
    client_register($mac_addr);
    $replacements = array('line3'=> 'Your PassKey:' . client_get_passkey($mac_addr)['passkey'] );
    $msg = array_replace($msg_not_registered,$replacements);
    send_msg($msg, 8999 /* muss noch die ID rein */);
}