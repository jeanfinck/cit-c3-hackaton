// $Id$
(function ($) {
  $(document).ready(function() {
    /**
     * 'Add new comment' behavior
     */
    $('div.comments a.new').click(function() {
      $(this).hide();
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
    $('div.comment-box input#edit-cancel').click(function(e) {
      e.preventDefault();
      $('div.comments a.new').show();
      $('div.comment-box').hide();
      $('form#contribute-new-comment-form textarea#edit-comment', $(this).parents()).val('');
    });
    
    /**
     * Submit Button behavior
     */
    $('div.comment-box input#edit-submit--3').mousedown(function() {
      var commentBox = $('div.comment-box');
      var QAcontainer = $(this).parents('div.comments-container');
      var commentElement = $('form#contribute-new-comment-form textarea#edit-comment', QAcontainer);
      
      commentBox.hide();
      
      if (commentElement.val()) {
        $('div.comments', QAcontainer).append('<div class="comment">' + commentElement.val() + ' - Now</div>');
        $('div.comments a.new', QAcontainer).appendTo($('div.comments', QAcontainer));
        commentElement.val(''); // clear textarea of comment
      }
      
      $('div.comments a.new').show();
    });
    
  });
})(jQuery);
