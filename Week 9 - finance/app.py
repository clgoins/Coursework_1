import os
import sys

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    # get the users name and current cash for the header
    user = db.execute("SELECT * FROM users WHERE id=?", session["user_id"])[0]
    username = user["username"]
    cash = user["cash"]

    # total value of users portfolio
    total = cash

    # total up the number of stocks the user has traded
    allTransactions = db.execute(
        "SELECT id, symbol, SUM(quantity) as shares FROM transactions WHERE userid=? GROUP BY symbol", session["user_id"])
    holdings = []

    # if the user currently owns 1 or more shares in a stock they've traded, add it to the holdings list
    for stock in allTransactions:
        if stock["shares"] > 0:
            holdings.append(stock)

    # add and remove any necessary information to holdings before passing it to render template
    # index.html is expecting symbol, shares held, company name, price per share, and total value
    for stock in holdings:
        # look up stock info
        stockInfo = lookup(stock["symbol"])

        # store name, price, and total holding value in dictionary
        stock["name"] = stockInfo["name"]
        stock["price"] = usd(stockInfo["price"])
        stock["value"] = usd(stockInfo["price"] * stock["shares"])
        total += stockInfo["price"] * stock["shares"]

    return render_template("index.html", username=username, cash=usd(cash), total=usd(total), holdings=holdings)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")

    else:
        # get user input from form submission
        symbol = request.form.get("symbol")
        shareInput = request.form.get("shares")

        # check if shares and symbol are valid
        if symbol == "":
            return apology("Stock symbol cannot be blank")

        if lookup(symbol) == None:
            return apology("Stock symbol does not exist")

        try:
            shares = int(shareInput)
        except ValueError:
            return apology("Invalid quantity entered")

        if shares <= 0:
            return apology("Invalid quantity entered")

        # get total cost of transaction based on current share price
        cost = lookup(symbol)["price"] * shares

        # check if user has enough cash
        if cost <= db.execute("SELECT * FROM users WHERE id=?", session["user_id"])[0]["cash"]:
            # if user has money, buy stock, add to transactions db
            db.execute("INSERT INTO transactions (userid, symbol, quantity, price, total, time, type) VALUES (?, ?, ?, ?, ?, ?, ?)",
                       session["user_id"], symbol, shares, cost/shares, cost, datetime.now().strftime("%m/%d/%Y, %H:%M:%S"), "BUY")
            # decrement money from user in users table
            db.execute("UPDATE users SET cash=cash-? WHERE id=?", cost, session["user_id"])
        else:
            # if user has no money, render apology
            return apology("Insufficient funds for transaction")

        return redirect("/")


@app.route("/history")
@login_required
def history():
    # get a list of all past transactions on users account, with most recent first
    transactions = db.execute("SELECT * FROM transactions WHERE userid=? ORDER BY time DESC", session["user_id"])

    # index.html is expecting symbol, name, quantity, price, value, time
    for transaction in transactions:
        # get transaction info
        transactionInfo = lookup(transaction["symbol"])
        transaction["name"] = transactionInfo["name"]
        transaction["value"] = usd(transaction["total"])
        transaction["price"] = usd(transaction["price"])

    return render_template("history.html", transactions=transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

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

    if "quote" not in session:
        session["quote"] = []

    if request.method == "GET":
        return render_template("quote.html", quotes=session["quote"])

    else:
        # add form input to a list
        quote = lookup(request.form.get("symbol"))

        if quote == None:
            return apology("Stock symbol does not exist")

        # format the price in USD
        quote["price"] = usd(quote["price"])

        # add stock to users session
        session["quote"].append(quote)

        # display the list in a table on the page
        return render_template("quote.html", quotes=session["quote"])


@app.route("/resetQuote", methods=["POST"])
@login_required
def resetQuote():
    session["quote"] = []
    return redirect("/quote")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # if requested by GET, show the register page
    if request.method == "GET":
        return render_template("register.html")

    # if requested by POST from form submission;
    else:
        # get the values entered in each field
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # if username is empty, render and apology
        if username == "":
            return apology("Username cannot be blank")

        # if password is empty, render apology
        if password == "":
            return apology("Password cannot be blank")

        # if password and confirmation don't match, render apology
        if password != confirmation:
            return apology("Passwords do not match")

        # if all three fields are valid so far, check that the username doesn't already exist
        if db.execute("SELECT * FROM users WHERE username=?", username) != []:
            return apology("Username already exists")

        # if the username and password are valid, store the username and a hash of the password into the database
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, generate_password_hash(password))

        # return to homepage
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    allTransactions = db.execute(
        "SELECT symbol, SUM(quantity) as shares FROM transactions WHERE userid=? GROUP BY symbol", session["user_id"])
    holdings = []

    # if used no longer owns shares of a stock, remove it from the list
    for stock in allTransactions:
        if stock["shares"] > 0:
            holdings.append(stock)

    if request.method == "GET":
        # populate a list of stock symbols the user owns
        return render_template("sell.html", holdings=holdings)

    else:
        symbolToSell = request.form.get("symbol")
        sharesToSellInput = request.form.get("shares")

        # initialize stockToSell as empty
        stockToSell = None

        # get the specific stock out of the list of user's holdings
        for stock in holdings:
            if stock["symbol"] == symbolToSell:
                stockToSell = stock
                break

        # if user does not own shares of given stock, return apology
        if stockToSell == None:
            return apology("Invalid stock selected")

        # make sure shares entered is a number
        try:
            sharesToSell = int(sharesToSellInput)
        except ValueError:
            return apology("Invalid quantity entered")

        # validate that shares entered is greater than 0
        if sharesToSell < 1:
            return apology("Invalid quantity entered")

        # make sure user has enough shares to sell
        if sharesToSell > stockToSell["shares"]:
            return apology("Cannot sell more shares than user owns")

        # get current selling price of stock
        price = lookup(symbolToSell)["price"]

        # insert a sales transaction into DB
        db.execute("INSERT INTO transactions (userid, symbol, quantity, price, total, time, type) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], symbolToSell, -sharesToSell, price, -sharesToSell*price, datetime.now().strftime("%m/%d/%Y, %H:%M:%S"), "SELL")
        # add money to user in users table
        db.execute("UPDATE users SET cash=cash+? WHERE id=?", price*sharesToSell, session["user_id"])
        return redirect("/")
