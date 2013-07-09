// $Id$
(function ($) {
  $(document).ready(function() {
    /**
     * 'Add new comment' behavior
     */
    $('div.comments a.new').click(function(){
      var commentBox = $('div.comment-box'); 
      var QAcontainer = $(this).parents('div.question, div.answer');
      var referenceNid = $('input[type="hidden"]:first', QAcontainer).val();
      
      commentBox.show();
      
      // Move comment container to selected question/answer
      commentBox.insertAfter($('div.comments', QAcontainer));
      
      // Populate input field reference with selected node id
      $('form#contribute-new-comment-form input[name="reference"]').val(referenceNid);
    });
    
    /**
     * Cancel Button behavior
     */
    $('div.comment-box input#edit-cancel').click(function(e){
      e.preventDefault();
      $('div.comment-box').hide();
    });
    
  });
})(jQuery);
