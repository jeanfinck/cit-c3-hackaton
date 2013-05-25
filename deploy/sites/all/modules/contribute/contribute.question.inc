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
function contribute_question_form($form, $form_state) {

  if (!isset($_SESSION['advice_accepted']) || !$_SESSION['advice_accepted']) {
    drupal_goto('questions/ask/advice');
  }

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#placeholder' => t("what's your programming question? be specific"),
  );

  $form['category'] =  array(
    '#title' => t('Category'),
    '#type' => 'select',
    '#options' => array('- Select a main category -'),
  );

  $terms = taxonomy_get_tree(2);

  foreach ($terms as $term) {
    $form['category']['#options'][$term->tid] = $term->name;
  }

  $form['question'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#format' => 'minimal',
    '#required' => TRUE,
  );

  $tags = isset($form_state['input']['terms']) ? drupal_explode_tags($form_state['input']['terms']) : array();
  $terms = array();

  foreach ($tags as $i => $tag) {
    $terms[$i] = new stdClass;
    $terms[$i]->tid = 'text';
    $terms[$i]->name = $tag;
  }

  $form['field_terms'] = array(
    '#markup' => theme('active_tags_term_list_remove', array('terms' => $terms)),
  );

  $form['term_entry'] = array(
    '#title' => t('Tags'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'taxonomy/autocomplete/field_question_tags',
    '#size' => 60,
    '#maxlength' => 1024,
    '#attributes' => array('class' => array('at-term-entry')),
    '#placeholder' => 'at least one tag such as (c# c arrays), max 5 tags'
  );

  $form['add_button'] = array(
    '#type' => 'button',
    '#value' => t('Add'),
    '#attributes' => array('class' => array('at-add-btn')),
  );

  $form['terms'] = array(
    '#type' => 'hidden',
    '#title' => t('Tags'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 1024,
    '#attributes' => array('class' => array('at-terms')),
    '#theme_wrappers' => array(),
  );


  $form['submit'] = array(
    '#value' => t('Post Your Question'),
    '#type' => 'submit',
  );

  drupal_add_css(drupal_get_path('module', 'active_tags') . '/active_tags.css');
  drupal_add_js(array('activeTags' => array('mode' => 'single')), 'setting');
  drupal_add_js(drupal_get_path('module', 'active_tags') . '/active_tags.js');
  drupal_add_js(drupal_get_path('module', 'contribute') . '/js/contribute.question.js');

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
function contribute_question_form_validate(&$form, &$form_state) {
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
  if (!empty($form_state['values']['terms'])) {
    $typed_terms = drupal_explode_tags($form_state['values']['terms']);
    $tags = array();

    foreach ($typed_terms as $typed_term) {
      $typed_term = strtolower($typed_term);
      // See if the term exists in the chosen vocabulary and return the tid;
      // otherwise, create a new 'autocreate' term for insert/update.
      if ($possibilities = taxonomy_term_load_multiple(array(), array('name' => trim($typed_term), 'vid' => 1))) {
        $term = array_pop($possibilities);
      }
      else {
        $term = array(
          'tid' => 'autocreate',
          'vid' => 1,
          'name' => $typed_term,
          'vocabulary_machine_name' => 'tags',
        );
      }
      $tags[] = (array)$term;
    }
  }

  taxonomy_field_presave(NULL, NULL, NULL, NULL, NULL, $tags);

  global $user;

  $node = new stdClass;
  $node->type = 'question';
  $node->field_question_user[LANGUAGE_NONE][0]['uid'] = $user->uid;
  $node->name = $user->name;
  $node->created = strtotime("now");
  $node->changed = strtotime("now");
  $node->status = 1;
  $node->comment = 0;
  $node->promote = 0;
  $node->language = LANGUAGE_NONE;
  $node->moderate = 0;
  $node->sticky = 0;
  $node->revision = 0;
  $node->title = $form_state['values']['title'];
  $node->body[LANGUAGE_NONE][0] = $form_state['values']['question'];

  foreach ($tags as $tag) {
    $node->field_question_tags[LANGUAGE_NONE][]['tid'] = $tag['tid'];
  }

  node_save($node);
  drupal_set_message("Your question has been posted!");
}

/**
 * [contribute_advice_accept_form description]
 * @return [type]
 */
function contribute_advice_accept_form() {
  $items['accept'] = array(
    '#type' => 'checkbox',
    '#title' => t('thanks, I will keep these tips in mind when asking'),
  );

  $items['submit'] = array(
    '#type' => 'submit',
    '#value' => t('proceed'),
  );

  return $items;
}

/**
 * [contribute_advice_accept_form description]
 * @return [type]
 */
function contribute_advice_accept_form_validate($form, &$form_state) {
  if (!$form_state['values']['accept']) {
    form_set_error('accept', t('Please read our advices on asking a new question.'));
  }
}

/**
 * [contribute_advice_accept_form description]
 * @return [type]
 */
function contribute_advice_accept_form_submit($form, &$form_state) {
  $_SESSION['advice_accepted'] = TRUE;
  drupal_goto('questions/ask');
}

function contribute_form_search_block_form_alter(&$form, &$form_state, $form_id) {
  $form['search_block_form']['#placeholder'] = t('search for questions here!');
}
