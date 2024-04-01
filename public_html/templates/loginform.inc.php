<?php
	if(!defined('ROOT')) die('Don\'t call this directly.');

	// handle login
	if(isset($_POST['user']) && isset($_POST['pass'])) {
		if($_POST['user'] === $config['admin_user'] && password_verify($_POST['pass'], $config['admin_pass'])) {
			$host = get_host(false); // cookies are port-agnostic
			$domain = ($host != 'localhost') ? $host : false;
			$hash = hash('sha256', $config['install_signature']);
			setcookie('newsbox', $hash, NOW_UNIX+$config['cookie_life'], '/', $domain, false);
			header('Location: '.$config['url'].DS.'dashboard');
			die();
		} else {
			header('HTTP/1.0 401 Unauthorized');
			$response = array(
				'status' => 'danger',
				'message' => 'You entered wrong user credentials. Please try again.'
			);
		}
	}

	$title_suffix = 'login';
	require(ROOT.DS.'templates'.DS.'header.inc.php');

?>

<body>
    <?php require(ROOT.DS.'templates'.DS.'nav.inc.php'); ?>
    <div class="container">
        <div class="row">
            <div class="col">

                <?php if(path(1) == 'recovery'): ?>
                <div role="alert" class="alert alert-success">A recovery link has been sent to your email address.</div>
                <?php endif; ?>

                <?php if(isset($response['status']) && isset($response['message'])): ?>
                <div role="alert" class="alert alert-<?= $response['status'] ?>"><?= $response['message'] ?></p>
                <?php endif; ?>

                <form action="" method="post">

                    <label for="user" class="form-label">Username</label>
                    <input type="text" name="user" class="form-control" required>

                    <label for="passwd" class="form-label">Password</label>
                    <input type="password" name="pass" id="passwd" class="form-control" required>
					<br>

                    <div class="login-nav">
                        <input type="submit" name="" value="Login" class="btn btn-success">
                        <a href="/recovery" class="btn btn-warning">Forgot password</a>
                    </div>
                </form>

            </div>
        </div>
        <?php require(ROOT.DS.'templates'.DS.'footer.inc.php'); ?>

    <script>
    </script>
</body>
</html>