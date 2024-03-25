<?php
	if(!defined('ROOT')) die('Don\'t call this directly.');

	/*
    if(!$config['logged_in']) {
		// wrong data, kick user to login page
		header('HTTP/1.0 401 Unauthorized');
		header('Location: '.$config['url'].'/login');
		die();
	}
    */

    $newmsg_saved = false; 

    if(!empty($_POST['line1'])) {
        $error_msg = "";
        $arr_content = array(
            'line1' => $_POST['line1'],
            'line2' => $_POST['line2'],
            'line3' => $_POST['line3'],
            'validfrom' => empty($_POST['validfrom'])?  "now" : $_POST['validfrom']
        );

        $new_id = msg_insert($arr_content);
        if ($new_id > 1) {
            $newmsg_saved = true;
        } else {
            $newmsg_saved = false;
        }

    } else {
        $error_msg = "The first line must have a content!";
    }

	$title_suffix = 'New Message';
	require(ROOT.DS.'templates'.DS.'header.inc.php');
?>

<body>
    <?php require(ROOT.DS.'templates'.DS.'nav.inc.php'); ?>
    <div class="container">
        <div class="row">
            <div class="col">
                <div class="card">
                    <div class="card-header">
                        Create a new message
                    </div>
                    <div class="card-body">
                        <h5 class="card-title"></h5>
                        <p class="card-text"></p>

                        <?php if(!empty($error_msg)): ?>
                            <div class="alert alert-danger" role="alert"><?= $error_msg ?></div>
                        <?php endif; ?>

                        <?php if($newmsg_saved): ?>
                            <div class="alert alert-success" role="alert">New message saved with ID: <?= $new_id ?></div>
                        <?php endif; ?>

                        <form action="" method="post" enctype="multipart/form-data" id="post-new-form"
                            data-redirect="<?= $config['url'] ?>">

                            <div class="form-check">
                                <input class="form-check-input" type="checkbox" value="" id="flexCheckDefault">
                                <label class="form-check-label" for="flexCheckDefault">
                                    Fake Topic
                                </label>
                            </div>

                            <div class="input-group mb-3">
                                <span class="input-group-text" id="line1">Line 1</span>
                                <input name="line1" type="text" maxlength="20" class="form-control"
                                    placeholder="required" aria-label="line1" aria-describedby="line1">
                            </div>

                            <div class="input-group mb-3">
                                <span class="input-group-text" id="line2">Line 2</span>
                                <input name="line2" type="text" maxlength="20" class="form-control"
                                    placeholder="optional" aria-label="line2" aria-describedby="line2">
                            </div>

                            <div class="input-group mb-3">
                                <span class="input-group-text" id="line3">Line 3</span>
                                <input name="line3" type="text" maxlength="20" class="form-control"
                                    placeholder="optional" aria-label="line3" aria-describedby="line3">
                            </div>

                            <!-- https://www.codeply.com/p/zU0EWDmIfn -->
                            <div class="input-group mb-3">
                                <span class="input-group-text" id="validFrom">Valid from</span>
                                <input id="validFrom" class="form-control" type="datetime-local" />
                                <span id="validFromSelected"></span>
                            </div>


                            <button type="submit" class="btn btn-success">Save</button>
                        </form>
                    </div>
                </div>

            </div>

            <?php require(ROOT.DS.'templates'.DS.'footer.inc.php'); ?>

            <script>
            $(function() {
                $('#datepicker').datepicker();
            });
            </script>
</body>

</html>