// $Id$
(function ($) {
  $(document).ready(function() {

    // Voting UP
    $('div.answer div.vote a.vote-up').click(function(){
      var answerNid = $('input', $(this).parent()).val();
      sendNewAnswerVote(answerNid, 'up');
    });
    
    // Voting Down
    $('div.answer div.vote a.vote-down').click(function(){
      var answerNid = $('input', $(this).parent()).val();
      sendNewAnswerVote(answerNid, 'down');
    });

    function sendNewAnswerVote(answerNid, voteType) {
      var posting = $.post('/post/answer/vote/' + answerNid + '/' + voteType);

      posting.done(function(data) {
        var returnedDataObj = JSON.parse(data);
        
        if (returnedDataObj.status == 'success') {
          // Vote Success
          $('div.answer-nid-' + returnedDataObj.answer + ' span.vote-count-post div.field-item').html(returnedDataObj.votes);
        }
        else if(returnedDataObj.status == 'error') {
          // Vote Error
          alert(returnedDataObj.message);
        }
      });
    };
    
  });
})(jQuery);
