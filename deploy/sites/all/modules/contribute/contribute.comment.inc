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
    '#value' => t('Post Your Answer'),
  );

  return $form;
}

/**
 * Form submit handler to answer form
 * 
 * @see contribute_answer_form()
 * 
 * @param array $form
 * @param array $form_state
 */
function contribute_answer_form_submit($form, $form_state) {
  $question_nid = contribute_get_node_id_from_question_path($form_state['complete form']['#action']);
  $user_id = contribute_get_current_user_id();
  $answer_message = $form_state['values']['answer'];
  
  contribute_add_new_answer($question_nid, $user_id, $answer_message);
}

/**
 * Adds new answer node
 * 
 * @param int $question_nid
 * @param int $user_id
 * @param string $answer_message
 */
function contribute_add_new_answer($question_nid, $user_id, $answer_message) {
  if ($question_nid && $user_id && $answer_message) {
    $node = new stdClass();
    $node->type = CONTRIBUTE_ANSWER_CONTENT_TYPE;
    node_object_prepare($node);

    $node->title = "Answer to question node " . $question_nid;
    $node->language = LANGUAGE_NONE;

    // Custom fields
    $node->body[LANGUAGE_NONE][0]['value'] = $answer_message;
    $node->field_answer_user[LANGUAGE_NONE][0]['uid'] = $user_id;
    $node->field_answer_question[LANGUAGE_NONE][0]['nid'] = $question_nid;
    $node->field_answer_votes[LANGUAGE_NONE][0]['value'] = 0;
    node_save($node);
  }
}

/**
 * Returns the node id from a given question path page.
 * The question path page follow the pattern '/question/%nid%/%question-alias-title%'
 * 
 * @param string $question_path
 * @return Ambigous <NULL, int>
 */
function contribute_get_node_id_from_question_path($question_path) {
  $exploded_path = explode('/', $question_path);
  
  $node_id = $exploded_path[2];
  
  return is_numeric($node_id) ? $node_id : NULL;
}

/**
 * Returns the ID of the current logged user
 * 
 * @return int
 */
function contribute_get_current_user_id() {
  $account = $GLOBALS['user'];
  
  return $account->uid;
}

/**
 * Returns an array with the nodes object of the answers related to a question nid
 *
 * @param nid $question_nid
 */
function contribute_get_answers_by_question_nid($question_nid) {
  $query = new EntityFieldQuery;
  $entities = $query
  ->entityCondition('entity_type', 'node')
  ->propertyCondition('status', 1)
  ->fieldCondition('field_answer_question', 'nid', $question_nid, '=')
  ->fieldOrderBy('field_answer_votes','value', 'DESC')
  ->execute();

  return !empty($entities) ? node_load_multiple(array_keys($entities['node'])) : array();
}