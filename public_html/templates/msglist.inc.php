<?php
if (!defined('ROOT')) die('Don\'t call this directly.');

if (!$config['logged_in']) {
  // wrong data, kick user to login page
  header('HTTP/1.0 401 Unauthorized');
  header('Location: ' . $config['url'] . '/login');
  die();
}

$title_suffix = 'Message List';
require(ROOT . DS . 'templates' . DS . 'header.inc.php');

?>

<body>
  <?php require(ROOT . DS . 'templates' . DS . 'nav.inc.php'); ?>

  <div class="container">
    <div class="row">
      <div class="col">
        <br>
        <div class="card">
          <div class="card-header">
            Create a new message
          </div>
          <div class="card-body">
            <h5 class="card-title"></h5>
            <p class="card-text"></p>

            <?php if (!empty($error_msg)) : ?>
              <div class="alert alert-danger" role="alert"><?= $error_msg ?></div>
            <?php endif; ?>

            <?php if (!empty($success_msg)) : ?>
              <div class="alert alert-success" role="alert"><?= $success_msg ?></div>
            <?php endif; ?>

            <form action="" method="post" enctype="multipart/form-data" id="post-new-form" data-redirect="<?= $config['url'] ?>">

              <div class="form-check">
                <input class="form-check-input" type="checkbox" value="" id="flexCheckDefault">
                <label class="form-check-label" for="flexCheckDefault">
                  Fake Topic
                </label>
              </div>

              <div class="input-group mb-3">
                <span class="input-group-text" id="line1">Line 1</span>
                <input name="line1" type="text" maxlength="20" class="form-control" placeholder="required" aria-label="line1" aria-describedby="line1">
              </div>

              <div class="input-group mb-3">
                <span class="input-group-text" id="line2">Line 2</span>
                <input name="line2" type="text" maxlength="20" class="form-control" placeholder="optional" aria-label="line2" aria-describedby="line2">
              </div>

              <div class="input-group mb-3">
                <span class="input-group-text" id="line3">Line 3</span>
                <input name="line3" type="text" maxlength="20" class="form-control" placeholder="optional" aria-label="line3" aria-describedby="line3">
              </div>

              <div class="input-group mb-3">
                <span class="input-group-text" id="validfrom">Valid from</span>
                <input name="validfrom" class="form-control" type="datetime-local">
              </div>

              <button type="submit" class="btn btn-success float-end">Save</button>
            </form>
          </div>
        </div>
      </div>
    </div>

    <br>

    <div class="row">
      <div class="col">
        <h2>List of Messages</h2>
        <div class="accordion" id="accordionNewsbox">

          <?php foreach (msg_list() as $row) : ?>
            <div class="accordion-item">
              <h2 class="accordion-header" id="heading<?= $row['id'] ?>">
                <button class="accordion-button" type="button" data-bs-toggle="collapse" data-bs-target="#collapse<?= $row['id'] ?>" aria-expanded="true" aria-controls="collapse<?= $row['id'] ?>">
                  #<?= $row['id'] ?> valid from: <?= $row['validfrom'] ?>
                </button>
              </h2>
              <div id="collapse<?= $row['id'] ?>" class="accordion-collapse collapse show" aria-labelledby="heading<?= $row['id'] ?>" data-bs-parent="#accordionNewsbox">
                <div class="accordion-body">
                  <?= $row['line1'] ?><br>
                  <?= $row['line2'] ?><br>
                  <?= $row['line3'] ?><br>
                  <hr>
                  <a href="<?= $config['url_detected'] ?>/msg/delete/<?= $row['id'] ?>"><i class="bi bi-trash"></i></a> &nbsp;
                  <a href="<?= $config['url_detected'] ?>/msg/update/<?= $row['id'] ?>"><i class="bi bi-pencil"></i></a>
                </div>
              </div>
            </div>
          <?php endforeach; ?>
        </div>
      </div>
    </div>

  </div>
  <?php require(ROOT . DS . 'templates' . DS . 'footer.inc.php'); ?>

  <script>

  </script>

</body>

</html>