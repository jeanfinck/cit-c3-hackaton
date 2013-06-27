<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print $unpublished; ?>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>

  <div<?php print $content_attributes; ?>>

    <!-- Question -->
    <div class="question">
      <?php print render($content['body']);?>
    </div>
    <!-- [End] Question -->

    <!-- Question user data -->
    <div class="user-data">
      <span class="time">posted <?php print date('D, d M Y H:i:s', $node->created)?></span>
      <div class="author">
        <?php $user_image_path = isset($node->author_details->picture->uri) ? $node->author_details->picture->uri : 'public://pictures/default_user_avatar.png';
          print theme('image', array(
                  'path' => $user_image_path,
                  'title' => $node->author_details->name,
                  'width' => '32px',
                  'attributes' => array('align' => 'left', 'hspace' => '10'),
                ));?>
        <?php print l($node->author_details->name, 'user/' . $node->uid, array('attributes' => array('target'=>'_blank')));?><br/>
        <?php print $node->author_details->mail;?>
      </div>
    </div>
    <!-- [End] Question user data -->

    <!-- Tag and categories -->
    <div class="tags categories">
      <?php print render($content['field_question_category']);?>
      <?php print render($content['field_question_tags']);?>
    </div>
    <!-- [End] Tag and categories -->

    <!-- Answers -->
    <div class="answers">
      <h2 clas="answer-count"><?php print count($answers);?> Answers</h2>

      <!-- Correct Answer -->
      <?php if (!empty($correct_answer)):?>
      <div class="answer answer-nid-<?php print $correct_answer->nid;?> correct">

        <!-- Answers Vote -->
        <div class="vote">
          <input type="hidden" value="<?php print $correct_answer->nid;?>">
          <a title="This answer is useful" class="vote-up">up vote</a>
          <span class="vote-count-post">
            <?php print $node->total_votes;?>
          </span>
          <a title="This answer is not useful" class="vote-down">down vote</a>
          <span class="vote-accepted-on">accepted</span>
        </div>
        <!-- [End] Answers Vote -->

        <?php print render(field_view_field('node', $correct_answer, 'body', array('label'=>'hidden')));?>
      </div>
      <?php endif;?>
      <!-- [End] Correct Answer -->

      <!-- Other Answers -->
      <?php if (!empty($correct_answer)){ unset($answers[$correct_answer->nid]); } // Remove correct answer from the normal list
              foreach ($answers as $nid => $answer_node):?>
      <div class="answer answer-nid-<?php print $nid;?>">
        <?php if ($answer_node->current_user_already_voted):?>
        <div class="already-voted">You voted for this Answer</div>
        <?php endif;?>

        <!-- Answers Vote -->
        <div class="vote">
          <input type="hidden" value="<?php print $nid;?>">
          <a title="This answer is useful" class="vote-up">up vote</a>
          <span class="vote-count-post ">
          <?php print $answer_node->total_votes;?>
          </span>
          <a title="This answer is not useful" class="vote-down">down vote</a>
        </div>
        <!-- [End] Answers Vote -->

        <?php $element = field_view_field('node', $answer_node, 'body', array('label'=>'hidden'));
          print render($element);?>

        <!-- Answers User data -->
        <div class="user-data">
          <span class="time">answered <?php print date('D, d M Y H:i:s', $answer_node->created)?></span>
          <div class="author">
            <?php $user_image_path = isset($answer_node->author_details->picture->uri) ? $answer_node->author_details->picture->uri : 'public://pictures/default_user_avatar.png';
              print theme('image', array(
                      'path' => $user_image_path,
                      'title' => $answer_node->author_details->name,
                      'width' => '32px',
                      'attributes' => array('align' => 'left', 'hspace' => '10'),
                    ));?>
            <?php print l($answer_node->author_details->name, 'user/' . $answer_node->author_details->uid, array('attributes' => array('target'=>'_blank')));?><br/>
            <?php print $answer_node->author_details->mail;?>
          </div>
        </div>
        <!-- [End] Answers User data -->

      </div>
      <?php endforeach; ?>
    </div>
    <!-- [End] Answers -->

  </div>

  <!-- Comment -->
  <div class="comment-box">
    <?php //print render(drupal_get_form('contribute_comment_form')); ?>
  </div>
  <!-- [End] Comment -->

  <!-- New Answer -->
  <div class="answer new">
    <?php
      $element = drupal_get_form('contribute_answer_form');
      print render($element);
    ?>
  </div>
  <!-- [End] New Answer -->
</article>
