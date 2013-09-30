// $Id$
(function ($) {
  $(document).ready(function() {
    votesInit();
    bestAnswerInit();
  });

  /**
   * Implements votes behavior
   */
  function votesInit() {
    $('div.vote a.vote-up, div.vote a.vote-down').click(function(){
      var nid = $('input[type="hidden"]:first', $(this).parents()).val();
      var voteElement = $(this).closest('div').parent();
      var voteValue = $(this).attr('class');
      
      sendVoteRequest(nid, voteElement, voteValue);
    });
  }
  
  /**
   * Send a post request to save the user vote
   */
  function sendVoteRequest(nid, voteElement, voteValue) {
    if ($(voteElement).hasClass('question')) {
      var posting = $.post('/post/question/vote/' + nid + '/' + voteValue);
    }
    else if ($(voteElement).hasClass('answer')) {
      var questionNid = Drupal.settings.contribute.questionNid;
      var posting = $.post('/post/answer/vote/' + questionNid + '/' + nid + '/' + voteValue);
    }
    
    if (typeof posting != 'undefined') {
      posting.done(function(data) {
        var returnedDataObj = JSON.parse(data);
        
        if (returnedDataObj.status == 'success') {
          // Dynamically update votes number
          $('span.vote-count-post', voteElement).html(returnedDataObj.votes);
        }

        // Display Glow message
        $.gritter.add({
          title: returnedDataObj.title,
          text: returnedDataObj.message
        });
      });
    }
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
