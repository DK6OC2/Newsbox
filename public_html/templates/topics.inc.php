<?php
if (!defined('ROOT')) die('Don\'t call this directly.');

if (!$config['logged_in']) {
    // wrong data, kick user to login page
    header('HTTP/1.0 401 Unauthorized');
    header('Location: ' . $config['url'] . '/login');
    die();
}

$title_suffix = 'Topics';
require(ROOT . DS . 'templates' . DS . 'header.inc.php');

?>


<body>
    <?php require(ROOT . DS . 'templates' . DS . 'nav.inc.php'); ?>
    <div class="container">
        <br>
        <div class="row">
            <div class="col">
                <div class="card">
                    <div class="card-header">
                        Create a topic
                    </div>
                    <div class="card-body">

                        <?php if (!empty($error_msg)) : ?>
                            <div class="alert alert-danger" role="alert"><?= $error_msg ?></div>
                        <?php endif; ?>

                        <?php if (!empty($success_msg)) : ?>
                            <div class="alert alert-success" role="alert"><?= $success_msg ?></div>
                        <?php endif; ?>

                        <form action="" method="post" enctype="multipart/form-data" id="post-new-form" data-redirect="<?= $config['url'] ?>">

                            <div class="input-group mb-3">
                                <span class="input-group-text" id="topic">Name</span>
                                <input name="topic" type="text" maxlength="9" class="form-control" placeholder="required" aria-label="topic" aria-describedby="topic">
                            </div>

                            <div class="input-group mb-3">
                                <span class="input-group-text" id="topic">Remark</span>
                                <input name="remark" type="text" class="form-control" placeholder="" aria-label="remark" aria-describedby="remark">
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
                <div class="card">
                    <div class="card-header">
                        List of topics
                    </div>
                    <div class="card-body">
                        <table class="table">
                            <thead>
                                <tr>
                                    <th scope="col">#</th>
                                    <th scope="col">Name</th>
                                    <th scope="col">Remark</th>
                                    <th scope="col">Actions</th>
                                </tr>
                            </thead>
                            <tbody>
                                <?php

                                // Get Topics
                                $rows = topic_list();

                                // Display Topics
                                $html = "";
                                foreach ($rows as $row) {
                                    $html .= '<tr>';
                                    $html .= '<th scope="row">' . $row['id'] . '</th>';
                                    $html .= '<td>' . htmlspecialchars($row['name']) . '</td>';
                                    $html .= '<td>' . htmlspecialchars($row['remark']) . '</td>';
                                    $html .= '<td>
                                            <a href="' . $config['url_detected'] . '/topics/delete/' . $row['id'] . '"><i class="bi bi-trash"></i></a>
                                            <a href="' . $config['url_detected'] . '/topics/update/' . $row['id'] . '"><i class="bi bi-pencil"></i></a> </td>';
                                    $html .= '</tr>';
                                }
                                echo $html;
                                ?>

                            </tbody>
                        </table>

                    </div>
                </div>
            </div>
        </div>
        <br>

    </div>

    <script>

    </script>
</body>