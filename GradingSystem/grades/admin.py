from django.contrib import admin
from .models import Assignment, Submission

#Register your models here.
@admin.register(Assignment)
class AssignmentAdmin(admin.ModelAdmin):
    list_display = ('title', 'deadline', 'weight', 'points')
    search_fields = ('title', 'description')
    list_filter = ('deadline',)

@admin.register(Submission)
class SubmissionAdmin(admin.ModelAdmin):
    list_display = ('assignment', 'author', 'grader', 'score')
    search_fields = ('assignment__title', 'author__username', 'grader__username')
    list_filter = ('author', 'grader')


