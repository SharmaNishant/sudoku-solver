//to display selected file name
$("#file_select").on("change", function()
{
    //check for filesize
    var fiveMBSize = 5242880;
    var fileSelectElement = this;
    if( fileSelectElement.files[0].size > fiveMBSize ){
        alert('This file size is larger than 5 MB\'s.');
        return;
    }
    //regex used to extract filename from the element
    var value = fileSelectElement.value.split(/[\/\\]/).pop();

    //TODO: add file extension check

    $("#file_select_label").val(value);

});


//when solveit button is clicked
function solveSudokuFile() {
    var fileSelectElement, selectedFile;

    fileSelectElement = document.getElementById('file_select');
    if (!fileSelectElement) {
        alert("Um, couldn't find the fileinput element.");
    }
    else if (!fileSelectElement.files) {
        alert("This browser doesn't seem to support the `files` property of file inputs.");
    }
    else if (!fileSelectElement.files[0]) {
        alert("Please select a file before clicking 'Solve it!'");
    }
    else {
        selectedFile = fileSelectElement.files[0];
        alert("Successful");
    }
}


