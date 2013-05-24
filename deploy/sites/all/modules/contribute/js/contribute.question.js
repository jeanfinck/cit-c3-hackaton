jQuery(function() {
  initSideBarBlocks();

  initTags();
});

function initSideBarBlocks() {
  jQuery('div.region-sidebar-second .block').hide();

  jQuery('#edit-title').focus(function(){
    jQuery('div.region-sidebar-second .block').hide();
    jQuery('#block-block-1').fadeIn();
  });

  CKEDITOR.instances["edit-question-value"].on('focus', function(e) {
    jQuery('div.region-sidebar-second .block').hide();
    jQuery('#block-block-2').fadeIn();
  });

  jQuery('#edit-term-entry').focus(function(){
    jQuery('div.region-sidebar-second .block').hide();
    jQuery('#block-block-3').fadeIn();
  });

  jQuery('#edit-title').focus();
}

function initTags() {
  tagCount = 0;
  
  jQuery('#edit-term-entry').keyup(function(e) {
    if (!jQuery(this).val()) {
      return;
    }
    
    if (e.keyCode == 32 || e.keyCode == 13) {
      if (tagCount < 5) {
        tagCount++;
        jQuery('#edit-add-button').click();
      }
      else {
        alert(Drupal.t("Only 5 tags are allowed.\nPlease remove one tag for adding another."));
      }
    }
  });
  
  jQuery('#contribute-question-form').on('click', 'span.at-term-action-remove', function(e){
    tagCount--;
  });
}
