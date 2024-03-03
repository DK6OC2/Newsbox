<?php 

//debug
//echo client_check(path(1));
//echo current_datetime(); 
//echo NOW; 

function send_msg($arr_msg){
    header('Content-Type: application/json; charset=utf-8');
    print json_encode($arr_msg);
}

// set variable
$mac_addr = path(1);

// check if mac is missing
if (empty($mac_addr)) {
    $msg = array(
        'Topic'	 => 'E409',
        'Zeile1' => ' - E R R O R - ',
        'Zeile2' => ' Missing Addr ',
    );
    send_msg($msg);
    exit;
}

// check if max is 12 char
if (strlen($mac_addr) !== 12 ){
    $msg = array(
        'Topic'	 => 'E409',
        'Zeile1' => ' - E R R O R - ',
        'Zeile2' => substr($mac_addr, 0, 20),
        'Zeile3' => 'is not a mac addr!'
    );
    send_msg($msg);
    exit;
}

// check if mac is registered
if ( client_check($mac_addr) == 1 ) {
    $clientStatus = "known";
    send_msg(msg_select_latest());

} else {
    $clientStatus = "unknown";
   
    $replacements = array('Zeile3'=> 'Dein PassKey:' . client_get_passkey($mac_addr)['passkey'] );
    $msg = array_replace($msg_not_registered,$replacements);
    send_msg($msg);
}
