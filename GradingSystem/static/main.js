import { $ } from "/static/jquery/src/jquery.js";

export function say_hi(elt) {
    console.log("Say hi to", elt);
}

function make_grade_hypothesized(table) {
    // Create the Hypothesize button
    var hypothesizeButton = $('.hypothesize-button');

    // Click handler for the Hypothesize button
    hypothesizeButton.on('click', function() {
        if ($(table).hasClass('hypothesized')) {
            // Switch to actual grades
            $(this).text('Hypothesize');
            $(table).removeClass('hypothesized');
            restoreOriginalGrades(table);
        } else {
            // Switch to hypothesized grades
            $(this).text('Actual grades');
            $(table).addClass('hypothesized');
            replaceGradesWithInputs(table);
            recalculateGrades(table);
        }

    });


}

function replaceGradesWithInputs(table) {
    // Store the original total grade
    var originalTotalGrade = $('td.final-grade-display').text();
    $(table).data('original-total-grade', originalTotalGrade);

    $(table).find('td.grade-cell').each(function() {
        var content = $(this).text().trim();
        if (content === 'Not Due' || content === 'Ungraded') {
            var input = $('<input>', { type: 'number', min: 0, max: 100, class: 'hypothesize-input' });
            $(this).data('original-content', content);
            $(this).empty().append(input);

            input.change(function() {
                recalculateGrades(table);
            });
        }
    });
}

function restoreOriginalGrades(table) {
    // Restore the original total grade
    var originalTotalGrade = $(table).data('original-total-grade');
    if (originalTotalGrade !== undefined) {
        var originalGrade = $('<strong>').text(originalTotalGrade);
        $('td.final-grade-display').empty().append(originalGrade);
    }

    $(table).find('td.grade-cell').each(function() {
        if ($(this).data('original-content')) {
            $(this).text($(this).data('original-content'));
        }
    });
}

function recalculateGrades(table) {
    var earnedPoints = 0;
    var availablePoints = 0;

    $(table).find('tbody tr').each(function() {
        var weight = parseFloat($(this).find('td').data('weight'));
        var gradeCell = $(this).find('td.grade-cell');
        var gradeText = gradeCell.find('input.hypothesize-input').val() || gradeCell.text().trim();
        var grade = parseFloat(gradeText);

        if(gradeText === "Missing"){
            grade = 0;
        }

        if (gradeText !== "Not Due" && gradeText !== "Ungraded" && !isNaN(grade) && grade >= 0) {           
                earnedPoints += (grade / 100) * weight;
                availablePoints += weight; 
        }

        //console.log(`earnedPoints=${earnedPoints}, availablePoints=${availablePoints}, weight=${weight}, grade=${grade}, gradeText=${gradeText}%`); 
    });
    
    var finalGrade = availablePoints > 0 ? (earnedPoints / availablePoints * 100) : 0;
    var strongFinalGrade = $('<strong>').text(finalGrade.toFixed(2) + '%');
    $('td.final-grade-display').empty().append(strongFinalGrade);

    //console.log(`Total Earned Points=${earnedPoints}, Total Available Points=${availablePoints}, Final Grade=${finalGrade}%`);
}

function make_form_async(form){
    $(form).on('submit', function(event) {
        event.preventDefault();

        // Disable the submit button to prevent multiple submissions
        $(this).find('button[type="submit"]').prop('disabled', true);

        // Create FormData object from the form
        var formData = new FormData(this);

        // Retrieve the CSRF token
        var csrftoken = $(this).find('[name=csrfmiddlewaretoken]').val();

        // Get enctype from the form
        var enctype = $(this).attr('enctype');

        // AJAX request
        $.ajax({
            url: $(this).attr('action'),
            type: 'POST',
            data: formData,
            processData: false,
            contentType: false,
            mimeType: enctype,
            beforeSend: function(xhr) {
                xhr.setRequestHeader("X-CSRFToken", csrftoken);
            },
            success: function(response) {
                // Handle success: replace the form with a success message
                $(form).replaceWith('<div class="succeeded-form">Upload succeeded </div>');
            },
            error: function(xhr, status, error) {
                // Handle error: log the error and re-enable the submit button
                console.log("Error: " + error);
                $(form).find('button[type="submit"]').prop('disabled', false);
            }
        });
    });
}

function make_table_sortable(table){
    $(table).find("th.sortable").on("click", function () {
        var index = $(this).index();
        var tbody = $(table).find("tbody");
        var rows = tbody.find("tr").toArray();
        var isAscending = $(this).hasClass("sort-asc");
        var isDescending = $(this).hasClass("sort-desc");


        if (isDescending) {
            // Restore original order based on data-index
            rows.sort(function (a, b) {
                return $(a).data("index") - $(b).data("index");
            });
        } else {
            // Sort based on data-value or text content
            rows.sort(function (a, b) {
                var aValue = $(a).find("td").eq(index).data("value") || $(a).find("td").eq(index).text();
                var bValue = $(b).find("td").eq(index).data("value") || $(b).find("td").eq(index).text();

                aValue = isNaN(aValue) ? aValue : parseFloat(aValue);
                bValue = isNaN(bValue) ? bValue : parseFloat(bValue);

                return isAscending ? (aValue > bValue ? 1 : -1) : (aValue < bValue ? 1 : -1);
            });
        }

        // Re-append sorted rows to tbody
        $(rows).appendTo(tbody);

        // Update sorting state classes
        $(table).find("th").removeClass("sort-asc sort-desc");
        if (isAscending) {
            $(this).addClass("sort-desc");
        } else if (!isDescending) {
            $(this).addClass("sort-asc");
        }
    });


}

say_hi($("h1"));

//Apply sorting function to each table
$(document).ready(function() {
    // Apply sorting functionality to each table on the page
    $("table").each(function() {
        make_table_sortable(this);
    });

    // Apply async form submission to the student submission form
    var submissionForm = $('.submission-form');
    if (submissionForm.length) {
        make_form_async(submissionForm);
    }

    var gradeTable = $('.main-content');
    if (gradeTable.length) {
        make_grade_hypothesized(gradeTable);
    }      
});
