<footer>
	<nav>
		<ul>
			<?php if($config['logged_in']): ?><li><a href="<?= $config['url_detected'] ?>/settings">Settings</a></li><?php endif; ?>
			<?php if($config['logged_in']): ?><li><a href="<?= $config['url_detected'] ?>/logout">Logout</a></li><?php else: ?><li><a href="<?= $config['url'] ?>/login">Login</a></li><?php endif; ?>
		</ul>
	</nav>
</footer>
<script>
</script>
