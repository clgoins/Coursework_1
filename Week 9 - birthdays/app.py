import os
import sys

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: add some default text to input boxes so the user knows what to type where

        # Get users input
        name = request.form.get("birthdayName")
        monthInput = request.form.get("birthdayMonth")
        dayInput = request.form.get("birthdayDay")

        # validate input, make sure numbers are valid
        try:
            month = int(monthInput)
            day = int(dayInput)
        except ValueError:
            return redirect("/")

        if month < 1 or day < 1:
            return redirect("/")

        # add entry to database
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)

        # redirect to homepage via GET
        return redirect("/")

    else:

        # create a list of dictionaries from the birthday db
        birthdays = db.execute("SELECT * FROM birthdays")

        # render the page, passing the dict list to the index.html template
        return render_template("index.html", birthdayList=birthdays)


@app.route("/remove/", methods=["GET", "POST"])
def remove():
    if request.method == "POST":
        removalID = request.form.get("id")
        db.execute("DELETE FROM birthdays WHERE id = ?", removalID)

        return redirect("/")

    else:
        return redirect("/")
