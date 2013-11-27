<?php
/**
 * TODO: Insert template variables available.
 */

global $user;
?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print $unpublished; ?>

  <div<?php print $content_attributes; ?>>
    <div class="answer answer-nid-<?php print $nid;?> <?php if ($is_best_answer): ?>best<?php endif;?>">
      <input type="hidden" value="<?php print $nid;?>">
      <?php if ($current_user_already_voted):?>
      <div class="already-voted">You voted for this Answer</div>
      <?php endif;?>

      <!-- Vote -->
      <div class="vote">
        <a title="This answer is useful" class="vote-up">up vote</a>
        <span class="vote-count-post "><?php print $total_votes;?></span>
        <a title="This answer is not useful" class="vote-down">down vote</a>
        <?php if (($question->uid == $user->uid) && !$question->selected_answer): ?>
        <a title="Select the best Answer" class="best-answer" href="/post/answer/best/<?php print $node->nid;?>/<?php print $node->nid;?>">Best Answer</a>
        <?php endif;?>
        <?php if ($is_best_answer):?>
        <span class="vote-accepted-on" title="The question owner accepted this as the best answer">accepted</span>
        <?php endif;?>
      </div>
      <!-- [End] Vote -->

      <!-- Answer content -->
      <?php print html_entity_decode(render(field_view_field('node', $node, 'body', array('label'=>'hidden')))); ?>
      <!-- [End] Answer content -->

      <!-- User data -->
      <div class="user-data">
        <span class="time">answered <?php print date('D, d M Y H:i:s', $node->created)?></span>
        <div class="author">
          <?php $user_image_path = isset($author_details->picture->uri) ? $author_details->picture->uri : 'public://pictures/default_user_avatar.png';
            print theme('image', array(
                    'path' => $user_image_path,
                    'title' => $author_details->name,
                    'width' => '32px',
                    'attributes' => array('align' => 'left', 'hspace' => '10'),
                  ));?>
          <div class="author-info">
            <?php print l($author_details->name, 'user/' . $author_details->uid, array('attributes' => array('target'=>'_blank')));?><br/>
            <?php if ($author_details->mail) : ?>
              <span><?php print $author_details->mail;?></span><br>
            <?php endif; ?>
            <span>Score: 10.000</span><br />
            <span class="badge-icon">4 badges</span>
          </div>
        </div>
      </div>
      <!-- [End] User data -->

      <!-- Comments -->
      <?php print render($content['comments']); ?>
      <!-- [End] Comments -->
    </div>
  </div>
</article>
