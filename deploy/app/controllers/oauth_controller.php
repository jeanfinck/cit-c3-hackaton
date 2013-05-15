<?php
require_once '../vendors/google-api-php-client/src/Google_Client.php';
require_once '../vendors/google-api-php-client/src/contrib/Google_PlusService.php';

class OauthController extends AppController {
  var $uses = array();
  
  public function index() {
    // Set your cached access token. Remember to replace $_SESSION with a
    // real database or memcached.
    session_start();
    
    $client = new Google_Client();
    $client->setApplicationName('cit-c3-coordino');
    // Visit https://code.google.com/apis/console?api=plus to generate your
    // client id, client secret, and to register your redirect uri.
    $client->setClientId('276422030021.apps.googleusercontent.com');
    $client->setClientSecret('r-8c7keF-9FWs89VDPvQR600');
    $client->setRedirectUri('http://cit.hnus.webfactional.com/oauth');
//     $client->setDeveloperKey('insert_your_simple_api_key');
    $plus = new Google_PlusService($client);
    
    if (isset($_GET['code'])) {
      $client->authenticate();
      $_SESSION['token'] = $client->getAccessToken();
      $redirect = 'http://' . $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF'];
      header('Location: ' . filter_var($redirect, FILTER_SANITIZE_URL));
    }
    
    if (isset($_SESSION['token'])) {
      $client->setAccessToken($_SESSION['token']);
    }
    
    if ($client->getAccessToken()) {
      $activities = $plus->activities->listActivities('me', 'public');
      print 'Your Activities: <pre>' . print_r($activities, true) . '</pre>';
    
      // We're not done yet. Remember to update the cached access token.
      // Remember to replace $_SESSION with a real database or memcached.
      $_SESSION['token'] = $client->getAccessToken();
    } else {
      $authUrl = $client->createAuthUrl();
      print "<a href='$authUrl'>Connect Me!</a>";
    }
    
    
  }

}
?>