<?php
/**
 * @file
 * Some functions to help development debugging
 */

/**
 * Dumps the input variable for debugging purposes
 * with a preformatted HTML (no need to check the source code for better view).
 *
 * This works better with arrays.
 *
 * @param Mixed $data
 *   The input variable to be printed
 */
function pr($data) {
  echo "<pre>" . print_r($data, TRUE) . "</pre>";
}

/**
 * Dumps the input variable for debugging purposes
 * with a preformatted HTML (no need to check the source code for better view)
 *
 * @param Mixed $data
 *   The input variable to be printed
 */
function dump($data) {
  pr(var_export($data, TRUE));
}


/**
 * Dumps the input variable for debugging purposes
 * with a preformatted HTML (no need to check the source code for better view)
 *
 * This function is the same as pr() but it also stops the script execution.
 *
 * @param Mixed $data
 *   The input variable to be printed
 */
function e($data) {
	die(pr($data));
}

/**
 * Dumps the input variable for debugging purposes
 * with a preformatted HTML (no need to check the source code for better view)
 *
 * This function is the same as dump() but it also stops the script execution.
 *
 * @param Mixed $data
 *   The input variable to be printed
 */
function d($data) {
	die(dump($data));
}
