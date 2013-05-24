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
  jQuery('#edit-term-entry').keypress(function(e) {
    if (e.charCode == 32) {
      jQuery('#edit-add-button').click();
    }
  });
}
