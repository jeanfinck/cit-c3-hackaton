<?php
/**
 * @file
 * Implements features related to Vote functionality.
 */

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
 * Send a response via json format, related to a node vote.
 *
 * @param array $args
 */
function contribute_send_node_vote_response_json_format($args) {
  print json_encode($args);
  drupal_exit();
}