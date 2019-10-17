import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    total_val = 0
    SQL_exec = db.execute(
        "SELECT symbol, SUM(shares) as 'shrssumm' FROM transactions WHERE userid=:userid GROUP BY symbol HAVING SUM(shares)>0", userid=session["user_id"])
    rows = db.execute("SELECT cash FROM users WHERE id=:userid", userid=session["user_id"])
    if len(SQL_exec) > 0:
        for row in SQL_exec:
            result = lookup(row["symbol"])
            row.update(name=result["name"], price=usd(result["price"]), total=usd(result["price"] * row["shrssumm"]))
            total_val += result["price"] * row["shrssumm"]
        # rows = db.execute("SELECT cash FROM users WHERE id=:userid",userid=session["user_id"])
        grand_total = float(rows[0]["cash"]) + total_val
        return render_template("index.html", result=SQL_exec, cash=usd(rows[0]["cash"]), grand_total=usd(grand_total))
    else:
        return render_template("index.html", result="", cash=usd(rows[0]["cash"]), grand_total=usd(rows[0]["cash"]))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Missing symbol", 400)
        if not request.form.get("shares"):
            return apology("Missing number of shares", 400)
        if not (request.form.get("shares").isdigit()) or (int(request.form.get("shares"))) < 0:
            return apology("Invalid quantity of shares", 400)
        result = lookup(request.form.get("symbol"))
        if not result:
            return apology("Invalid symbol", 400)

        # Money check
        # Request for user cash
        SQL_exec = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        cash = float(SQL_exec[0]["cash"])
        remainder = cash - float(result["price"]) * int(request.form.get("shares"))
        if remainder < 0:
            return apology("Not enough money", 402)

        # Insert values into table
        SQL_exec = db.execute("INSERT INTO transactions (userid, symbol, shares, price) VALUES (:userid, :symbol, :shares, :price)",
                              userid=session["user_id"], symbol=result["symbol"], shares=request.form.get("shares"), price=result["price"])

        # Update available cash
        SQL_exec = db.execute("UPDATE users SET cash=:remainder WHERE id=:id", remainder=remainder, id=session["user_id"])

        # FOR TESTING PURPOSES
        flash('You have successfully bought shares')

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # return redirect("/buy")
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    SQL_exec = db.execute("SELECT id FROM users WHERE username=:username", username=request.args.get("username"))
    if len(SQL_exec) != 0:
        return jsonify(False)
    else:
        return jsonify(True)
    # return jsonify("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    SQL_exec = db.execute("SELECT symbol,shares,price,trats FROM transactions WHERE userid=:userid", userid=session["user_id"])
    for row in SQL_exec:
        row.update(price=usd(row["price"]))
    return render_template("history.html", result=SQL_exec)


@app.route("/pchange", methods=["GET", "POST"])
@login_required
def pchange():
    """Change password for user"""
    if request.method == "POST":
        # implementation
        if not request.form.get("curr_password"):
            return apology("The field 'Current Password' is blank", 403)
        if not request.form.get("password"):
            return apology("Check new password", 403)
        elif not request.form.get("confirm"):
            return apology("Check new password confirm", 403)
        elif request.form.get("password") != request.form.get("confirm"):
            return apology("Check new password's", 403)

        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        if not check_password_hash(rows[0]["hash"], request.form.get("curr_password")):
            return apology("Check entered current password!", 403)

        # Update password
        SQL_exec = db.execute("UPDATE users SET hash=:hash WHERE id=:id", hash=generate_password_hash(
            request.form.get("password")), id=session["user_id"])

        # flashing success
        flash('Your password has been changed')
        # Redirect user to home page
        return redirect("/")
    else:
        return render_template("pchange.html")


