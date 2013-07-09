<?php
/**
 * @file
 * Implements features related to comment functionalities.
 */

/**
 * Form constructor for comments
 *
 * @return array
 *   An array of form items.
 */
function contribute_new_comment_form($form, &$form_state) {
  $form['comment'] = array(
    '#title' => t('Comment'),
    '#type' => 'textarea',
    '#description' => t ('Insert a new comment'),
    '#rows' => 3,
    '#resizable' => FALSE,
  );

  $form['reference'] = array(
    '#type' => 'hidden',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add comment'),
    '#ajax' => array(
      'callback' => 'contribute_new_comment_form_ajax_submit',
      'wrapper' => 'answers',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
  );

  return $form;
}

function contribute_new_comment_form_ajax_submit($form, &$form_state) {
  $node_reference = $form_state['values']['reference'];
  $comment_message = $form_state['values']['comment'];
  $user_id = contribute_get_current_user_id();

  contribute_add_new_comment($node_reference, $user_id, $comment_message);
}

function contribute_add_new_comment($reference_nid, $user_id, $comment) {
  $node = new stdClass();
  $node->type = CONTRIBUTE_COMMENT_CONTENT_TYPE;
  node_object_prepare($node);

  $node->title = CONTRIBUTE_PREFIX_TITLE_COMMENT . $reference_nid;
  $node->language = LANGUAGE_NONE;

  // Custom fields
  $node->body[LANGUAGE_NONE][0]['value'] = $comment;
  $node->field_comment_user[LANGUAGE_NONE][0]['uid'] = $user_id;
  $node->field_comment_reference[LANGUAGE_NONE][0]['nid'] = $reference_nid;
  node_save($node);
}

