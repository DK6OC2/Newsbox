<?php
if (!defined('ROOT')) die('Don\'t call this directly.');

if (!$config['logged_in']) {
  // wrong data, kick user to login page
  header('HTTP/1.0 401 Unauthorized');
  header('Location: ' . $config['url'] . '/login');
  die();
}

$response = array('class' => '', 'text' => '');

if (!empty($_POST['line1']) &&  !empty($_POST['validfrom'])) {

  //built data array
  $arr_content = array(
    'line1' => $_POST['line1'],
    'line2' => $_POST['line2'],
    'line3' => $_POST['line3'],
    'validfrom' => $_POST['validfrom']
  );

  // insert into db
  $new_id = msg_insert($arr_content);
  if ($new_id > 1) {
    $response = array('class' => 'success', 'text' => 'New message saved with ID:' . $new_id);
    $title_suffix = 'Messages - New entry stored';
  } else {
    $response = array('class' => 'danger', 'text' => 'Something went wrong!');
    $title_suffix = 'Messages';
  }
} else {
  $response = array('class' => 'warning', 'text' => 'Line 1 is required!');
  $title_suffix = 'Messages';
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

            <?php if (is_array($response) && !empty($response)) : ?>
              <div class="alert alert-<?= $response['class'] ?>" role="alert"><?= $response['text'] ?></div>
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
                <button class="accordion-button" type="button" data-bs-toggle="collapse" data-bs-target="#collapse<?= $row['id'] ?>" 
                 aria-expanded="true" aria-controls=" collapse<?= $row['id'] ?>" 
                 <?= (strtotime($row['validfrom']) < strtotime('now')) ? 'style="background-color: lightgrey;"' : '' ?>
                 <?= (strtotime($row['validfrom']) > strtotime('now')) ? 'style="background-color: lightyellow;"' : '' ?>
                 >
                  #<?= $row['id'] ?> valid from: <?= $row['validfrom'] ?>
                </button>
              </h2>
              <div id="collapse<?= $row['id'] ?>" class="accordion-collapse collapse" aria-labelledby="heading<?= $row['id'] ?>" data-bs-parent="#accordionNewsbox">
                <div class="accordion-body">
                  <p style="font-family:monospace;">
                    <?= $row['line1'] ?><br>
                    <?= $row['line2'] ?><br>
                    <?= $row['line3'] ?><br>
                  </p>
                  <hr>
                  <a href="<?= $config['url_detected'] ?>/msg/delete/<?= $row['id'] ?>"><i class="bi bi-trash"></i></a> &nbsp;
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