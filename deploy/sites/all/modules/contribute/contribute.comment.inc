<?php
/**
 * @file
 * Implements features related to Comments.
 */

/**
 * Form constructor for comments
 *
 * @return array
 *   An array of form items.
 */
function contribute_comment_form($form, &$form_state) {
  $form['comment'] = array(
    '#title' => t('Comment'),
    '#type' => 'textarea',
    '#description' => t ('Insert a new comment'),
  );

  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => 'question',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add comment'),
    '#ajax' => array(
      'callback' => 'contribute_comment_form_ajax_submit',
      'wrapper' => 'answers',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $form;
}

function contribute_comment_form_ajax_submit($form, &$form_state) {
  return array("#markup" => "<div id='wrapper'></div>");
}

/**
 * Form constructor for comments
 *
 * @return array
 *   An array of form items.
 */
function contribute_answer_form($form, &$form_state) {
  $form['answer'] = array(
    '#title' => t('Your Answer'),
    '#type' => 'textarea',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Post Your Answer')
  );

  return $form;
}

function contribute_answer_form_submit($form, $form_state) {
//   echo '<pre>';
//   print_r($form_state); die;

  $node = new stdClass();
  $node->type = CONTRIBUTE_QUESTION_CONTENT_TYPE;
  node_object_prepare($node);

  $node->title = "My Awesome Title";
  $node->language = LANGUAGE_NONE;

  // Try to set your custom field
//   $node->field_reasons_for_being_awesome[LANGUAGE_NONE][0]['value'] = "Too many to count...";
//   $node->field_awesome_points[LANGUAGE_NONE][0]['value'] = 23;
  node_save($node);

}