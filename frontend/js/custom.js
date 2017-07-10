$(function() {

  // We can attach the `fileselect` event to all file inputs on the page
  $(document).on('change', ':file', function() {
    var input = $(this),
        numFiles = input.get(0).files ? input.get(0).files.length : 1,
        label = input.val().replace(/\\/g, '/').replace(/.*\//, '');
    input.trigger('fileselect', [numFiles, label]);
  });

  // We can watch for our custom `fileselect` event like this
  $(document).ready( function() {
      $(':file').on('fileselect', function(event, numFiles, label) {

          var input = $(this).parents('.input-group').find(':text'),
              log = numFiles > 1 ? numFiles + ' files selected' : label;

          if( input.length ) {
              input.val(log);
          } else {
              if( log ) alert(log);
          }

      });
      
  });


});



$(function(){ 

$(document).ready(function () {  
  $('#sudokusolver').append($('<div>').addClass('wrapper')
        .append($('<div>').addClass('col')
          .append(generateSudokuGrid())));
});  


function generateSudokuGrid(data) {
    return $('<table>').append(multiPush(9, function () {
        return $('<tr>').append(multiPush(9, function () {
            return $('<td class="cell"><input type="number" maxlength="1"></div>');
        }));
    })).addClass('sudoku');
}

function multiPush(count, func, scope) {
    var arr = [];
    for (var i = 0; i < count; i++) {
        arr.push(func.call(scope, i));
    }
    return arr;
}


});

