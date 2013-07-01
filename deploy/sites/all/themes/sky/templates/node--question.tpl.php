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

    <!-- Answers Container -->
    <div class="answers">
      <h2 clas="answer-count"><?php print count($answers);?> Answers</h2>

      <!-- Best Answer Selected -->
      <?php if (!empty($best_answer)):?>
      <div class="answer answer-nid-<?php print $best_answer->nid;?> best">

        <?php if ($best_answer->current_user_already_voted):?>
        <div class="already-voted">You voted for this Answer</div>
        <?php endif;?>

        <!-- Vote -->
        <div class="vote">
          <input type="hidden" value="<?php print $best_answer->nid;?>">
          <a title="This answer is useful" class="vote-up">up vote</a>
          <span class="vote-count-post"><?php print $best_answer->total_votes;?></span>
          <a title="This answer is not useful" class="vote-down">down vote</a>
          <span class="vote-accepted-on" title="The question owner accepted this as the best answer">accepted</span>
        </div>
        <!-- [End] Vote -->

        <!-- User data -->
        <div class="user-data">
          <span class="time">answered <?php print date('D, d M Y H:i:s', $best_answer->created)?></span>
          <div class="author">
            <?php $user_image_path = isset($best_answer->author_details->picture->uri) ? $best_answer->author_details->picture->uri : 'public://pictures/default_user_avatar.png';
              print theme('image', array(
                      'path' => $user_image_path,
                      'title' => $best_answer->author_details->name,
                      'width' => '32px',
                      'attributes' => array('align' => 'left', 'hspace' => '10'),
                    ));?>
            <?php print l($best_answer->author_details->name, 'user/' . $best_answer->author_details->uid, array('attributes' => array('target'=>'_blank')));?><br/>
            <?php print $best_answer->author_details->mail;?>
          </div>
        </div>
        <!-- [End] User data -->

        <?php print render(field_view_field('node', $best_answer, 'body', array('label'=>'hidden')));?>
      </div>
      <?php endif;?>
      <!-- [End] Best Answer Selected -->

      <!-- Other Answers -->
      <?php if (!empty($best_answer)){ unset($answers[$best_answer->nid]); } // Remove best answer from the normal list
              foreach ($answers as $nid => $answer_node):?>
      <div class="answer answer-nid-<?php print $nid;?>">

        <?php if ($answer_node->current_user_already_voted):?>
        <div class="already-voted">You voted for this Answer</div>
        <?php endif;?>

        <!-- Vote -->
        <div class="vote">
          <input type="hidden" value="<?php print $nid;?>">
          <a title="This answer is useful" class="vote-up">up vote</a>
          <span class="vote-count-post "><?php print $answer_node->total_votes;?></span>
          <a title="This answer is not useful" class="vote-down">down vote</a>
          <?php global $user; if (($node->uid == $user->uid) && empty($best_answer)):?>
          <a title="Select the best Answer" class="best-answer" href="/post/answer/best/<?php print $node->nid;?>/<?php print $answer_node->nid;?>">Best Answer</a>
          <?php endif;?>
        </div>
        <!-- [End] Vote -->

        <?php print render(field_view_field('node', $answer_node, 'body', array('label'=>'hidden')));?>

        <!-- User data -->
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
        <!-- [End] User data -->

      </div>
      <?php endforeach; ?>
    </div>
    <!-- [End] Answers Container -->
  </div>

  <!-- Comment -->
  <div class="comment-box">
    <?php //print render(drupal_get_form('contribute_comment_form')); ?>
  </div>
  <!-- [End] Comment -->

  <!-- New Answer -->
  <?php global $user; if ($node->uid != $user->uid):?>
  <div class="answer new">
    <?php print render(drupal_get_form('contribute_answer_form')); ?>
  </div>
  <?php endif;?>
  <!-- [End] New Answer -->
</article>
