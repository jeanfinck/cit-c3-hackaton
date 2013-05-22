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

  $form['tags'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'field-type-taxonomy-term-reference',
        'field-name-field-question-tags',
        'field-widget-taxonomy-autocomplete',
      )
    ),
    '#tree' => TRUE,
    '#language' => LANGUAGE_NONE,
    LANGUAGE_NONE => array(
      '#language' => LANGUAGE_NONE,
      '#entity_type' => 'node',
      '#bundle' => 'question',
      '#field_name' => 'field_question_tags',
      '#columns' => array('tid'),
      '#title' => t('Tags:'),
      '#type' => 'textfield',
      '#autocomplete_path' => 'taxonomy/autocomplete/field_question_tags',
      '#after_build' => array('field_form_element_after_build'),
      '#field_parents' => array(),
    ),
    '#access' => TRUE,
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
  //die(print_r($form_state));
  if (!empty($form_state['values']['tags'][LANGUAGE_NONE])) {
    $typed_terms = drupal_explode_tags($form_state['values']['tags'][LANGUAGE_NONE]);
    $tags = array();

    foreach ($typed_terms as $typed_term) {
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

  foreach ($tags as $tag) {
    $node->field_question_tags[LANGUAGE_NONE][]['tid'] = $tag['tid'];
  }
//die(print_r($node));

  node_save($node);

  drupal_set_message("Your question has been posted!");
}
