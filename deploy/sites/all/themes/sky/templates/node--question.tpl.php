<?php
/**
 * TODO: Insert template variables available.
 */
?>

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
            <?php if ($node->author_details->mail) : ?>
              <span><?php print $node->author_details->mail;?></span><br>
            <?php endif; ?>
            <span>Score: 10.000</span><br />
            <span class="badge-icon">4 badges</span>
          </div>
        </div>
      </div>
      <!-- [End] Question user data -->

      <!-- Tag and categories -->
      <div class="tags categories">
        <?php print render($content['field_question_category']);?>
        <?php print render($content['field_question_tags']);?>
      </div>
      <!-- [End] Tag and categories -->

      <!-- Comments -->
      <?php print render($content['comments']); ?>
      <!-- [End] Comments -->
      </div>
      <!-- [End] Question -->

    <!-- Answers Container -->
    <div class="answers">
      <h2 class="answer-count"><?php print count($answers);?> Answers</h2>

      <!-- Answers -->
      <?php
      foreach ($answers as $nid => $answer_node) {
        $answer_node_view = node_view($answer_node);

        // Adds answer comment box.
        $answer_node_view['comments'] = comment_node_page_additions($answer_node);

        print drupal_render($answer_node_view);
      }
      ?>
    </div>
    <!-- [End] Answers Container -->
  </div>

  <!-- New Answer -->
  <?php global $user; if ($node->uid != $user->uid):?>
  <div class="answer new">
    <?php print render(drupal_get_form('contribute_answer_form')); ?>
  </div>
  <?php endif;?>
  <!-- [End] New Answer -->
</article>
