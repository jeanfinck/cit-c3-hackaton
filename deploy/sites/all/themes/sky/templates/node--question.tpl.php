<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <?php print $unpublished; ?>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>

  <div<?php print $content_attributes; ?>>

    <div class="question">
      <?php print render($content['body']);?>
    </div>

    <div class="user-data">
      <?php print render($content['field_question_user']);?>
      <?php if (!empty($user_picture)): ?>
        <div class="user-picture"><?php print $user_picture; ?></div>
      <?php endif; ?>

      <?php if ($display_submitted): ?>
        <div class="submitted"><?php print $submitted; ?></div>
      <?php endif; ?>
    </div>

    <div class="tags categories">
      <?php print render($content['field_question_category']);?>
      <?php print render($content['field_question_tags']);?>
    </div>

    <div class="answers">
      <h2><?php print count($answers);?> Answers</h2>
      <!-- Correct Answer -->
      <?php if (!empty($correct_answer)):?>
      <div class="answer answer-nid-<?php print $correct_answer->nid;?> correct">
        <div class="vote">
          <input type="hidden" value="<?php print $correct_answer->nid;?>">
          <a title="This answer is useful" class="vote-up">up vote</a>
          <span class="vote-count-post">
          <?php print render(field_view_field('node', $correct_answer, 'field_answer_votes', array('label'=>'hidden')));?>
          </span>
          <a title="This answer is not useful" class="vote-down">down vote</a>
          <span class="vote-accepted-on">accepted</span>
        </div>

        <?php print render(field_view_field('node', $correct_answer, 'body', array('label'=>'hidden')));?>
      </div>
      <?php endif;?>

      <!-- Other Answers -->
      <?php if (!empty($correct_answer)){ unset($answers[$correct_answer->nid]); } // Remove correct answer from the normal list
      foreach ($answers as $nid => $answer_node): ?>
      <div class="answer answer-nid-<?php print $nid;?>">
        <div class="vote">
          <input type="hidden" value="<?php print $nid;?>">
          <a title="This answer is useful" class="vote-up">up vote</a>
          <span class="vote-count-post ">
          <?php print render(field_view_field('node', $answer_node, 'field_answer_votes', array('label'=>'hidden')));?>
          </span>
          <a title="This answer is not useful" class="vote-down">down vote</a>
        </div>
        <?php print render(field_view_field('node', $answer_node, 'body', array('label'=>'hidden')));?>
      </div>
      <?php endforeach; ?>
    </div>

  </div>

  <div class="comment-box">
    <?php //print render(drupal_get_form('contribute_comment_form')); ?>
  </div>

  <div class="answer new">
    <?php print render(drupal_get_form('contribute_answer_form')); ?>
  </div>
</article>
