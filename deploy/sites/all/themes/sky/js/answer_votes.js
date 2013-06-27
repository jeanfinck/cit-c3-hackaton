// $Id$
(function ($) {
  $(document).ready(function() {

    // Voting UP
    $('div.answer div.vote a.vote-up').click(function(){
      var answerNid = $('input', $(this).parent()).val();
      var questionNid = Drupal.settings.contribute.questionNid;
      sendNewAnswerVote(questionNid, answerNid, 'up');
    });
    
    // Voting Down
    $('div.answer div.vote a.vote-down').click(function(){
      var answerNid = $('input', $(this).parent()).val();
      var questionNid = Drupal.settings.contribute.questionNid;
      sendNewAnswerVote(questionNid, answerNid, 'down');
    });

    function sendNewAnswerVote(questionNid, answerNid, voteType) {
      var posting = $.post('/post/answer/vote/' + questionNid + '/' + answerNid + '/' + voteType);

      posting.done(function(data) {
        var returnedDataObj = JSON.parse(data);
        
        if (returnedDataObj.status == 'success') {
          // Update votes number
          $('div.answer-nid-' + returnedDataObj.answer + ' span.vote-count-post').html(returnedDataObj.votes);
        }
        
        // Display Glow message
        $.gritter.add({
          title: returnedDataObj.title,
          text: returnedDataObj.message
        });
        
      });
    };
    
  });
})(jQuery);
