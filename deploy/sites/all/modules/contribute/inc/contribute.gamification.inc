<?php
/**
 * @file
 * Implements Gamification functions
 */

function contribute_gimme_scores($action) {
  $user_id = contribute_get_current_user_id();

  $node = current(entity_load('node', FALSE, array('title' => $action)));
  $score = current(field_get_items('node', $node, 'field_scores_earned'));
  $score = $score['value'];
  $q = db_insert("contribute_user_scores")
  ->fields(array(
    'uid' => $user_id,
    'score' => $score,
    'action' => $action
  ))
  ->execute();
  contribute_gimme_badge($action, $user_id, $node->nid);
}

function contribute_gimme_badge($action, $uid, $nid) {
  //Get user scores, grouped by action
  $scores_query = db_query("SELECT SUM(score) as points, action FROM {contribute_user_scores} WHERE uid = :user AND action = :action GROUP BY action", array('user' => $uid, 'action' => $action));
  $scores = $scores_query->fetchObject();

  //Get badges
  $badges_query = new EntityFieldQuery();
  $badges_query->entityCondition('entity_type', 'node')
  ->entityCondition('bundle', 'badges', '=')
  ->fieldCondition('field_action', 'nid', $nid, '=');
  $badges_query = $badges_query->execute();
  if(!empty($badges_query)) {
    $nids = array_keys($badges_query['node']);
    $badges = node_load_multiple($nids);

    foreach($badges as $badge) {
      $needed = current(field_get_items('node', $badge, 'field_scores_needed'));
      $needed = $needed['value'];
      if($needed <= $scores->points) {
        $verify_query = db_query("SELECT uid from {contribute_badges_users} WHERE uid = :uid AND badge_nid = :badgeId", array('uid' => $uid, 'badgeId' => $badge->nid));
        if($verify_query->rowCount() == 0){
          $q = db_insert("contribute_badges_users")
          ->fields(array(
            'uid' => $uid,
            'badge_nid' => $badge->nid,
          ))
          ->execute();
        }
      }
    }
  }
}