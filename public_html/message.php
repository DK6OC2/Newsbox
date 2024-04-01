<?php


// Get MAC from URL
$mac_addr = path(1);

/**
 * Formats the result string for the client
 *
 * @param [array] $arr_msg
 * @return void
 */
function send_json_msg($arr_msg)
{
    header('Content-Type: application/json; charset=utf-8');
    $string_to_send = array(
        'messages' => $arr_msg
    );
    print json_encode($string_to_send);
}

// check if mac is missing and response
if (empty($mac_addr)) {
    $msg = array(
        'id'        => '0',
        'date'      => date('d.m.Y'),
        'topic'     => 'ERROR',
        'subject'   => 'ERROR',
        'line1'     => ' Sorry, but MAC',
        'line2'     => ' Addr is  missing.',
        'line3'     => ' Pse try again!'
    );
    send_json_msg($msg);
    exit;
}

// check if mac is 12 char long
if (strlen($mac_addr) !== 12) {
    $msg = array(
        'id'      => '0',
        'date'    => date('d.m.Y'),
        'topic'   => 'ERROR',
        'subject' => 'ERROR',
        'line1'   => ' Sorry, but ',
        'line2'   => substr($mac_addr, 0, 20),
        'line3'   => 'is not a mac addr!'
    );
    send_json_msg($msg);
    exit;
}

// check if mac is registered
if (client_check($mac_addr) == 1) {
    $clientStatus = "known";
    [$message] = msg_select_latest();
    send_json_msg($message);
} else {
    $clientStatus = "unknown";
    client_register($mac_addr);
    $replacements = array('line3' => 'Your PassKey:' . client_get_passkey($mac_addr)['passkey']);
    $msg = array_replace($msg_not_registered, $replacements);
    send_json_msg($msg);
}