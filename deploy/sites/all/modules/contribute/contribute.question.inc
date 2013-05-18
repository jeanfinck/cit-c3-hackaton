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
function contribute_question_form_validate($form, &$form_state) {

}

/**
 * Submittion handler for contribute_question_form().
 *
 * @param  [type] $form
 * @param  [type] $form_state
 * @return [type]
 */
function contribute_question_form_submit($form, &$form_state) {

}
