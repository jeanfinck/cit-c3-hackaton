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

  </div>

  <div class="comment">
    <?php print render(drupal_get_form('contribute_comment_form')); ?>
  </div>

</article>
