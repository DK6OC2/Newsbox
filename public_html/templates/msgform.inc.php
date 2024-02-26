<?php
	if(!defined('ROOT')) die('Don\'t call this directly.');



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

                        <form action="" method="post">
                        <div class="form-check">
                            <input class="form-check-input" type="checkbox" value="" id="flexCheckDefault">
                            <label class="form-check-label" for="flexCheckDefault">
                                NewsTopic List
                            </label>
                        </div>

                        <div class="input-group mb-3">
                            <span class="input-group-text" id="2ndLine">2nd Line</span>
                            <input name="2ndLine" type="text" maxlength="20" class="form-control"
                                placeholder="Text zweite Zeile" aria-label="2nd Line" aria-describedby="2ndLine">
                        </div>

                        <div class="input-group mb-3">
                            <span class="input-group-text" id="3rdLine">3rd Line</span>
                            <input name="3rdLine" type="text" maxlength="20" class="form-control"
                                placeholder="Text dritte Zeile" aria-label="3rd Line" aria-describedby="3rdLine">
                        </div>

                        <div class="input-group mb-3">
                            <span class="input-group-text" id="4thLine">4th Line</span>
                            <input name="4thLine" type="text" maxlength="20" class="form-control"
                                placeholder="Text vierte Zeile" aria-label="4th Line" aria-describedby="4thLine">
                        </div>

                    
                        <button type="submit" class="btn btn-success">Save</button>
                        </form>
                    </div>
                </div>

            </div>
            <div class="col"></div>

            <?php require(ROOT.DS.'templates'.DS.'footer.inc.php'); ?>
</body>

</html>