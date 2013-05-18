<?php
/**
 * @file
 * Implements features related to Questions.
 */

/**
 * Form constructor for Ask a Question form.
 *
 * @return array
 *   An array of form items.
 */
function contribute_ask_form($form, &$form_state) {
  $form['title'] = array(
    '#title' => t('Title:'),
    '#type' => 'textfield',
    '#maxlength' => 255,
  );

  return $form;
}
