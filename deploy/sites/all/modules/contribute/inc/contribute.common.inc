<?php
/**
 * @file
 * Common Functions
 */

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
 * Overrides the default translation of Timeago dates if necessary.
 */
function contribute_timeago_add_js() {
  $library_path = libraries_get_path('timeago');
  $path = $library_path . '/jquery.timeago.' . $GLOBALS['language']->language . '.js';

  if (file_exists($path)) {
    drupal_add_js($path, array('scope' => 'footer'));
    drupal_add_js($library_path . '/timeago.js', array('scope' => 'footer'));
  }
}

/**
 * Implements gritter plugin behavior
 *
 * Gritter is a notification bubble/popup that appears in the top right of the page.
 */
function contribute_add_gritter_plugin() {
  $gritter_library_path = libraries_get_path('gritter');

  if ($gritter_library_path) {
    drupal_add_js($gritter_library_path . '/js/jquery.gritter.min.js');
    drupal_add_css($gritter_library_path . '/css/jquery.gritter.css');
  }
}