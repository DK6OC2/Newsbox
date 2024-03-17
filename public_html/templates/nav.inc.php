<nav class="navbar navbar-expand-lg navbar-light bg-light">
        <div class="container-fluid">
            <a class="navbar-brand" href="#">NewsBox</a>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse"
                data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false"
                aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse" id="navbarSupportedContent">
                <ul class="navbar-nav me-auto mb-2 mb-lg-0">



                    <li class="nav-item">
                        <?php if($config['logged_in']): ?>
                            <a href="<?= $config['url_detected'] ?>/logout" class="nav-link">Logout</a>
                        <?php else: ?>
                            <a href="<?= $config['url_detected'] ?>/login" class="nav-link">Login</a>
                        <?php endif; ?>
                    </li>

                    <li class="nav-item">
                    <?php if($config['logged_in']): ?>
                        <a href="<?= $config['url_detected'] ?>/settings" class="nav-link">Settings</a>
                    <?php endif; ?>
                    </li>

                    <?php if($config['logged_in']): ?>
                    <li class="nav-item">
                        <a href="<?= $config['url_detected'] ?>/msglist" class="nav-link">Messages</a>
                    </li>
                    <li class="nav-item">
                        <a href="<?= $config['url_detected'] ?>/msgnew" class="nav-link">New</a>
                    </li>
                    <?php endif; ?>

            </div>
        </div>
    </nav>