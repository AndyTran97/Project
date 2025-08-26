from django.shortcuts import render
from . import models
from django.shortcuts import get_object_or_404, redirect
from django.utils import timezone
from decimal import Decimal
from django.http import Http404, HttpResponseBadRequest, HttpResponseNotAllowed
from django.views.decorators.http import require_POST
from django.contrib.auth import authenticate, login, logout
from django.contrib import messages
from django.http import HttpResponseRedirect
from django.contrib.auth.decorators import login_required, user_passes_test
import os
from django.db.models import Count, Q
from django.http import HttpResponse
from django.core.exceptions import PermissionDenied

# Views
def is_ta_or_admin(user):
    return user.groups.filter(name="Teaching Assistants").exists() or user.is_superuser

def is_student(user):
    return user.groups.filter(name="Students").exists()

def is_ta(user):
    return user.groups.filter(name="Teaching Assistants").exists()

def is_admin(user):
    return user.is_superuser

@login_required
def assignments(request):
    assignments = models.Assignment.objects.order_by("deadline")
    return render(request, "assignments.html", {"assignments": assignments})

def pick_grader(assignment):
    ta_group = models.Group.objects.get(name="Teaching Assistants")

    # Annotate each TA with the count of submissions for the specific assignment they need to grade
    ta_with_fewest_assignments = (
        ta_group.user_set
        .annotate(total_assigned = Count('graded_set__assignment', filter=Q(graded_set__assignment=assignment)))
        .order_by('total_assigned')
        .first()
    )

    return ta_with_fewest_assignments

@login_required
def show_upload(request, filename):
    # Look up the submission by filename
    try:
        submission = models.Submission.objects.get(file=filename)
    except models.Submission.DoesNotExist:
        raise Http404(f"No submission with filename {filename} found.")

    # Check user permissions
    user = request.user
    if not (user == submission.author or user == submission.grader or user.is_superuser):
        raise PermissionDenied

    # Serve the file
    with submission.file.open() as fd:
        response = HttpResponse(fd)
        response["Content-Disposition"] = \
            f'attachment; filename="{submission.file.name}"'
        return response
    

@login_required
@user_passes_test(is_student)
def submit(request, assignment_id):
    assignment = get_object_or_404(models.Assignment, pk=assignment_id)
    if assignment.deadline < timezone.now():
        return HttpResponseBadRequest("Assignment is past due")

    submitted_file = request.FILES.get('file')

    print(submitted_file)

    if not submitted_file:
        return HttpResponseBadRequest("No file submitted")

    submission, created = models.Submission.objects.get_or_create(
        assignment=assignment,
        author=request.user,
        defaults={'file': submitted_file, 'grader': pick_grader(assignment)}
    )
    if not created:
        submission.file = submitted_file
        submission.save()

    return redirect(f"/{assignment_id}/")

@login_required
def index(request, assignment_id):
    user = request.user
    username = user.username if user.is_authenticated else ""
    # Get Assignment by its ID
    try:
        assignment = get_object_or_404(models.Assignment, pk=assignment_id)
    except Http404: 
        raise Http404(f"Assignment with id {assignment_id} does not exist.")

    # Give the context of this assignment
    context = {
        "assignment": assignment,
        "description": assignment.description,
        "title": assignment.title,
        "is_student": is_student(user),
        "is_ta_or_admin": is_ta(user) or is_admin(user),
    }

    # Check if this user is TA or Admin
    if user.is_authenticated and (is_ta(user) or is_admin(user)):
        isDue = assignment.deadline < timezone.now()      
        # Total submissions for this assignment
        total_submissions = assignment.submissions.count()

        ta_submissions = assignment.submissions.filter(
            grader__username= username).count()

        # Total number of students
        try:
            total_students = models.Group.objects.get(
                name="Students").user_set.count()
        except models.Group.DoesNotExist:
            total_students = 0

        context.update({
            "total_submissions": total_submissions,
            "ta_submissions": ta_submissions,
            "total_students": total_students,
            "isDue" : isDue,
        })
    #Check if this user is Student    
    elif user.is_authenticated and is_student(user):
        isDue = False
        student_submission = assignment.submissions.filter(author=user).first()      
        if student_submission:
            isDue = True
            # Extract file name
            filename = os.path.basename(student_submission.file.name)
            if student_submission.score is not None:
                # For submitted, graded assignment
                score_percent = (student_submission.score / assignment.points) * 100
                submission_info = f"Your submission, <strong>{filename}</strong>, received <strong>{student_submission.score}/ {assignment.points: .1f}</strong> points (<strong>{score_percent: .2f}%</strong>)"
            elif assignment.deadline < timezone.now():
                # For submitted, ungraded, past due assignment
                submission_info = f"Your submission, <strong>{filename}</strong>, is being graded"
            else:
                # For submitted, not due assignment
                submission_info = f"Your current submission is <strong>{filename}</strong>"
                isDue = False
        else:
            if assignment.deadline < timezone.now():
                # For not submitted, past due assignment
                submission_info = "You did not submit this assignment and received 0 points"
                isDue = True
            else:
                # For not submitted, not due assignment
                submission_info = "No current submission"
                

        context.update({
            "submission_info": submission_info,
            "isDue" : isDue,
        })


    return render(request, "index.html", context)

