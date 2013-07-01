// $Id$
(function ($) {
  $(document).ready(function() {
    answerVotesInit();
    bestAnswerInit();
  });

  /**
   * Implements answer votes behavior
   */
  function answerVotesInit() {
    $('div.answer div.vote a.vote-up, div.answer div.vote a.vote-down').click(function(){
      var voteType = $(this).attr('class');
      var answerNid = $('input', $(this).parent()).val();
      var questionNid = Drupal.settings.contribute.questionNid;
      var dialogTitle = 'Are you sure?';
      var dialogMessage = 'Are you sure you want to make this vote?';
      
      newAnswerVote(questionNid, answerNid, voteType, dialogTitle, dialogMessage);
    });
  }
  
  /**
   * New answer vote behavior
   */
  function newAnswerVote(questionNid, answerNid, voteType, dialogTitle, dialogMessage) {
    $('body').append('<div id="dialog-confirm"><p>' + Drupal.t(dialogMessage) + '</p></div>');
    
    $("div#dialog-confirm").dialog({
      resizable: false,
      modal: true,
      draggable: false,
      title: Drupal.t(dialogTitle),
      buttons: {
        'OK': function() {
          $(this).dialog('close');
          $(this).remove();
          sendAnswerVoteRequest(questionNid, answerNid, voteType);
        },
        Cancel: function() {
          $(this).dialog('close');
          $(this).remove();
        }
      }
    });
  }
  
  /**
   * Send a post request to save the user vote
   */
  function sendAnswerVoteRequest(questionNid, answerNid, voteType) {
    var posting = $.post('/post/answer/vote/' + questionNid + '/' + answerNid + '/' + voteType);

    posting.done(function(data) {
      var returnedDataObj = JSON.parse(data);
      
      if (returnedDataObj.status == 'success') {
        // Dynamically update votes number
        $('div.answer-nid-' + returnedDataObj.answer + ' span.vote-count-post').html(returnedDataObj.votes);
      }
      
      // Display Glow message
      $.gritter.add({
        title: returnedDataObj.title,
        text: returnedDataObj.message
      });
      
    });
  };
  
  /**
   * Select Best Answer Behavior
   */
  function bestAnswerInit() {
    $('div.vote a.best-answer').click(function(e){
      e.preventDefault();
      
      var requestURL = $(this).attr('href');
      
      $('body').append('<div id="dialog-confirm"><p>' + Drupal.t('Are you sure you want to mark this answer as the best solution?') + '</p></div>');
      
      $("div#dialog-confirm").dialog({
        resizable: false,
        modal: true,
        draggable: false,
        title: Drupal.t('Are you sure?'),
        buttons: {
          'OK': function() {
            $(this).dialog('close');
            $(this).remove();
            window.location.href = requestURL;
          },
          Cancel: function() {
            $(this).dialog('close');
            $(this).remove();
          }
        }
      });
    });
  }
})(jQuery);
