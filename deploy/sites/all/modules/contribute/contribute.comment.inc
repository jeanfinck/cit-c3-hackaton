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

    $node->title = CONTRIBUTE_PREFIX_TITLE_ANSWER . $question_nid;
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
  ->fieldCondition(CONTRIBUTE_FIELD_ANSWER_QUESTION, 'nid', $question_nid, '=')
  ->fieldOrderBy(CONTRIBUTE_FIELD_ANSWER_VOTES, 'value', 'DESC')
  ->execute();

  return !empty($entities) ? node_load_multiple(array_keys($entities['node'])) : array();
}

/**
 * Add a vote to a given answer
 *
 * @param int $answer_nid
 * @param string $vote_type
 */
function contribute_add_answer_vote_callback($question_nid, $answer_nid, $vote_type = CONTRIBUTE_VOTE_UP) {
  $answer_node = node_load($answer_nid);

  // Confirms if the user not voted yet
  $user_id = contribute_get_current_user_id();
  if (contribute_answer_belongs_to_question($answer_node, $question_nid)
      && !contribute_user_already_voted_for_answer($user_id, $answer_node)) {
    // Add Score
    _gimme_scores(CONTRIBUTE_GAMIFICATION_UP_VOTE);

    // Total of computed votes
    $total_of_votes = contribute_get_total_computed_answer_votes($answer_node, $vote_type);

    // Save Answer, votes and user vote
    contribute_compute_answer_vote($answer_nid, $total_of_votes, $user_id);

    // Send success response in a json format
    contribute_send_a_success_vote_response_json_format($total_of_votes, $answer_nid);
  }
  else {
    // Add Score
    _gimme_scores(CONTRIBUTE_GAMIFICATION_DOWN_VOTE);

    // Get default votes of the answer
    $total_of_votes = contribute_get_total_computed_answer_votes($answer_node);

    // Send error response in a json format
    contribute_send_a_already_voted_response_json_format($total_of_votes, $answer_nid);
  }
}

/**
 * Verifies if a answer belongs to a question
 *
 * @param object $answer_node
 * @param int $question_nid
 */
function contribute_answer_belongs_to_question($answer_node, $question_nid) {
  $belongs = FALSE;

  if ($answer_node->field_answer_question[LANGUAGE_NONE][0]['nid'] == $question_nid) {
    $belongs = TRUE;
  }

  return $belongs;
}

/**
 * Returns the total of the computed answer votes
 *
 * @param object $answer_node
 * @param string $vote_type
 */
function contribute_get_total_computed_answer_votes($answer_node, $vote_type = NULL) {
  $total_of_votes = contribute_get_total_answer_votes($answer_node);

  switch ($vote_type) {
    case CONTRIBUTE_VOTE_UP:
      // Positive Vote
      $total_of_votes++;
      break;

    case CONTRIBUTE_VOTE_DOWN:
      // Negative Vote
      $total_of_votes--;
      break;
  }

  return $total_of_votes;
}

/**
 * Send a success response via json format related to a vote to a answer.
 *
 * @param int $total_of_votes
 * @param int $answer_nid
 */
function contribute_send_a_success_vote_response_json_format($total_of_votes, $answer_nid) {
  contribute_send_vote_response_json(array(
    'status' => 'success',
    'title' => 'Success!',
    'message' => 'Your vote for this answer was computed.',
    'votes' => $total_of_votes,
    'answer' => $answer_nid,
  ));
}

/**
 * Send a 'already voted' response via json format related to a vote to a answer.
 *
 * @param int $total_of_votes
 * @param int $answer_nid
 */
function contribute_send_a_already_voted_response_json_format($total_of_votes, $answer_nid) {
  contribute_send_vote_response_json(array(
    'status' => 'error',
    'title' => 'Attention',
    'message' => 'You have already voted on this answer.',
    'votes' => $total_of_votes,
    'answer' => $answer_nid,
  ));
}

/**
 * Print a response via json format
 *
 * @param array $args
 */
function contribute_send_vote_response_json($args) {
  print json_encode($args);
}

/**
 * Save the vote information in the answer node
 *
 * @param int $answer_nid
 * @param int $total_of_votes
 * @param int $user_id
 */
function contribute_compute_answer_vote($answer_nid, $total_of_votes, $user_id){
  $answer_node = node_load($answer_nid);

  // Save Answer, votes and user vote
  $answer_node->field_answer_votes[LANGUAGE_NONE][0]['value'] = $total_of_votes;
  $answer_node->field_answer_user_votes[LANGUAGE_NONE][]['uid'] = $user_id;
  node_save($answer_node);
}

/**
 * Verifies that the user has already voted in the answer
 *
 * @param int $user_id
 * @param object $answer_node
 */
function contribute_user_already_voted_for_answer($user_id, $answer_node) {
  $already_voted = FALSE;

  $user_votes_field = field_get_items('node', $answer_node, CONTRIBUTE_FIELD_ANSWER_USER_VOTES);
  if (is_array($user_votes_field)) {
    foreach($user_votes_field as $user_vote) {
      if ($user_vote['uid'] == $user_id) {
        $already_voted = TRUE;
        break;
      }
    }
  }

  return $already_voted;
}

/**
 * Returns the total of the answer votes
 *
 * @param object $answer_node
 */
function contribute_get_total_answer_votes($answer_node) {
  $total_votes_field_item = field_get_items('node', $answer_node, CONTRIBUTE_FIELD_ANSWER_VOTES);

  return $total_votes_field_item[0]['value'];
}
