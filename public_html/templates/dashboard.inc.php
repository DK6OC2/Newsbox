<?php
if (!defined('ROOT')) die('Don\'t call this directly.');

if (!$config['logged_in']) {
  // wrong data, kick user to login page
  header('HTTP/1.0 401 Unauthorized');
  header('Location: ' . $config['url'] . '/login');
  die();
}

$response = array('class' => '', 'text' => '');

$title_suffix = 'Dashboard';
require(ROOT . DS . 'templates' . DS . 'header.inc.php');

?>
<body>
    <?php require(ROOT . DS . 'templates' . DS . 'nav.inc.php'); ?>
    <div class="container">
        <br>
        <h2>Dashboard</h2>
        <div class="row">
            <div class="col">
            </div>
        </div>
    </div>


</body>
</html>