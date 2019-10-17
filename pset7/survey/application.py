import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    login = request.form.get("login")
    server = request.form.get("server")
    hash = request.form.get("hash")
    if not login:
        return render_template("error.html", message="You missed 'Login name' in the form")
    if not server:
        return render_template("error.html", message="You missed 'Server name' in the form")
    if not hash:
        return render_template("error.html", message="You missed 'Hash type' in the form")
    with open('survey.csv', 'a') as file:
        write = csv.writer(file)
        write.writerow((login, server, hash))
    return redirect("/sheet")
    # return render_template("error.html", message="TODO")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    with open('survey.csv', 'r') as file:
        read = csv.reader(file)
        nodes = list(read)
    return render_template("sheet.html", nodes=nodes)
    # return render_template("error.html", message="TODO")