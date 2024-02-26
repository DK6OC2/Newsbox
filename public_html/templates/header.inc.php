<?php

	$title_suffix = isset($title_suffix) ? ' - ' . $title_suffix : '';

	header('Content-Type: text/html; charset=utf-8');

?>
<!DOCTYPE html>
<html>

<head>
    <title><?= empty($config['microblog_account']) ? "" : $config['microblog_account'] . "'s "; ?>Newsbox
        <?= $title_suffix ?></title>

    <meta charset="utf-8">
    <!--Let browser know website is optimized for mobile-->
    <meta name="viewport" content="width=device-width, initial-scale=1.0" >


    <!--Import Google Icon Font-->
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">

    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css"
        integrity="sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN" crossorigin="anonymous">

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.min.js"
        integrity="sha384-BBtl+eGJRgqQAUMxJ7pMwbEyER4l1g+O15P+16Ep7Q9Q+zqX6gSbd85u4mG4QzX+" crossorigin="anonymous">
    </script>

    <link rel="stylesheet" href="<?= $config['url_detected'] ?>/css/microblog.css" >
    <link rel="stylesheet" href="http://newsbox.local/css/microblog.css" >

</head>