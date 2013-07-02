<?php
/**
 * @file
 * Implements features related to Answers.
 */

/**
 * Form constructor for comments
 *
 * @return array
 *   An array of form items.
 */
function contribute_answer_comment_form($form, &$form_state) {
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
      'callback' => 'contribute_answer_comment_form_ajax_submit',
      'wrapper' => 'answers',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $form;
}

function contribute_answer_comment_form_ajax_submit($form, &$form_state) {
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
 * Returns an array with the nodes object of the answers related to a question nid
 *
 * @param int $question_nid
 */
function contribute_get_answers_by_question_nid($question_nid) {
  $query = db_select('node', 'n');
  $query->leftJoin('contribute_votes', 'v', 'n.nid = v.nid');
  $query->leftJoin('field_data_field_select_answer', 's', 's.field_select_answer_nid = n.nid');
  $query->innerJoin('field_data_field_answer_question', 'f', 'f.entity_id = n.nid');
  $query->groupBy('n.nid');
  $query->condition('f.field_answer_question_nid', $question_nid, '=');
  $query->fields('n', array('nid'));
  $query->fields('s', array('field_select_answer_nid'));
  $query->addExpression('IFNULL(SUM(vote), 0)', 'total');
  $query->orderBy('s.field_select_answer_nid', 'DESC');
  $query->orderBy('total', 'DESC');
  $results = $query->execute()->fetchAll();

  $nodes_id = array();
  $best_answer = NULL;
  foreach ($results as $result) {
    $nodes_id[] = $result->nid;

    if ($result->field_select_answer_nid) {
      $best_answer = $result->field_select_answer_nid;
    }
  }

  $nodes = node_load_multiple($nodes_id);
  if ($nodes && $best_answer) {
    $nodes[$best_answer]->best_answer = true;
  }

  return $nodes;
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
        contribute_gimme_scores(CONTRIBUTE_GAMIFICATION_UP_VOTE);
      }
      else {
        $vote = -1;
        contribute_gimme_scores(CONTRIBUTE_GAMIFICATION_DOWN_VOTE);
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
 * Verifies if a question belongs to the current user
 *
 * @param int $question_id
 */
function contribute_question_belongs_to_current_user($question_id) {
  $belongs = FALSE;

  $question_node = node_load($question_id);
  $current_user_id = contribute_get_current_user_id();

  if ($question_node->uid == $current_user_id) {
    $belongs = TRUE;
  }

  return $belongs;
}

/**
 * Verifies if a question already have a best answer
 *
 * @param int $question_nid
 */
function contribute_question_already_have_best_answer($question_nid) {
  $question_node = node_load($question_nid);

  return isset($question_node->field_select_answer[LANGUAGE_NONE][0]['nid']) ? TRUE : FALSE;
}

/**
 * Callback to hook_menu to user select the best answer to a question
 *
 * @param int $question_nid
 * @param int $answer_nid
 */
function contribute_select_best_answer_to_question_callback($question_nid, $answer_nid) {
  $answer_node = node_load($answer_nid);

  // Validates if is a correct request
  if (contribute_question_belongs_to_current_user($question_nid)
      && contribute_answer_belongs_to_question($answer_node, $question_nid)
      && !contribute_question_already_have_best_answer($question_nid)) {
    contribute_save_best_answer_choice_to_question($question_nid, $answer_nid);
  }

  drupal_goto(drupal_get_path_alias('node/' . $question_nid));
}

/**
 * Save the best answer to a question
 *
 * @param int $question_nid
 * @param int $best_answer_nid
 */
function contribute_save_best_answer_choice_to_question($question_nid, $best_answer_nid) {
  $question_node = node_load($question_nid);
  node_object_prepare($question_node);
  $question_node->field_select_answer[LANGUAGE_NONE][0]['nid'] = $best_answer_nid;
  node_save($question_node);
}