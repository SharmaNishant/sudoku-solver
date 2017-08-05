
//code for reading and siplaying selected filename
/*$(function() {

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


});*/

//to display selected file name
$("#file_select").on("change", function()
{
    if( (this.files[0].size/1024/1024) > 5 ){
        alert('This file size is: ' + this.files[0].size/1024/1024 + "MB.");
        return;
    }
    var value = this.value.split(/[\/\\]/).pop();
    $("#file_select_label").val(value);
});


//when solveit button is clicked
function solveSudokuFile() {
    var input, file;

    input = document.getElementById('file_select');
    if (!input) {
        alert("Um, couldn't find the fileinput element.");
    }
    else if (!input.files) {
        alert("This browser doesn't seem to support the `files` property of file inputs.");
    }
    else if (!input.files[0]) {
        alert("Please select a file before clicking 'Solve it!'");
    }
    else {
        file = input.files[0];
        alert("Successful");
    }
}


$(function(){ 

$(document).ready(function () {  
  $('#sudokusolver').append($('<div>').addClass('wrapper')
        .append($('<div>').addClass('col')
          .append(generateSudokuGrid())));
});  


function generateSudokuGrid(data) {
    return $('<table>').append( rowPush(9, function () {
        return $('<tr>').append( colPush(9, arguments[0], function () {
            return $('<td class="cell"><input type="number" min="0" max="9" id="input'+ arguments[1] + arguments[0] + '"></div>');
        }));
    })).addClass('sudoku');
}

function rowPush(count, func, scope) {
    var arr = [];
    for (var i = 0; i < count; i++) {
        arr.push(func.call(scope, i));
    }
    return arr;
}

function colPush(count, rowNum, func, scope) {
    var arr = [];
    for (var i = 0; i < count; i++) {
        arr.push(func.call(scope, i, arguments[1]));
    }
    return arr;
}


});

