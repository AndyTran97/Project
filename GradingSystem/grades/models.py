from django.db import models
from django.contrib.auth.models import User, Group
from django.core.validators import MinValueValidator

# Models
class Assignment(models.Model):
    title = models.CharField(max_length=200)
    description = models.TextField()
    deadline = models.DateTimeField()
    weight = models.IntegerField(validators=[MinValueValidator(0)])
    points = models.IntegerField(validators=[MinValueValidator(0)])

    def __str__(self):
        return self.title
    
class Submission(models.Model):
    assignment = models.ForeignKey(
        Assignment, 
        on_delete=models.CASCADE,
        related_name='submissions')
    author = models.ForeignKey(
        User, 
        on_delete=models.CASCADE,
        related_name='author_set')
    grader = models.ForeignKey(
        User,
        on_delete = models.SET_NULL, 
        related_name='graded_set',
        blank=True, 
        null=True)
    file = models.FileField(upload_to='')
    score = models.FloatField(validators=[MinValueValidator(0)], blank=True, null=True, default= None)

    # toString method for Title and Username
    def __str__(self):
        return f"Submission of '{self.assignment.title}' by {self.author.username}"
    
    # Unique (Assignment, Author)
    class Meta:
        unique_together = ('assignment', 'author')                            