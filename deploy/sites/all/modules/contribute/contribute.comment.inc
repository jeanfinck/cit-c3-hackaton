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
  $answer_message = $form_state['values']['answer'];

  contribute_add_new_answer($question_nid, $answer_message);
}

/**
 * Adds new answer node
 *
 * @param int $question_nid
 * @param int $user_id
 * @param string $answer_message
 */
function contribute_add_new_answer($question_nid, $answer_message) {
  if ($question_nid && $answer_message) {
    $node = new stdClass();
    $node->type = CONTRIBUTE_ANSWER_CONTENT_TYPE;
    node_object_prepare($node);

    $node->title = CONTRIBUTE_PREFIX_TITLE_ANSWER . $question_nid;
    $node->language = LANGUAGE_NONE;

    // Custom fields
    $node->body[LANGUAGE_NONE][0]['value'] = $answer_message;
    $node->field_answer_question[LANGUAGE_NONE][0]['nid'] = $question_nid;
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
 * @param int $question_nid
 */
function contribute_get_answers_by_question_nid($question_nid) {
  $query = db_select('node', 'n');
  $query->leftJoin('contribute_votes', 'v', 'n.nid = v.nid');
  $query->join('field_data_field_answer_question', 'f', 'f.entity_id = n.nid');
  $query->groupBy('n.nid');
  $query->condition('f.field_answer_question_nid', $question_nid, '=');
  $query->fields('n', array('nid'));
  $query->addExpression('IFNULL(SUM(vote), 0)', 'total');
  $query->orderBy('total', 'DESC');
  $results = $query->execute()->fetchAll();

  $nodes_id = array();
  foreach ($results as $result) {
    $nodes_id[] = $result->nid;
  }

  return node_load_multiple($nodes_id);
}

/**
 * Add a vote to a given answer
 *
 * @param int $answer_nid
 * @param string $vote_type
 */
function contribute_answer_vote_callback($question_nid, $answer_nid, $vote_type) {
  $user_id = contribute_get_current_user_id();

  try {
    if (contribure_is_possible_to_make_a_answer_vote($vote_type, $user_id, $answer_nid, $question_nid)) {
      // Saves user vote
      if ($vote_type == CONTRIBUTE_VOTE_UP) {
        $vote = 1;
        _gimme_scores(CONTRIBUTE_GAMIFICATION_UP_VOTE);
      }
      else {
        $vote = -1;
        _gimme_scores(CONTRIBUTE_GAMIFICATION_DOWN_VOTE);
      }
      contribute_compute_node_vote($answer_nid, $user_id, CONTRIBUTE_VOTE_TYPE_ANSWER, $vote);

      // Success response args
      $vote_response = array(
        'status' => 'success',
        'title' => t('Success!'),
        'message' => t('Your vote for this answer was computed.'),
      );
    }
  }
  catch (Exception $e) {
    // Error response args
    $vote_response = array(
      'status' => 'error',
      'title' => t('Attention!'),
      'message' => t($e->getMessage()),
    );
  }

  // Send vote response
  $total_of_votes = contribute_get_total_node_votes($answer_nid);
  contribute_send_node_vote_response_json_format(array_merge(array('votes' => $total_of_votes, 'answer' => $answer_nid), $vote_response));
}

/**
 * Validates if is possible to make a new answer vote
 *
 * @param string $vote_type
 * @param int $user_id
 * @param int $answer_nid
 * @param int $question_nid
 *
 * @return boolean $is_possible
 */
function contribure_is_possible_to_make_a_answer_vote($vote_type, $user_id, $answer_nid, $question_nid) {
  $answer_node = node_load($answer_nid);


  $is_a_allowed_vote_type = contribute_is_a_allowed_node_vote_type($vote_type);
  $answer_belongs_to_question = contribute_answer_belongs_to_question($answer_node, $question_nid);
  $user_already_voted_for_node = contribute_user_already_voted_for_node($user_id, $answer_nid, CONTRIBUTE_VOTE_TYPE_ANSWER);
  $current_user_is_owner_of_answer = ($user_id == $answer_node->uid);

  // Throw Exceptions
  if ($current_user_is_owner_of_answer) {
    throw new Exception('You can not vote for your own answer.');
  }
  elseif ($user_already_voted_for_node) {
    throw new Exception('You have already voted on this answer.');
  }

  $is_possible = ($is_a_allowed_vote_type && $answer_belongs_to_question && !$user_already_voted_for_node && !$current_user_is_owner_of_answer);

  return $is_possible;
}

/**
 * Checks if the given vote type is allowed.
 *
 * @param string $vote_type
 */
function contribute_is_a_allowed_node_vote_type($vote_type) {
  return in_array($vote_type, contribute_get_allowed_votes_type());
}

/**
 * Returns an array with the all allowed votes type
 * This function has the role to keep all kinds of votes allowed
 *
 * @return array
 */
function contribute_get_allowed_votes_type() {
  return array(
    CONTRIBUTE_VOTE_UP,
    CONTRIBUTE_VOTE_DOWN,
  );
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
 * Send a response via json format, related to a node vote.
 *
 * @param array $args
 */
function contribute_send_node_vote_response_json_format($args) {
  print json_encode($args);
  drupal_exit();
}

/**
 * Save the vote information in the answer node
 *
 * @param int $node_id
 * @param int $user_id
 * @param int $type
 *   A numeric representation of the type of the vote
 * @param int $vote
 */
function contribute_compute_node_vote($node_id, $user_id, $type, $vote) {
  db_insert('contribute_votes')
  ->fields(array(
    'nid' => $node_id,
    'uid' => $user_id,
    'type' => $type,
    'vote' => $vote,
  ))
  ->execute();
}

/**
 * Verifies that the user has already voted in a node
 *
 * @param int $user_id
 * @param int $node_id
 * @param int $vote_type
 *   A numeric representation of the type of the vote
 */
function contribute_user_already_voted_for_node($user_id, $node_id, $vote_type) {
  $already_voted = FALSE;

  $query = db_select('contribute_votes', 'c')
  ->fields('c', array('id'))
  ->condition('uid', $user_id, '=')
  ->condition('type', $vote_type, '=')
  ->condition('nid', $node_id, '=');
  $result = $query->execute();

  if ($result->rowCount()) {
    $already_voted = TRUE;
  }

  return $already_voted;
}

/**
 * Returns the total of the node votes
 *
 * @param int $node_id
 */
function contribute_get_total_node_votes($node_id) {
  $query = db_select('contribute_votes', 'c')
  ->fields('c', array('id'))
  ->condition('nid', $node_id, '=')
  ->groupBy('c.nid');;
  $query->addExpression('SUM(vote)', 'total');
  $result = $query->execute()->fetch();

  return $result ? $result->total : 0;
}
