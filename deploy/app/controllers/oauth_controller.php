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
    $plus = new Google_PlusService($client);
    
    if (isset($_REQUEST['logout'])) {
      unset($_SESSION['access_token']);
    }
    
    if (isset($_GET['code'])) {
      $client->authenticate($_GET['code']);
      $_SESSION['access_token'] = $client->getAccessToken();
      header('Location: http://' . $_SERVER['HTTP_HOST'] . $_SERVER['PHP_SELF']);
    }
    
    if (isset($_SESSION['access_token'])) {
      $client->setAccessToken($_SESSION['access_token']);
    }
    
    if ($client->getAccessToken()) {
      $me = $plus->people->get('me');
    
      // These fields are currently filtered through the PHP sanitize filters.
      // See http://www.php.net/manual/en/filter.filters.sanitize.php
      $url = filter_var($me['url'], FILTER_VALIDATE_URL);
      $img = filter_var($me['image']['url'], FILTER_VALIDATE_URL);
      $name = filter_var($me['displayName'], FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_HIGH);
      $personMarkup = "<a rel='me' href='$url'>$name</a><div><img src='$img'></div>";
    
      $optParams = array('maxResults' => 100);
      $activities = $plus->activities->listActivities('me', 'public', $optParams);
      $activityMarkup = '';
      foreach($activities['items'] as $activity) {
        // These fields are currently filtered through the PHP sanitize filters.
        // See http://www.php.net/manual/en/filter.filters.sanitize.php
        $url = filter_var($activity['url'], FILTER_VALIDATE_URL);
        $title = filter_var($activity['title'], FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_HIGH);
        $content = filter_var($activity['object']['content'], FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_HIGH);
        $activityMarkup .= "<div class='activity'><a href='$url'>$title</a><div>$content</div></div>";
      }
    
      // The access token may have been updated lazily.
      $_SESSION['access_token'] = $client->getAccessToken();
    } else {
      $authUrl = $client->createAuthUrl();
    }
    

}
?>