@app.route("/purge", methods=["GET", "POST"])
@login_required
def purge():
    """Change password for user"""
    if request.method == "POST":
        # implementation
        if not request.form.get("password") or not request.form.get("confirm"):
            return apology("Password is blank", 403)
        elif request.form.get("password") != request.form.get("confirm"):
            return apology("Check password's", 403)

        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        if not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("Check entered current password!", 403)

        # Delete from WebApp
        SQL_exec = db.execute("DELETE FROM users WHERE id=:id", id=session["user_id"])
        SQL_exec = db.execute("DELETE FROM transactions WHERE userid=:id", id=session["user_id"])

        # Forget any user_id
        session.clear()

        # Redirect user to home page
        return redirect("/")
    else:
        return render_template("purge.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # FOR TESTING PURPOSES
        flash('Welcome!')

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Missing symbol", 400)
        result = lookup(request.form.get("symbol"))
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # render GET submission
        # result = lookup(request.args.get("symbol")
        # or redirect
        # return redirect("/quote")
        return render_template("quote.html")
    if result:
        return render_template("quoted.html", result=result, usd_price=usd(result["price"]))
    else:
        return apology("Invalid symbol", 400)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # TODO
    if request.method == "POST":
        # Ensure username, password or password confirm was submitted
        if not request.form.get("username"):
            return apology("Missing username!", 400)
        elif not request.form.get("password"):
            return apology("Missing password!", 400)
        elif not request.form.get("confirmation"):
            return apology("Missing confirm for password!", 400)
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("Password's does not match", 400)

        # Query database for username
        rows = db.execute("SELECT id FROM users WHERE username = :username", username=request.form.get("username"))

        # Ensure username does not exists
        if rows:
            return apology("username already taken", 400)

        # Ensure that password and confirm are the same
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password and confirm do not match", 400)
        # Generate hash from password
        hashed_pass = generate_password_hash(request.form.get("password"))
        # Insert values into table
        SQL_exec = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashed_pass)",
                              username=request.form.get("username"), hashed_pass=hashed_pass)

        SQL_exec = db.execute("SELECT id FROM users WHERE username = :username",
                              username=request.form.get("username"))

        # Remember which user has register
        session["user_id"] = SQL_exec[0]["id"]

        # FOR TESTING PURPOSES
        flash('You are registered!')

        # Redirect user to login form
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
        # return apology("please use POST", 403)
        # another path
        # return redirect("/register")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        SQL_exec = db.execute(
            "SELECT symbol FROM transactions WHERE userid=:userid GROUP BY symbol HAVING SUM(shares)>0", userid=session["user_id"])
        return render_template("sell.html", rows=SQL_exec)
    else:
        SQL_exec = db.execute(
            "SELECT symbol, SUM(shares) AS 'shrssumm' FROM transactions WHERE userid=:userid GROUP BY symbol HAVING SUM(shares)>0", userid=session["user_id"])
        symbol_present = 0
        for row in SQL_exec:
            if request.form.get("symbol") == row["symbol"]:
                if int(row["shrssumm"]) >= int(request.form.get("shares")):
                    # Lookup current price for stock
                    result = lookup(request.form.get("symbol"))
                    # Insert values into transactions table
                    SQL_exec = db.execute("INSERT INTO transactions (userid, symbol, shares, price) VALUES (:userid, :symbol, :shares, :price)",
                                          userid=session["user_id"], symbol=request.form.get("symbol"), shares=(0 - int(request.form.get("shares"))), price=result["price"])
                    # Update available cash
                    # Retrieve value
                    SQL_exec = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
                    # Calculate
                    remainder = float(SQL_exec[0]["cash"]) + float(request.form.get("shares")) * result["price"]
                    # return render_template("test.html",rem=remainder,t=float(SQL_exec[0]["cash"]),g=float(request.form.get("shares")),b=result["price"])
                    # Update
                    SQL_exec = db.execute("UPDATE users SET cash=:remainder WHERE id=:id",
                                          remainder=remainder, id=session["user_id"])

                    # Flashing
                    flash("You successfully sell it!")

                    # Redirect user to home page
                    return redirect("/")

                else:
                    return apology("You do not have enough shares", 400)
        return apology("Invalid symbol", 400)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
