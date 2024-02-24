<footer>
	<nav>
		<ul>
			<?php if($config['logged_in']): ?><li><a href="<?= $config['url'] ?>/settings">Settings</a></li><?php endif; ?>
			<?php if($config['logged_in']): ?><li><a href="<?= $config['url'] ?>/logout">Logout</a></li><?php else: ?><li><a href="<?= $config['url'] ?>/login">Login</a></li><?php endif; ?>
		</ul>
	</nav>
</footer>
<script>
	window.mb = {
		'url': new URL('<?= $config['url'] ?>'),
		'theme': {
			'name': '<?= $config['theme'] ?>',
			'script': null
		}
	};
	// mostly used for passkey management
</script>