@login_required
@user_passes_test(is_ta_or_admin) #to avoid students see /index/submissions pages, only ta or admin does
def submissions(request, assignment_id):
   # Assignment by its ID
    try:
        assignment = get_object_or_404(models.Assignment, pk=assignment_id)
    except Http404: 
        raise Http404(f"Assignment with id {assignment_id} does not exist.")
   
    # get the username if user already log in
    username = request.user.username if request.user.is_authenticated else ""

    submissions = assignment.submissions.filter(
        grader__username= username).order_by('author__username')

    context = {
        "id": assignment_id,
        "assignment": assignment,
        "submissions": submissions,
        "title": assignment.title,
    }

    return render(request, "submissions.html", context)

@login_required
def profile(request):
    assignments = models.Assignment.objects.order_by("deadline")
    assignment_data = []
    total_grade = ""

    now = timezone.now()
    user = request.user
    username = user.username if user.is_authenticated else ""
    is_student_view = False

    # Check if this user is TA or Admin
    if user.is_authenticated and is_ta(user):    
        for assignment in assignments:
            if assignment.deadline > now:
                assignment_status = "Not Due"
            else:
                graded_submissions = assignment.submissions.filter(
                    score__isnull=False, grader__username= username).count()
                ta_submissions = assignment.submissions.filter(
                    grader__username= username).count()
                assignment_status = f"{graded_submissions}/{ta_submissions}"

            assignment_data.append({
                "id": assignment.id,
                "title": assignment.title,
                "assignment": assignment,
                "status": assignment_status
            })
    elif user.is_authenticated and is_admin(user):
        for assignment in assignments:         
            graded_submissions = assignment.submissions.filter(score__isnull=False).count()  
            total_submissions = assignment.submissions.count()               
            assignment_status = f"{graded_submissions}/{total_submissions}"

            assignment_data.append({
                "id": assignment.id,
                "title": assignment.title,
                "assignment": assignment,
                "status": assignment_status
            })
    elif user.is_authenticated and is_student(user):
        is_student_view = True
        earned_points = 0
        available_points = 0

        for assignment in assignments:
            student_submission = assignment.submissions.filter(author__username=username).first()
            assignment_status = ""
            score = 0
            weight = assignment.weight
            if student_submission:
                if student_submission.score is not None:
                    assignment_status = "{:.2f}%".format(student_submission.score / assignment.points * 100)
                    score = student_submission.score / assignment.points * weight
                    earned_points += score
                    available_points += weight                   
                else:
                    assignment_status = "Ungraded"
            else: # No submission
                if assignment.deadline > now:
                    assignment_status = "Not Due"
                else:
                    assignment_status = "Missing"    


            assignment_data.append({
                "id": assignment.id,
                "title": assignment.title,
                "assignment": assignment,
                "status": assignment_status,
                "weight": weight,
            })

            total_grade = (earned_points / available_points * 100) if available_points > 0 else 0    

    context = {
        "assignments": assignment_data,
        "username": username,
        "title": "Profile",
        "is_student": is_student_view,
        "total_grade": "{:.2f}%".format(total_grade) if user.is_authenticated and is_student(user) else None
    }
    return render(request, "profile.html", context)


def login_form(request):
    next_url = request.GET.get("next") or request.POST.get("next", "/profile/") 
   
    if request.method == 'POST':
        username = request.POST.get("username", "")
        password = request.POST.get("password", "")
           
        user = authenticate(request, username=username, password=password)

        if user is not None:
            login(request, user)     
            return redirect(next_url)          
        else:
            message = "Username and password do not match"
            return render(request, "login.html", {'error': message, 'next': next_url})


    return render(request, "login.html", {'next': next_url})

def logout_form(request):
    logout(request)
    return HttpResponseRedirect('/profile/login/')

@login_required
@require_POST
def grade(request, assignment_id):
    try:
        assignment = get_object_or_404(models.Assignment, pk=assignment_id)
    except Http404: 
        raise Http404(f"Assignment with id {assignment_id} does not exist.")
    
    # get the username if user already log in
    username = request.user.username if request.user.is_authenticated else ""

    submissions = assignment.submissions.filter(grader__username= username)
    for key, value in request.POST.items():
        if key.startswith('grade-'):
            try:
                submission_id = int(key.split('-')[1])
            except IndexError:
                continue
            except ValueError:
                raise Http404(f"Invalid submission ID in {key}")
            
            try:
                submission = submissions.get(pk=submission_id)
            except models.Submission.DoesNotExist:
                raise Http404(f"Submission with id {submission_id} does not exist.")

            try:
                score = float(value)
            except ValueError:
                score = None
           
            submission.score = score
            submission.save()
            
    return redirect(f"/{assignment_id}/submissions")
