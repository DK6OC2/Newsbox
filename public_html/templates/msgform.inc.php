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

    if(!empty($_POST['line1'])) {

        $arr_content = array(
            'line1' => $_POST['line1'],
            'line2' => $_POST['line2'],
            'line3' => $_POST['line3']
        );
        $id = msg_insert($arr_content, NOW);
        echo $id;

    } else {
        $error_msg = "Die erste Zeile muss einen Inhalt haben!";
        echo $error_msg;
    }

	$title_suffix = 'New Message';
	require(ROOT.DS.'templates'.DS.'header.inc.php');
?>

<body ontouchstart="">

    <div class="container">
        <div class="row">
            <div class="col"></div>
            <div class="col-6">
                <div class="card">
                    <div class="card-header">
                        Create a new message
                    </div>
                    <div class="card-body">
                        <h5 class="card-title"></h5>
                        <p class="card-text"></p>

                        <form action="" method="post" enctype="multipart/form-data" id="post-new-form"
                            data-redirect="<?= $config['url'] ?>">
                            <div class="form-check">
                                <input class="form-check-input" type="checkbox" value="" id="flexCheckDefault">
                                <label class="form-check-label" for="flexCheckDefault">
                                    NewsTopic List
                                </label>
                            </div>

                            <div class="input-group mb-3">
                                <span class="input-group-text" id="line1">1st Line</span>
                                <input name="line1" type="text" maxlength="20" class="form-control"
                                    placeholder="Erste Zeile ist pflicht" aria-label="line1" aria-describedby="line1">
                            </div>

                            <div class="input-group mb-3">
                                <span class="input-group-text" id="line2">2nd Line</span>
                                <input name="line2" type="text" maxlength="20" class="form-control"
                                    placeholder="Zweite Zeile optional" aria-label="line2" aria-describedby="line2">
                            </div>

                            <div class="input-group mb-3">
                                <span class="input-group-text" id="line3">3rd Line</span>
                                <input name="line3" type="text" maxlength="20" class="form-control"
                                    placeholder="Dritte Zeile optional" aria-label="line3" aria-describedby="line3">
                            </div>

                            <label for="date" class="col-1 col-form-label">Date</label>
                            <div class="col-5">
                                <div class="input-group date" id="datepicker">
                                    <input type="text" class="form-control" id="date" />
                                    <span class="input-group-append">
                                        <span class="input-group-text bg-light d-block">
                                            <i class="fa fa-calendar"></i>
                                        </span>
                                    </span>
                                </div>
                            </div>

                            <button type="submit" class="btn btn-success">Save</button>
                        </form>
                    </div>
                </div>

            </div>
            <div class="col"></div>

            <?php require(ROOT.DS.'templates'.DS.'footer.inc.php'); ?>

            </body>
<script>
    $(function(){
  $('#datepicker').datepicker();
});
</script>
     



</html>