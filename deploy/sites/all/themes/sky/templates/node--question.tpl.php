<?php $best_answer = field_get_items('node', $node, 'field_select_answer'); global $user; ?>

<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print $unpublished; ?>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>

  <div<?php print $content_attributes; ?>>

    <!-- Question -->
    <div class="question">
      <input type="hidden" value="<?php print $node->nid;?>">
        <!-- Vote -->
        <div class="vote">
          <a title="This question shows research effort it is useful and clear" class="vote-up">up vote</a>
          <span class="vote-count-post "><?php print $node->votes; ?></span>
          <a title="This question does not show any research effort it is unclear or not useful" class="vote-down">down vote</a>
        </div>
        <!-- [End] Vote -->
      <?php print render($content['body']);?>

      <!-- Question user data -->
      <div class="user-data">
        <span class="time">posted <?php print date('D, d M Y H:i:s', $node->created);?></span>
        <div class="author">
          <?php $user_image_path = isset($node->author_details->picture->uri) ? $node->author_details->picture->uri : 'public://pictures/default_user_avatar.png';
            print theme('image', array(
                    'path' => $user_image_path,
                    'title' => $node->author_details->name,
                    'width' => '32px',
                    'attributes' => array('align' => 'left', 'hspace' => '10'),
                  ));?>
          <div class="author-info">
            <?php print l($node->author_details->name, 'user/' . $node->uid, array('attributes' => array('target'=>'_blank')));?><br/>
            <?php if ($node->author_details->mail) { ?>
              <span><?php print $node->author_details->mail;?></span><br>
            <?php } ?>
            <span>Score: 10.000</span><br />
            <span class="badge-icon">4 badges</span>
          </div>
        </div>
      </div>
      <!-- [End] Question user data -->

      <!-- Comments -->
      <div class="comments-container">
        <div class="comments">
          <a class="new">add comment</a>
          <?php foreach ($node->comments as $question_comment): ?>
          <div class="comment">
          <?php print $question_comment->body[LANGUAGE_NONE][0]['value'];?>
           – <span>by <?php $user_comment = user_load($question_comment->uid); print l($user_comment->name, 'user/' . $user_comment->uid, array('attributes' => array('target'=>'_blank')));?>
           <?php print date('M d \a\t H:i', $question_comment->created);?></span>

          </div>
          <?php endforeach;?>
        </div>
      </div>
      <!-- [End] Comments -->

      <!-- Tag and categories -->
      <div class="tags categories">
        <?php print render($content['field_question_category']);?>
        <?php print render($content['field_question_tags']);?>
      </div>
      <!-- [End] Tag and categories -->
      </div>
      <!-- [End] Question -->

    <!-- Answers Container -->
    <div class="answers">
      <h2 class="answer-count"><?php print count($answers);?> Answers</h2>

      <!-- Answers -->
      <?php foreach ($answers as $nid => $answer_node):?>
      <div class="answer answer-nid-<?php print $nid;?> <?php if (isset($answer_node->best_answer)):?>best<?php endif;?>">
        <input type="hidden" value="<?php print $nid;?>">
        <?php if ($answer_node->current_user_already_voted):?>
        <div class="already-voted">You voted for this Answer</div>
        <?php endif;?>

        <!-- Vote -->
        <div class="vote">
          <a title="This answer is useful" class="vote-up">up vote</a>
          <span class="vote-count-post "><?php print $answer_node->total_votes;?></span>
          <a title="This answer is not useful" class="vote-down">down vote</a>
          <?php if (($node->uid == $user->uid) && !$best_answer):?>
          <a title="Select the best Answer" class="best-answer" href="/post/answer/best/<?php print $node->nid;?>/<?php print $answer_node->nid;?>">Best Answer</a>
          <?php endif;?>
          <?php if (isset($answer_node->best_answer)):?>
          <span class="vote-accepted-on" title="The question owner accepted this as the best answer">accepted</span>
          <?php endif;?>
        </div>
        <!-- [End] Vote -->

        <!-- Answer content -->
        <?php print html_entity_decode(render(field_view_field('node', $answer_node, 'body', array('label'=>'hidden')))); ?>
        <!-- [End] Answer content -->

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
            <div class="author-info">
              <?php print l($answer_node->author_details->name, 'user/' . $answer_node->author_details->uid, array('attributes' => array('target'=>'_blank')));?><br/>
              <?php if ($answer_node->author_details->mail){ ?>
                <span><?php print $answer_node->author_details->mail;?></span><br>
              <?php } ?>
              <span>Score: 10.000</span><br />
              <span class="badge-icon">4 badges</span>
            </div>
          </div>
        </div>
        <!-- [End] User data -->

        <!-- Comments -->
        <div class="comments-container">
          <div class="comments">
            <a class="new">add comment</a>
            <?php foreach ($answer_node->comments as $comment_node):?>
            <div class="comment">
            <?php print $comment_node->body[LANGUAGE_NONE][0]['value'];?>
             – <span>by <?php $user_comment = user_load($comment_node->uid); print l($user_comment->name, 'user/' . $user_comment->uid, array('attributes' => array('target'=>'_blank')));?>
             <?php print date('M d \a\t H:i', $comment_node->created);?></span>

            </div>
            <?php endforeach;?>
          </div>
        </div>
        <!-- [End] Comments -->
      </div>
      <?php endforeach; ?>
    </div>
    <!-- [End] Answers Container -->
  </div>

  <!-- Comment -->
  <div class="comment-box">
    <?php print render(drupal_get_form('contribute_new_comment_form')); ?>
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
