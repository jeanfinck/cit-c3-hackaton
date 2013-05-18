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
function contribute_question_form() {

  $form['title'] = array(
    '#title' => t('Title:'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  $form['category'] =  array(
    '#title' => t('Category:'),
    '#type' => 'select',
    '#options' => array('- Select a main category -'),
  );

  $terms = taxonomy_get_tree(1);

  foreach ($terms as $term) {
    $form['category']['#options'][$term->tid] = $term->name;
  }

  $form['question'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#format' => 'minimal',
    //'format' => array('access' => FALSE), // Hiding select options
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#value' => t('Post your question'),
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Validation handler for contribute_question_form().
 *
 * @param  [type] $form
 * @param  [type] $form_state
 * @return [type]
 */
function contribute_form_contribute_question_form_alter(&$form, &$form_state, $form_id) {
  $form['#after_build'][] = 'contribute_question_form_after_build';
}

function contribute_question_form_after_build(&$form) {
  $form['question']['format']['#access'] = FALSE;
  unset($form['question']['#pre_render'][0]);
  return $form;
}

/**
 * Validation handler for contribute_question_form().
 *
 * @param  [type] $form
 * @param  [type] $form_state
 * @return [type]
 */
function contribute_question_form_validate($form, &$form_state) {
  if ($form_state['values']['category'] == 0) {
    form_set_error('category', t('Choose a main category.'));
  }

  if (empty($form_state['values']['question']['value'])) {
    form_set_error('question', t('What about your question?'));
  }
}

/**
 * Submittion handler for contribute_question_form().
 *
 * @param  [type] $form
 * @param  [type] $form_state
 * @return [type]
 */
function contribute_question_form_submit($form, &$form_state) {
  global $user;

  $node = new stdClass;
  $node->type = 'question';
  $node->uid = $user->uid;
  $node->created = strtotime("now");
  $node->changed = strtotime("now");
  $node->status = 1;
  $node->comment = 0;
  $node->promote = 0;
  $node->moderate = 0;
  $node->sticky = 0;
  $node->title = $form['title']['#value'];
  $node->body = $form['question']['value']['#value'];

  node_save($node);

  drupal_set_message("Your question has been posted!");
}
