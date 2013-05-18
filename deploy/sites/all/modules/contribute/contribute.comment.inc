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
    '#value' => t('Submit')
  );

  return $form;
}